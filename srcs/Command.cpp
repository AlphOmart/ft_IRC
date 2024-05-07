/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/07 12:27:43 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

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
	(void)client;
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(2))
		it++;
	if (it == server._mapClient.end())
		throw ("NR : client doesn't exist.");
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		throw ("NR : channel doesn't exist.");
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




void	fctINVITE()
{
	std::cout << "INVITE" << std::endl;
}

void	fctTOPIC()
{
	std::cout << "TOPIC" << std::endl;
}

