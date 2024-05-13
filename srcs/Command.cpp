/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/13 14:36:32 by tdutel           ###   ########.fr       */
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
		if (server._mapChannel[i->at(1)]->getInvitOnly() == true && server._mapChannel[i->at(1)]->isInvited(client.getNick()) == false)
			throw ("NR : Channel is on invit only");

		if (server._mapChannel[i->at(1)]->getIsMdp() == true && i->size() == 3)	//si le channel a un mdp et qu'on a passé un mdp
		{
			if (server._mapChannel[i->at(1)]->getMdp() != i->at(2))
				throw ("NR : wrong password");
		}
		else if (server._mapChannel[i->at(1)]->getIsMdp() == true)
			throw ("NR : need password to join the channel");

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
	if (i->size() != 4)	// 4 instead of 3 because of HexChat ('kick' 'irc' '#general' ':tim')
		throw ("NR : wrong number of arguments");
	if (server._mapChannel.find(i->at(2)) == server._mapChannel.end())
		throw ("NR : channel doesn't exist.");
	std::string usr = i->at(3).substr(1);		//	pour gérer ":" devant client pour Hexchat : 'kick' 'irc' '#general' ':tim'
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != usr)
		it++;
	if (it == server._mapClient.end())
		throw ("NR : client doesn't exist.");
	if (server._mapChannel[i->at(2)]->isMember(it->second->getNick()) == false)
		throw ("NR : client is not in the channel.");
	if (server._mapChannel[i->at(2)]->isModerator(client.getNick()) == false)
		throw ("NR : you're not allowed to use this command (not a moderator)");
	it->second->rmChannel(server._mapChannel[i->at(2)]);
	server._mapChannel[i->at(2)]->rmMember(it->second);
	if (server._mapChannel[i->at(2)]->isModerator(it->second->getNick()) == true)
		server._mapChannel[i->at(2)]->rmModerator(it->second);
	if (server._mapChannel[i->at(2)]->isInvited(it->second->getNick()) == true)
		server._mapChannel[i->at(2)]->rmInvitMember(it->second);
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


void	fctINVITE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (i->size() != 3)
		throw ("NR : Wrong numbers fo args");
	if (server._mapChannel.find(i->at(2)) == server._mapChannel.end())
		throw ("NR : Channel doesn't exist");
	if (server._mapChannel[i->at(2)]->isMember(client.getNick()) == false)
		throw ("NR : You must be in the channel to invite someone (?ERR_NOTONCHANNEL?)");
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(1))
		it++;
	if (it == server._mapClient.end())
		throw ("NR : client doesn't exist.");
	server._mapChannel[i->at(2)]->addInvitMember(it->second);
	throw ("NR : client invited successfully");
}

// i->at(1) = user
// i->at(2) = channel

// ---------------------------------------------------------------------//
// Examples:	INVITE

//    :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel #Dust

//    INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to #Twilight_zone

//  Numeric Replies:
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
//            ERR_CHANOPRIVSNEEDED
//            RPL_INVITING                    RPL_AWAY
// ---------------------------------------------------------------------//


void	fctTOPIC(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (i->size() != 4)
		throw ("NR : Wrong numbers fo args");
	std::string chan = i->at(2).substr(1);
	if (server._mapChannel.find(chan) == server._mapChannel.end())
		throw ("NR : Channel doesn't exist");
	if (server._mapChannel[chan]->getTopicRestriction() == true && server._mapChannel[chan]->isModerator(client.getNick()) == false)
		throw ("NR : you're not allowed to use this command (not a moderator)");

	throw ("Changed Topic successfully");
}
// ---------------------------------------------------------------------//
//  Examples:	TOPIC

//    :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

//    TOPIC #test :another topic      ;set the topic on #test to "another topic".

//    TOPIC #test                     ; check the topic for #test.

//  Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED
// ---------------------------------------------------------------------//



void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	if (i->size() != 3 && i->size() != 4)
		throw ("NR : Wrong numbers fo args");
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		throw ("NR : Channel doesn't exist");
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
		throw ("NR : you're not allowed to use this command (not a moderator)");
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
			// std::cout << "+t.(TODO) only +o can change topic" << std::endl;
			server._mapChannel[i->at(1)]->setTopicRestriction(true);
			throw ("NR : topic restriction added successfully");
		case 3:
		{
			if (i->size() != 4)
				throw ("NR : wrong numbers of args");
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			throw ("NR : password added successfully");
		}
		case 4:
		{
			if (i->size() != 4)
				throw ("NR : wrong numbers of args");
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
				throw ("NR : client doesn't exist.");
			server._mapChannel[i->at(1)]->addModerator(it->second);
			throw ("NR : moderator added successfully");
		}
		case 5:
			if (i->size() != 4)
				throw ("NR : wrong numbers of args");
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
			// std::cout << "-t. everyone can change topic" << std::endl;
			server._mapChannel[i->at(1)]->setTopicRestriction(false);
			throw ("NR : topic restriction removed successfully");
		case 3:
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			throw ("NR : password removed successfully");
		case 4:
		{

			if (i->size() != 4)
				throw ("NR : wrong numbers of args");
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
				throw ("NR : client doesn't exist.");
			server._mapChannel[i->at(1)]->rmModerator(it->second);
			throw ("NR : moderator removed successfully");
		}
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


void	fctTOPIC()
{
	std::cout << "TOPIC" << std::endl;
}

