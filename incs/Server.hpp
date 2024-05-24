/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:09:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/24 12:53:08 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Client;
class Server;
class Channel;
		void	fctPASS(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctUSER(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctNICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctJOIN(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctINVITE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctTOPIC(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
		
		void	printERR(int nr, std::string str, Client &client);

class Server
{

//############################# PRIVATE ##########################################################//
	private:
//----------------------------- ATTRIBUTS ----------------------------------------------------------//

		uint64_t 														_port;
		const std::string 												_servName;
		const std::string												_pass;
		int																_n;

	//----	EPOLL ATTRIBUTS	----//
		int 															_epoll_fd;
		int																_server_fd;
		struct sockaddr_in												_server_addr;
		socklen_t														_addrLen;
		struct epoll_event												_event;
		struct epoll_event												_events[MAX_EVENTS];
		int																_nfds;

	//----	MAP	----//
		std::map<std::string, void(*) (std::vector<std::vector<std::string> >::iterator, Server&, Client&)>	_commandList;
		// std::map<std::string, Channel*>									_mapChannel;


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
		void										initCommand();
		std::vector<std::string>					splitStr(char *str, std::string sep);
		std::vector< std::vector<std::string> >		splitVector(std::vector<std::string> &line, std::string sep);


//############################# PUBLIC ##########################################################//

	public:
		std::map<int, Client*>											_mapClient;
		std::map<std::string, Channel*>									_mapChannel;

		Server(char *port);
		Server(char *port, const std::string& pass);
		~Server();
		void										epollWait();

		std::string									getPass();
		bool										nickAlreadyUsed(const std::string& str);
};

#endif
