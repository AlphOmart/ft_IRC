/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:58 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/19 12:59:42 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/Irc.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cerr << RED "Error\n" YELLOW "Usage: ./ircserv <port> <password>" RESET << std::endl, 1);
	try
	{

		Server Serv(argv[1], argv[2]);
		
		while (1)
			Serv.epollWait();
//		Serv.closeFd();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
		return (0);
}
