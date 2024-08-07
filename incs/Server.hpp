/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:09:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/07/20 12:53:14 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

	void	fctPASS(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctUSER(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctNICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctJOIN(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctINVITE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctTOPIC(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctPRIVMSG(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctPART(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctQUIT(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	fctPONG(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);
	void	Disco(Server& server, Client& client);


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
		void										epolloutEvent(int n);
		void										closeFd();

	//----	UTILS	----//
		void										initCommand();


//############################# PUBLIC ##########################################################//

	public:
		std::map<int, Client*>											_mapClient;
		std::map<std::string, Channel*>									_mapChannel;

		Server(char *port);
		Server(char *port, const std::string& pass);
		~Server();
		void										epollWait();

		int											getEpollfd();
		std::string									getPass();
		int											getUserSize();
		bool										nickAlreadyUsed(const std::string& str);

		//----- Clear Functions -----//
		void										clearMapClient();
		void										clearMapChannel();
};

#endif
