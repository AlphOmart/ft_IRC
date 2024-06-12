/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:26:23 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/12 11:25:15 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	JOIN0(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);

void	fctQUIT(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;

	JOIN0(i, server, client);
	
	client.clearChannel();	//maybe leaks for later
	for (std::map<std::string,Channel*>::iterator	it = server._mapChannel.begin(); it != server._mapChannel.end(); it++)
	{
		if (it->second->isMember(client.getNick()))
			it->second->rmMember(client);				// useless vu que ya JOIN0
	}

	str << client.getNick() << " is exiting the network with the message: Quit: " << i->at(1) << "\r\n";

	std::map<int, Client *>::iterator curClient = server._mapClient.find(client.getFd());
	for (std::map<int, Client *>::iterator it2 = server._mapClient.begin(); it2 != server._mapClient.end(); it2++)
	{
		if (it2 != curClient)
			it2->second->setMailbox(str.str(), server.getEpollfd());
	}
	client.setDestroy(true);
	curClient->second->setMailbox(str.str(), server.getEpollfd());	// peut pas car si join # puis reconnect aborting
}
//i->at(0) "QUIT"
//i->at(1) ":leaving"
