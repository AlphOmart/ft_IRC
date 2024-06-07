/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:18:11 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/07 15:20:09 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctPASS(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (client.getIsRegistered() == false)
	{
		if (i->at(1) == server.getPass())
		{
			client.setPass(true);
		}
		else
		{
			str << "*" << " :Password incorrect";
			printRPL(ERR_PASSWDMISMATCH, str.str(), client, server);
		}
	}
	else
	{
		str << client.getNick() << " :You may not reregister";
		printRPL(ERR_ALREADYREGISTERED, str.str(), client, server);
	}
}
