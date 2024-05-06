/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/06 15:55:01 by tdutel           ###   ########.fr       */
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
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())	//verifier si le channel n'existe pas
	{
		Channel *curChannel =  new Channel(i->at(1));		//TODO: creer un constructeur avec nick client pour MOD
		server._mapChannel[curChannel->getName()] = curChannel;
		client.addChannel(curChannel);
		curChannel->addMember(&client);
	}
	else		// si existe deja:
	{
		client.addChannel(server._mapChannel[i->at(1)]);
		server._mapChannel[i->at(1)]->addMember(&client);
	}
	
						std::cout << "channel list: " << std::endl;
						for (std::map<std::string, Channel*>::iterator it = server._mapChannel.begin(); it != server._mapChannel.end(); it++)
						{
							std::cout << "- " << it->second->getName() << std::endl;
						}

	std::string response;
	response = ":IRCServ 0 " + client.getNick() + " : Join channel " + i->at(1) + ".\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
	//TODO: mettre le message join channel en opposition avec un msg "already joined" dans le else
}




void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	(void)client;
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(2))
		it++;
	if (it == server._mapClient.end())
		throw ("//TODO:NR : client doesn't exist.");

	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		throw ("//TODO:NR : channel doesn't exist.");		//pour l'instant (core dumped) a fix
	it->second->rmChannel(server._mapChannel[i->at(1)]);
	// client.rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(it->second);
	std::cout << "KICK" << std::endl;
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

