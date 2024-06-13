/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:26:23 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/13 17:45:55 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	JOIN0(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);

void	fctQUIT(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	std::cout << RED << "quit function called" << RESET << std::endl;

	JOIN0(i, server, client);
	
	client.clearChannel();	//maybe leaks for later

	str << client.getNick() << " is exiting the network with the message: Quit: " << i->at(1) << "\r\n";

	std::map<int, Client *>::iterator curClient = server._mapClient.find(client.getFd());
	for (std::map<int, Client *>::iterator it2 = server._mapClient.begin(); it2 != server._mapClient.end(); it2++)
	{
		if (it2 != curClient)
			it2->second->setMailbox(str.str(), server.getEpollfd());
	}
	str.str("");
	str.clear();
	str << "Error :" << i->at(1) << "\r\n";
	curClient->second->setMailbox(str.str(), server.getEpollfd());	// peut pas car si join # puis reconnect aborting
	close(client.getFd());		//necessaire pour provoquer le rdhup mais fais leaks
}
	// client.setDestroy(true);
//i->at(0) "QUIT"
//i->at(1) ":leaving"
