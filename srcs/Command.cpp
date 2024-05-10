/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/10 15:39:24 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

int	flagCheck(std::string	str);

void	fctPASS(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	// if (client.getIsRegistered() == false)
	// {
		if (i->at(1) == server.getPass())
		{
			std::cout << "PASS" << std::endl;
			client.setPass(true);
		}
		else
		{
			std::string response;
			response = ":IRCServ 464 *: Password incorrect\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			//server.kickClient(client.getFd());
		}
	// }
}

void	fctNICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (server.nickAlreadyUsed(i->at(1)) == false)
		client.setNickname(i->at(1));
	else
	{
		std::string response;
		response = ":IRCServ 433 *: Nickname is already used. \r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
	if (client.getIsRegistered() == false && client.isRegistered() == true)	//permet de ignorer quand déjà  connecté
	{
		std::string response;
		response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + "@IRCServ\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}

void	fctUSER(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	(void)server;
	if (client.getIsRegistered() == false)	//permet de ignorer la fonction quand connecté
	{
		client.setUser(i->at(1));
		if (client.isRegistered() == true)
		{
			std::string response;
			response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + " @IRCServ\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
		}
	}
}

void	fctJOIN(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())	//si le channel n'existe pas
	{
		Channel *curChannel =  new Channel(i->at(1), client);
		server._mapChannel[curChannel->getName()] = curChannel;
		client.addChannel(curChannel);
		curChannel->addMember(&client);
	}
	else		// si existe deja:
	{
		if (server._mapChannel[i->at(1)]->getIsMdp() == true /*&& !i->at(2).empty()*/)	//si le channel a un mdp et qu'on a passé un mdp
		{																			//si pas de 2eme arguments TODO fix
			if (server._mapChannel[i->at(1)]->getMdp() != i->at(2))
				throw ("NR : wrong password");
		}
		/*else if (i->at(2).empty())				//si pas de 2eme arguments TODO fix : l'ecriture i->at().empty() throw un std::out_of_range
			throw ("NR : need password to join the channel");*/

		if (server._mapChannel[i->at(1)]->getInvitOnly() == true)
			throw ("NR : Channel is on invit only");
		if (server._mapChannel[i->at(1)]->getIsUserLimit() == true && server._mapChannel[i->at(1)]->getMemberSize() >= server._mapChannel[i->at(1)]->getIsUserLimit())
			throw ("NR : Channel is full");
		client.addChannel(server._mapChannel[i->at(1)]);
		server._mapChannel[i->at(1)]->addMember(&client);
	}
	throw("NR : IRCServ 0 " + client.getNick() + " : Join channel " + i->at(1) + ".\r\n");
	// std::string response;
	// response = ":IRCServ 0 " + client.getNick() + " : Join channel " + i->at(1) + ".\r\n";
	// send(client.getFd(), response.c_str(), response.length(), 0);
}




void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (i->size() != 3)
		throw ("NR : wrong number of arguments");
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(2))
		it++;
	if (it == server._mapClient.end())
		throw ("NR : client doesn't exist.");
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		throw ("NR : channel doesn't exist.");
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
		throw ("NR : you're not allowed to use this command (not a moderator)");
	it->second->rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(it->second);
	throw ("NR : kick successfully.");
}

// ---------------------------------------------------------------------//
//    Examples:	KICK

// KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

// KICK #Finnish John :Speaking English       ; Kick John from #Finnish using "Speaking English" as the reason (comment).

//  Numeric Replies:
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//            ERR_NOTONCHANNEL
// ---------------------------------------------------------------------//



void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		throw ("NR : Channel doesn't exist");
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
		throw ("NR : yoou're not allowed to use this command (not a moderator)");
	if (i->at(2).at(0) != '+' && i->at(2).at(0) != '-')
		throw ("NR : need operand + or - before flag");
	if (i->at(2).at(0) == '+')
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			throw ("NR : too many args, only 1 letter");
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(true);
			throw ("NR : invit only mode added successfully");
		case 2:
			std::cout << "+t.(todo)" << std::endl;
			break;
		case 3:
			if (i->at(3).empty())	//si pas de 3eme arguments TODO fix
				throw ("NR : new password missed");
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			throw ("NR : password added successfully");
		case 4:
			std::cout << "+o." << std::endl;
			break;
		case 5:
			if (i->at(3).empty()) //si pas de 3eme arguments TODO fix
				throw ("NR : new limit missed");
			server._mapChannel[i->at(1)]->setUserLimit(std::atoi(i->at(3).c_str()));
			throw ("NR : user limit added successfully");
		default:
			throw("NR : unknown flag");
		}
	}
	else
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			throw ("NR : too many args, only 1 letter");
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(false);
			throw ("NR : invit only mode removed successfully");
		case 2:
			std::cout << "-t." << std::endl;
			break;
		case 3:
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			throw ("NR : password removed successfully");
		case 4:
			std::cout << "-o." << std::endl;
			break;
		case 5:
			server._mapChannel[i->at(1)]->setUserLimit(0);
			throw ("NR : user limit added successfully");
		default:
			throw("NR : unknown flag");
		}
	}
	// i->at(2) = +-iktol
	// i->at(3) = okl arg
	
	(void)client;
}

int	flagCheck(std::string	str)
{
	if (str.size() > 2)
		return (0);
	if (str.find("i",0) == 1)
		return (1);
	if (str.find("t",0) == 1)
		return (2);
	if (str.find("k",0) == 1)
		return (3);
	if (str.find("o",0) == 1)
		return (4);
	if (str.find("l",0) == 1)
		return (5);
	return (-1);
}

// ---------------------------------------------------------------------//
//    Examples:	MODE

//            Use of Channel Modes:

// MODE #Finnish +i               ; Makes #Finnish channel 'invite-only'.

// MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on #Fins.

// MODE #42 +k oulu                ; Set the channel key to "oulu".

// MODE #eu-opers +l 10            ; Set the limit for the number of users on channel to 10.



void	fctINVITE()
{
	std::cout << "INVITE" << std::endl;
}

void	fctTOPIC()
{
	std::cout << "TOPIC" << std::endl;
}

