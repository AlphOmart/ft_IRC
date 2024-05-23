/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:40:00 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/23 16:21:52 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Irc.hpp"

void	printERR(int nr, std::string str, Client &client)	//affiche la numeric reply de l'erreur au client
{
	std::stringstream rpl;
	rpl << ": IRCserv " << nr << " :" << str;
	std::string result = rpl.str();
	send(client.getFd(), result.c_str(), result.size(), 0); //todo check pourquoi ne veut pas send ou en tout cas pas afficher sur hexchat
}
















// void	printNotif


// void	printNR(int nr, )
