/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:16:33 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/07 15:26:07 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	JOIN0(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::vector<std::string> str;
	std::string tmp;
	std::vector<std::vector<std::string> > j;

	(void)i;
	for (std::map<std::string,Channel*>::iterator	it = server._mapChannel.begin(); it != server._mapChannel.end(); ++it)
	{
		str.clear();
		tmp = "PART " + it->second->getName() + " leaving";
		str.push_back(tmp);
		j = splitVector(str, " ");
		if (it->second->isMember(client.getNick()))
		{
			fctPART(j.begin(), server, client);
			// it->second->rmMember(&client);
		}
	}
}

void	fctJOIN(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())	//si le channel n'existe pas
	{
		if (i->at(1).find('#') != 0)
		{
			if (i->at(1).find('0') == 0 && i->at(1).size() == 1)
			{
				JOIN0(i, server, client);
				return;
			}
			else
			{
				str << client.getNick() << " " << i->at(1) << " :No such channel";
				printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
				return ;
			}
		}
		Channel *curChannel =  new Channel(i->at(1), client);
		server._mapChannel[curChannel->getName()] = curChannel;
		client.addChannel(curChannel);
		curChannel->addMember(&client);
	}
	else		// si existe deja:
	{
		if (server._mapChannel[i->at(1)]->getInvitOnly() == true && server._mapChannel[i->at(1)]->isInvited(client.getNick()) == false)
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+i)";
			printRPL(ERR_INVITEONLYCHAN, str.str(), client, server);
			return ;
		}
		if (server._mapChannel[i->at(1)]->getIsMdp() == true)	//si le channel a un mdp et qu'on a passé un mdp
		{
			if ((i->size() != 3) || (i->size() == 3 && server._mapChannel[i->at(1)]->getMdp() != i->at(2)))
			{
				str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+k)";
				printRPL(ERR_BADCHANNELKEY, str.str(), client, server);
				return ;
			}
		}
		if (server._mapChannel[i->at(1)]->getIsUserLimit() == true && server._mapChannel[i->at(1)]->getMemberSize() >= server._mapChannel[i->at(1)]->getUserLimit())
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+l)";
			printRPL(ERR_CHANNELISFULL, str.str(), client, server);
			return ;
		}
		client.addChannel(server._mapChannel[i->at(1)]);
		server._mapChannel[i->at(1)]->addMember(&client);
	}
	std::string	server_msg = ":" + client.getNick() + "!" + client.getUser() + "@ircserv JOIN :" +  i->at(1) + "\r\n";
	client.setMailbox(server_msg, server.getEpollfd());

	str << client.getNick() << " " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getTopic();
	printRPL(RPL_TOPIC, str.str(), client, server);

	str.str("");
	str.clear();
	str << client.getNick() << " = " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getList();
	std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); ++it)
	{
		printRPL(RPL_NAMREPLY, str.str(), *it->second, server);
	}

	str.str("");
	str.clear();
	str << client.getNick() << " " << i->at(1) << " :End of /NAMES list";
	std::map<std::string, Client *> ptr2 = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it2 = ptr2.begin(); it2 != ptr2.end(); ++it2)
	{
		printRPL(RPL_ENDOFNAMES, str.str(), *it2->second, server);
	}		//todo nécessaire de l'envoyer a tous aussi je suppose ?


}
