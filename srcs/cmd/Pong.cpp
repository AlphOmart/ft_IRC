/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:26:58 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/10 17:20:45 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	fctPONG(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::string	str;
	std::string tmp;
	size_t	c = 1;

	while (c < i->size())
	{
		tmp += i->at(c) + " ";
		c++;
	}
	tmp = tmp.substr(0, tmp.size() - 1);
	str = ":"+ client.getNick() + "!" + client.getUser() + "@IRCServ PONG " + tmp + "\r\n"; 
	client.setMailbox(str, server.getEpollfd());
}
