/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:22:18 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/11 10:39:18 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"


void	fctINVITE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 3)
		{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
		}
	if (server._mapChannel.find(i->at(2)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(2) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(2)]->isMember(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(2) << " :You're not on that channel";
		printRPL(ERR_NOTONCHANNEL, str.str(), client, server);
		return ;
	}
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(1))
		it++;
	if (it == server._mapClient.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such nick/channel";
		printRPL(ERR_NOSUCHNICK, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(2)]->isMember(i->at(1)) == true)
	{
		str << client.getNick() << " " << i->at(1) << " " << i->at(2) << " :is already on channel";
		printRPL(ERR_USERONCHANNEL, str.str(), client, server);
		return ;
	}
	server._mapChannel[i->at(2)]->addInvitMember(it->second);
	str << client.getNick() << " " << i->at(1) << " " << i->at(2);
	printRPL(RPL_INVITING, str.str(), client, server);
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
