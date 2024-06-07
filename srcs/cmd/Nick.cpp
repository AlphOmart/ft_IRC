/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:19:05 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/07 15:20:05 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctNICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->at(1).find_first_of("., *?!") != std::string::npos || i->at(1).find_first_of("#:$~&@%+") == 0)
	{
		str << client.getNick() << " " << i->at(1) << " :Erroneus nickname";
		printRPL(ERR_ERRONEUSNICKNAME, str.str(), client, server);
		return ;
	}
	if (server.nickAlreadyUsed(i->at(1)) == false)
		client.setNickname(i->at(1));
	else
	{
		str << client.getNick() << " " << i->at(1) << " " << i->at(1) << " is already used.";
		printRPL(ERR_NICKNAMEINUSE, str.str(), client, server);
		return ;
	}
	if (client.getIsRegistered() == false && client.isRegistered() == true)	//permet de ignorer quand déjà  connecté
	{
		str << client.getNick() << " :Welcome to the " << "IRCServ" << " Network, " << client.getNick() << "[!" << client.getUser() << "@IRCServ]";
		printRPL(RPL_WELCOME, str.str(), client, server);


					str.str("");
					str.clear();
					str << client.getNick() << " :Your host is IRCServ, running version 1.0"; 
					printRPL(RPL_YOURHOST, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :This server was created 05/10/24"; 
					printRPL(RPL_CREATED, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " IRCServ 1.0 o itkol";
					printRPL(RPL_MYINFO, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " CHANMODES=,,tkl,i :are supported by this server";
					printRPL(RPL_MYINFO, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :There are " << server.getUserSize() << " users and " << 0 << " invisible on " << 0 << " servers";
					printRPL(RPL_LUSERCLIENT, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " 0 :operator(s) online";
					printRPL(RPL_LUSEROP, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :I have " << server.getUserSize() << " clients and " << 0 << " servers";
					printRPL(RPL_LUSERME, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :- IRCServ Message of the day - ";
					printRPL(RPL_MOTDSTART, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :Bienvenue sur mon serveur IRC";
					printRPL(RPL_MOTD, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :End of /MOTD command.";
					printRPL(RPL_ENDOFMOTD, str.str(), client, server);

		//	avertir autres clients.	//
		std::stringstream buff;
		buff << "Nouvelle connexion de " << client.getNick() << "\r\n";
		for (std::map<int, Client *>::iterator it = server._mapClient.begin(); it != server._mapClient.end(); it++)
		{
			if (it->second != &client)
				it->second->setMailbox(buff.str(), server.getEpollfd());	//ajout de l'input dans la mailbox
		}
	}
}
