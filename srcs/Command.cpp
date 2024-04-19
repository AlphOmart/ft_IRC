/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/19 15:09:44 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

void	fctPASS(std::string str, Server& server, Client& client)
{
	if (str == server.getPass())
	{
		std::cout << "PASS" << std::endl;
		client.setPass(true);
	}
	else
	{
		std::string response;
		response = ":IRCServ 464 *: Password incorrect: Server will close connection ! \r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);	//TODO : kick le client parce que probleme avec le terminal: repond 1 fois sur 2
		server.kickClient(client.getFd());
	}	
		//je soupsonne le protocole irc de bloquer la commande suivante quand bad password car si pass -> ok alors marche comme il faut
		// mais si erreur alors ligne d'apres useless
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
	if (client.isRegistered() == true)
	{
		std::string response;
		response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + "@IRCServ\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}

void	fctUSER(std::string str, Server& server, Client& client)
{
	(void)server;
	if (client.getIsRegistered() == false)
		client.setUser(str);
	if (client.isRegistered() == true)
	{
		std::string response;
		response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + " @IRCServ\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
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

