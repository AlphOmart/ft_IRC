/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:25:17 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/13 15:53:39 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctPART(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;

	if (i->size() < 3)	// HexChat ('part' '#general' ':reason')
	{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
	}
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isMember(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not on that channel";
		printRPL(ERR_NOTONCHANNEL, str.str(), client, server);
		return ;
	}

	client.rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(client);
	if (server._mapChannel[i->at(1)]->getMemberSize() == 0)
	{
		delete(server._mapChannel[i->at(1)]);
		server._mapChannel.erase(i->at(1));
		str << ":" << client.getNick() << "!" + client.getUser() + "@" << "IRCserv" << " PART " << i->at(1) << " " << i->at(2) << "\r\n";
	}
	else
	{
		str << ":" << client.getNick() << "!" + client.getUser() + "@" << "IRCserv" << " PART " << i->at(1) << " " << i->at(2) << "\r\n";
		std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
		for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); it++)
		{
				it->second->setMailbox(str.str(), server.getEpollfd());
		}
	}
	client.setMailbox(str.str(), server.getEpollfd());

}
