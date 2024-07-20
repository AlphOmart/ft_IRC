/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:23:18 by tdutel            #+#    #+#             */
/*   Updated: 2024/07/20 11:08:09 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

static void	FlagMode(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);

static int	flagCheck(char	c)
{
	if (c == 'i')
		return (1);
	if (c == 't')
		return (2);
	if (c == 'k')
		return (3);
	if (c == 'o')
		return (4);
	if (c == 'l')
		return (5);
	return (-1);
}

void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)	// :dan!~h@localhost	 MODE #foobar -bl+i
{																									//						  0		 1		i->at(2)
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
	if (i->size() == 2)
	{
		str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " " << server._mapChannel[i->at(1)]->getModes();
		printRPL(RPL_CHANNELMODEIS, str.str(), client, server);
		return ;
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
	FlagMode(i, server, client);
}


static void	FlagMode(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)//			str =	"-bl+i"
{
	std::stringstream str;
	char	f;
	size_t	k = 0;

	std::string	tmp;//		bl+i
	while (k + 1 < i->at(2).size())
	{
		f = i->at(2).at(k);
		tmp = i->at(2).substr(k + 1);
		for (size_t j = 0; j < tmp.find_first_of("-+") && j < tmp.length();)
		{
			switch (flagCheck(tmp.at(j)))
			{
				case 1:
				{
					if (f == '+')
					{
						server._mapChannel[i->at(1)]->setInvitOnly(true);
						server._mapChannel[i->at(1)]->addMode("i", true);
						break ;
					}
					else
					{
						server._mapChannel[i->at(1)]->setInvitOnly(false);
						server._mapChannel[i->at(1)]->addMode("i", false);
						break ;
					}
				}
				case 2:
				{
					if (f == '+')
					{
						server._mapChannel[i->at(1)]->setTopicRestriction(true);
						server._mapChannel[i->at(1)]->addMode("t", true);
						break ;
					}
					else
					{
						server._mapChannel[i->at(1)]->setTopicRestriction(false);
						server._mapChannel[i->at(1)]->addMode("t", false);
						break ;
					}
				}
				case 3:
				{
					if (f == '+')
					{
						if (i->size() < 4)
						{
							str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
							printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
							break ;
						}
						server._mapChannel[i->at(1)]->setMdp(i->at(3));
						server._mapChannel[i->at(1)]->addMode("k", true);
						break ;
					}
					else
					{
						server._mapChannel[i->at(1)]->setMdp("");
						server._mapChannel[i->at(1)]->addMode("k", false);
						break ;
					}
				}
				case 4:
				{
					if (f == '+')
					{
						if (i->size() != 4)
						{
							str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
							printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
							break ;
						}
						std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
						std::map<std::string, Client*>::iterator it = members.begin();
						while (it != members.end() && it->second->getNick() != i->at(3))
							it++;
						if (it == members.end())
						{
							str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
							printRPL(ERR_NOSUCHNICK, str.str(), client, server);
							break ;
						}
						server._mapChannel[i->at(1)]->addModerator(it->second);
						server._mapChannel[i->at(1)]->addMode("o", true);
						str.str("");
						str.clear();
						str << client.getNick() << " = " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getList();
						std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
						for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); it++)
						{
							printRPL(RPL_NAMREPLY, str.str(), *it->second, server);
						}
						str.str("");
						str.clear();
						str << client.getNick() << " " << i->at(1) << " :End of /NAMES list";
						std::map<std::string, Client *> ptr2 = server._mapChannel[i->at(1)]->getMembers();
						for (std::map<std::string, Client *>::iterator it2 = ptr2.begin(); it2 != ptr2.end(); it2++)
						{
							printRPL(RPL_ENDOFNAMES, str.str(), *it2->second, server);
						}
						break ;
					}
					else
					{
						if (i->size() != 4)
						{
							str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
							printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
							break ;
						}
						std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
						std::map<std::string, Client*>::iterator it = members.begin();
						while (it != members.end() && it->second->getNick() != i->at(3))
							it++;
						if (it == members.end())
						{
							str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
							printRPL(ERR_NOSUCHNICK, str.str(), client, server);
							break ;
						}
						server._mapChannel[i->at(1)]->rmModerator(*(it->second));
						server._mapChannel[i->at(1)]->addMode("o", false);
						str.str("");
						str.clear();
						str << client.getNick() << " = " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getList();
						std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
						for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); it++)
						{
							printRPL(RPL_NAMREPLY, str.str(), *it->second, server);
						}
					
						str.str("");
						str.clear();
						str << client.getNick() << " " << i->at(1) << " :End of /NAMES list";
						std::map<std::string, Client *> ptr2 = server._mapChannel[i->at(1)]->getMembers();
						for (std::map<std::string, Client *>::iterator it2 = ptr2.begin(); it2 != ptr2.end(); it2++)
						{
							printRPL(RPL_ENDOFNAMES, str.str(), *it2->second, server);
						}
						break ;
					}
				}
				case 5:
				{
					if (f == '+')
					{
						if (i->size() != 4)
						{
							str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
							printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
							break ;
						}
						server._mapChannel[i->at(1)]->setUserLimit(std::atoi(i->at(3).c_str()));
						server._mapChannel[i->at(1)]->addMode("l", true);
						break ;
					}
					else
					{
						server._mapChannel[i->at(1)]->setUserLimit(0);
						server._mapChannel[i->at(1)]->addMode("l", false);
						break ;
					}
				}
				default:
					str << client.getNick() << " :Unknown MODE flag";
					printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
					return ;
			}
			j++;
			k++;
		}
		k++;
	}
	str.str("");
	str.clear();
	str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " " << server._mapChannel[i->at(1)]->getModes();
	printChanRPL(RPL_CHANNELMODEIS, str.str(), *server._mapChannel[i->at(1)], server);
	return ;
}
