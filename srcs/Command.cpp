/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/18 15:30:57 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

void	fctPASS(std::string str, Server& server, Client& client)
{
	if (str == server.getPass())
		std::cout << "PASS" << std::endl;
	else
	{
		std::string response;
		response = ":IRCServ 464 Password incorrect\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}

void	fctNICK(std::string str, Server& server, Client& client)
{
	(void)server;
	(void)client;
		std::cout << "nickname : "<< str << std::endl;
}

void	fctUSER(std::string str, Server& server, Client& client)
{
	(void)server;
	(void)client;
	std::cout << "user : "<< str << std::endl;
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

