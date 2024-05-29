/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:40:00 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/29 17:52:29 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Irc.hpp"

void	printRPL(int nr, std::string str, Client &client, Server &server)	//affiche la numeric reply de l'erreur au client
{
	std::stringstream rpl;
	rpl << ":IRCserv " << nr << " " << str;
	std::string result = rpl.str() + "\r\n";
	client.setMailbox(result, server.getEpollfd());
	// if (send(client.getFd(), result.c_str(), result.size(), 0) == -1)
	// 	throw std::runtime_error("Error while sending.");
}

void	printMSG(std::string str, Client &client, Client &target, Server &server)
{
	std::string result = ":" + client.getNick() + " PRIVMSG " + target.getNick() + " :" + str + "\r\n";
	target.setMailbox(result, server.getEpollfd());
}

void	printChannel(std::string str, Client &client, Channel &chan, Server &server)
{
	std::string result = ":" + client.getNick() + " PRIVMSG " + chan.getName() + " :" + str + "\r\n";
	std::map<std::string, Client *> members = chan.getMembers();

	for (std::map<std::string, Client *>::iterator	it = members.begin(); it != members.end(); ++it)
	{
		if (it->first != client.getNick())
			it->second->setMailbox(result, server.getEpollfd());
	}
}













// void	printNotif


// void	printNR(int nr, )
