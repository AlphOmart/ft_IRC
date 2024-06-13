/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:24:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/13 15:49:30 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctPRIVMSG(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	std::string msg;
	if (i->at(2).at(0) == ':')
		msg = i->at(2).substr(1) + " ";
	else
		msg = i->at(2) + " ";
	std::size_t n = 3;
	while (n < i->size())
	{
		msg += i->at(n) + " ";
		n++;
	}
	if (i->size() < 3)
	{
		str << client.getNick() << " :No text to send";
		printRPL(ERR_NOTEXTTOSEND, str.str(), client, server);
		return ; 
	}
	if (i->at(1).at(0) != '#')	//si PRIVMSG a un autre client
	{
		std::map<int, Client*>::iterator it = server._mapClient.begin();
		while (it != server._mapClient.end() && it->second->getNick() != i->at(1))
			it++;
		if (it == server._mapClient.end())
		{
			str << client.getNick() << " " << i->at(1) << " :No such nick/channel";
			printRPL(ERR_NOSUCHNICK, str.str(), client, server);
			return ;
		}
		printMSG(msg, client, *it->second, server);
	}
	else		// channel
	{
		if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		{
			str << client.getNick() << " " << i->at(1) << " :No such nick/channel";
			printRPL(ERR_NOSUCHNICK, str.str(), client, server);
			return ;
		}
		if (server._mapChannel[i->at(1)]->isMember(client.getNick()) == false)
		{
			str << client.getNick() << " " << i->at(1) << " :Cannot send to channel";
			printRPL(ERR_CANNOTSENDTOCHAN, str.str(), client, server);
			return ;
		}

		printChannel(msg, client, *server._mapChannel[i->at(1)], server);
	}
}

// i->at(1) : target
// i->at(2+) : msg

// "PRIVMSG #ok :hola ?\r\n"


// ERR_NOSUCHNICK (401)
// ERR_CANNOTSENDTOCHAN (404)
// ERR_NOTEXTTOSEND (412)
