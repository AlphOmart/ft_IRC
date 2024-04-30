/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/30 12:59:29 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

void	fctPASS(std::string str, Server& server, Client& client)
{
	// if (client.getIsRegistered() == false)
	// {
		if (str == server.getPass())
		{
			std::cout << "PASS" << std::endl;
			client.setPass(true);
		}
		else
		{
			std::string response;
			response = ":IRCServ 464 *: Password incorrect\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);	//TODO : kick le client parce que probleme avec le terminal: repond 1 fois sur 2
			//server.kickClient(client.getFd());
		}
	// }
}

void	fctNICK(std::string str, Server& server, Client& client)
{
	if (server.nickAlreadyUsed(str) == false)
		client.setNickname(str);
	else
	{
		std::string response;
		response = ":IRCServ 433 *: Nickname is already used. \r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
	// if (client.getIsRegistered() == false)	//permet de ignorer quand déjà  connecté
	// {
		if (client.isRegistered() == true)
		{
			std::string response;
			response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + "@IRCServ\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
		}
	// }
}

void	fctUSER(std::string str, Server& server, Client& client)
{
	(void)server;
	if (client.getIsRegistered() == false)	//permet de ignorer la fonction quand connecté
	{
		client.setUser(str);
		if (client.isRegistered() == true)
		{
			std::string response;
			response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + " @IRCServ\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
		}
	}
}

void	fctJOIN(std::string str, Server& server, Client& client)
{
	if (server._mapChannel.find(str) == server._mapChannel.end())	//verifier si le channel n'existe pas
	{
		Channel *curChannel =  new Channel(str);
		server._mapChannel[curChannel->getName()] = curChannel;
		client.addChannel(curChannel);
		curChannel->addMember(&client);
	}
	else		// si existe deja:
	{
		client.addChannel(server._mapChannel[str]);
		server._mapChannel[str]->addMember(&client);
	}
	
						std::cout << "channel list: " << std::endl;
						for (std::map<std::string, Channel*>::iterator i = server._mapChannel.begin(); i != server._mapChannel.end(); i++)
						{
							std::cout << "- " << i->second->getName() << std::endl;
						}

	std::string response;
	response = ":IRCServ 0 " + client.getNick() + " : Join channel " + str + ".\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
	//TODO: mettre le message join channel en opposition avec un msg "already joined" dans le else
}

void	fctKICK()
{
	std::cout << "KICK" << std::endl;
}

void	fctINVITE()
{
	std::cout << "INVITE" << std::endl;
}

void	fctTOPIC()
{
	std::cout << "TOPIC" << std::endl;
}

