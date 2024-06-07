/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:23:18 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/07 15:24:07 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

static int	flagCheck(std::string	str)
{
	if (str.size() > 2)
		return (0);
	if (str.find("i",0) == 1)
		return (1);
	if (str.find("t",0) == 1)
		return (2);
	if (str.find("k",0) == 1)
		return (3);
	if (str.find("o",0) == 1)
		return (4);
	if (str.find("l",0) == 1)
		return (5);
	return (-1);
}

void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 2 /*&& i->size() != 3 && i->size() != 4*/)
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
	if (i->size() == 2)
	{
		str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " " << server._mapChannel[i->at(1)]->getModes();
		printRPL(RPL_CHANNELMODEIS, str.str(), client, server);
		return ; // :ircserver MODE #channel +m
	}
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ;
	}
	if (i->at(2).at(0) != '+' && i->at(2).at(0) != '-')
	{
		str << client.getNick() << " :Unknown MODE flag";
		printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
		return ;
	}
	if (i->at(2).at(0) == '+')
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(true);
			server._mapChannel[i->at(1)]->addMode("i", true);
			return ;
		case 2:
			server._mapChannel[i->at(1)]->setTopicRestriction(true);
			server._mapChannel[i->at(1)]->addMode("t", true);
			return ;
		case 3:
		{
			if (i->size() < 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			server._mapChannel[i->at(1)]->addMode("k", true);
			return ;
		}
		case 4:
		{
			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
			{
				str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
				printRPL(ERR_NOSUCHNICK, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->addModerator(it->second);
			server._mapChannel[i->at(1)]->addMode("o", true);
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

			return ;
		}
		case 5:
			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->setUserLimit(std::atoi(i->at(3).c_str()));
			server._mapChannel[i->at(1)]->addMode("l", true);
			return ;
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		}
	}
	else
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(false);
			server._mapChannel[i->at(1)]->addMode("i", false);
			return ;
		case 2:
			server._mapChannel[i->at(1)]->setTopicRestriction(false);
			server._mapChannel[i->at(1)]->addMode("t", false);
			return ;
		case 3:
			server._mapChannel[i->at(1)]->setMdp("");
			server._mapChannel[i->at(1)]->addMode("k", false);
			return ;
		case 4:
		{

			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
			{

				str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
				printRPL(ERR_NOSUCHNICK, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->rmModerator(it->second);
			server._mapChannel[i->at(1)]->addMode("o", false);
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
			return ;
		}
		case 5:
			server._mapChannel[i->at(1)]->setUserLimit(0);
			server._mapChannel[i->at(1)]->addMode("l", false);
			return ;
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		}
	}
}		//	"MODE #l\r\nWHO #l\r\n"

