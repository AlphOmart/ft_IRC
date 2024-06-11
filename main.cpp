/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:58 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/11 11:11:31 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/Irc.hpp"

void signal_handler(int signal_num) 
{
	(void)signal_num;
	throw (0);//std::runtime_error("program ended with ^C");
} 

int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cerr << RED "Error\n" YELLOW "Usage: ./ircserv <port> <password>" RESET << std::endl, 1);

	Server Serv(argv[1], argv[2]);

	try
	{
		signal(SIGINT, signal_handler);
		while (1)
		{
			try
			{
				Serv.epollWait();
			}
			catch(const std::out_of_range& e)
			{
				std::cerr << e.what() << std::endl;
			}
			catch(std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
	catch(int)
	{
		std::cerr  << std::endl << "program ended with ^C" << std::endl;
	}
	return (0);
}
