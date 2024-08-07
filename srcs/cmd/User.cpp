/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:19:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/07/19 14:01:23 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctUSER(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (client.getIsRegistered() == false)	//permet de ignorer la fonction quand connecté
	{
		if (i->size() != 5)	// <username> 0 * <realname>
		{
			str << client.getNick() << i->at(1) << " :Not enough parameters";
			printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
			return ;
		}
		client.setUser(i->at(1));
		if (client.isRegistered() == true)
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
					printRPL(RPL_ISUPPORT, str.str(), client, server);

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
	else
	{
		str << client.getNick() << " :You may not reregister";
		printRPL(ERR_ALREADYREGISTERED, str.str(), client, server);
	}
}
