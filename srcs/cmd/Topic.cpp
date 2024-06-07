/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:22:39 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/07 15:22:53 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"


void	fctTOPIC(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 2)
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
	if (server._mapChannel[i->at(1)]->getTopicRestriction() == true && server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ;
	}
	if (i->size() == 2)
	{
		if (server._mapChannel[i->at(1)]->getTopic().empty())
		{
			str << client.getNick() << " " << i->at(1) << " :No topic is set";
			printRPL(RPL_NOTOPIC, str.str(), client, server);
			return ;
		}
		str << client.getNick() << " " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getTopic();
		printRPL(RPL_TOPIC, str.str(), client, server);
		return ;
	}
	std::string newtopic;
	newtopic = i->at(2).substr(1) + " ";
	std::size_t n = 3;
	while (n < i->size())
	{
		newtopic += i->at(n) + " ";
		n++;
	}
	// std::string newtopic = i->at(2).substr(1);
	server._mapChannel[i->at(1)]->setTopic(newtopic);
}

// ---------------------------------------------------------------------//
//  Examples:	TOPIC
// 							i->at(0) = TOPIC
// 							i->at(1) = #channel
// 							i->at(2) = :newTopic

//		TOPIC #test :Newtopic		-> change 	-> if size == 3
//		TOPIC #test					-> print	-> if size == 2

//    :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

//    TOPIC #test :another topic      ;set the topic on #test to "another topic".

//    TOPIC #test                     ; check the topic for #test.

//  Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED
// ---------------------------------------------------------------------//
