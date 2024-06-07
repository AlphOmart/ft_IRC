/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:21:10 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/07 15:21:22 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 3)	// HexChat ('kick' '#general' 'tim' ':reason')
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
	std::string com;
	if (i->size() > 3)
		com = i->at(3).substr(1);		//	pour gérer ":" devant motif Hexchat
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(2))
		it++;
	if (it == server._mapClient.end())
	{
		str << client.getNick() << " " << i->at(2) << " " << i->at(1) << " :They aren't on that channel";
		printRPL(ERR_USERNOTINCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isMember(it->second->getNick()) == false)
	{
		str << client.getNick() << " " << i->at(2) << " " << i->at(1) << " :They aren't on that channel";
		printRPL(ERR_USERNOTINCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ;
	}
	it->second->rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(it->second);
	if (server._mapChannel[i->at(1)]->isModerator(it->second->getNick()) == true)
		server._mapChannel[i->at(1)]->rmModerator(it->second);
	if (server._mapChannel[i->at(1)]->isInvited(it->second->getNick()) == true)
		server._mapChannel[i->at(1)]->rmInvitMember(it->second);
	
	str << ":" << client.getNick() << "!" + client.getUser() + "@" << "IRCserv" << " KICK " << i->at(1) << " " << i->at(2) << " " << com << "\r\n";
	
	std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it2 = ptr.begin(); it2 != ptr.end(); ++it2)
	{
			it2->second->setMailbox(str.str(), server.getEpollfd());
	}
	it->second->setMailbox(str.str(), server.getEpollfd());
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
