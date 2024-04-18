/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:09:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/18 15:19:54 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Client;

		void										fctPASS(std::string str);
		void										fctNICK(std::string str);
		void										fctUSER(std::string str);
class Server
{

//############################# PRIVATE ##########################################################//
	private:
//----------------------------- ATTRIBUTS ----------------------------------------------------------//

		uint64_t 									_port;
		const std::string							_pass;

	//----	EPOLL ATTRIBUTS	----//
		int 										_epoll_fd;
		int											_server_fd;
		struct sockaddr_in							_server_addr;
		socklen_t									_addrLen;
		struct epoll_event							_event;
		struct epoll_event							_events[MAX_EVENTS];
		int											_nfds;

	//----	MAP	----//
		std::map<int, Client*>							_mapClient;
		std::map<std::string, void(*) (std::string)>	_commandList;


//----------------------------- FUNCTIONS ----------------------------------------------------------//

	//----	EPOLL FCT	----//
		void										epollCreation();
		void										socketCreation();
		void										addrConfig();
		void										linkSocket();
		void										listenConnectIn();
		void										addSocketToEpoll();
		void										eventLoop(int n);

		void										epollinEvent(int n);
		void										epollrdhupEvent(int n);
		void										epolloutEvent(int n);
		void										closeFd();

	//----	UTILS	----//
		std::string									getPass();
		void										initCommand();
		std::vector<std::string>					splitStr(char *str, char sep);


//############################# PUBLIC ##########################################################//

	public:
		Server(char *port);
		~Server();
		void										epollWait();

		//		FCT_COMMANDLIST		//
		// int							cmdCheck(char *buffer);
		// void						cmdMode(char *buffer);
		
};

#endif
