/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:40:00 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/11 11:10:59 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Irc.hpp"

void	printRPL(int nr, std::string str, Client &client, Server &server)	//affiche la numeric reply de l'erreur au client
{
	std::stringstream rpl;
	rpl << ":IRCserv " << nr << " " << str;
	std::string result = rpl.str() + "\r\n";
	client.setMailbox(result, server.getEpollfd());
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


std::vector<std::string> splitStr(const char *str, std::string sep)
{
    std::vector<std::string> result;
    std::string word = "";

    for (size_t i = 0; str[i] != '\0'; ++i) // Utilise '\0' pour la fin de la chaîne
    {   
        if (sep.find(str[i]) != std::string::npos) // Vérifie si le caractère est un séparateur
        {
            if (!word.empty())
            {
                result.push_back(word);
                word = "";
            }
        }
        else
            word += str[i];
    }

    // Ajoute le dernier mot si la chaîne ne se termine pas par un séparateur
    if (!word.empty()) {
        result.push_back(word);
    }
    return result;
}

std::vector< std::vector<std::string> > splitVector(std::vector<std::string> &line, std::string sep)
{
    std::vector< std::vector<std::string> > result;

    for (size_t i = 0; i < line.size(); ++i)
    {
        // Convertit chaque std::string en char* pour utiliser avec splitStr
        char *cstr = &line[i][0];
        std::vector<std::string> split = splitStr(cstr, sep);
        result.push_back(split);
    }

    return result;
}
