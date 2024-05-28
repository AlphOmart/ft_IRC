/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:40:00 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/28 11:41:33 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Irc.hpp"

void	printERR(int nr, std::string str, Client &client)	//affiche la numeric reply de l'erreur au client
{
	std::stringstream rpl;
	rpl << ":IRCserv " << nr << " " << str;
	std::string result = rpl.str() + "\r\n";
	if (send(client.getFd(), result.c_str(), result.size(), 0) == -1)
		throw std::runtime_error("Error while sending.");
}
















// void	printNotif


// void	printNR(int nr, )
