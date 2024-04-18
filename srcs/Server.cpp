/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:10:07 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/18 13:08:24 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

Server::Server(char *port) : _addrLen(sizeof(_server_addr))
{
	std::string str(port);
	if (std::string::npos != str.find_first_not_of("0123456789")) {
		throw	std::invalid_argument("Error : port is not valid.");
	}
	_port = std::strtol(port, NULL, 10);
	epollCreation();
	socketCreation();
	addrConfig();
	linkSocket();
	listenConnectIn();
	addSocketToEpoll();
	initCommand();
}

Server::~Server()
{
}


//############################# EPOLL FCT ##########################################################//


		// Création de l'instance epoll
void	Server::epollCreation()
{
	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1) {
		throw std::runtime_error("Error while calling epoll_create1().");
	}
}

		// Création du socket
void	Server::socketCreation()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		throw std::runtime_error("Error while creating socket.");
	}
	int option = 1;
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

}

		// Configuration de l'adresse du serveur
void	Server::addrConfig()
{
	memset(&_server_addr, 0, _addrLen);
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_addr.sin_port = htons(_port);
}

		// Liaison du socket à l'adresse du serveur
void	Server::linkSocket()
{
	if (bind(_server_fd, (struct sockaddr *)&_server_addr, _addrLen) == -1) {
		throw std::runtime_error("Error while binding socket.");
	}
}


		// Écoute des connexions entrantes
void	Server::listenConnectIn()
{
	if (listen(_server_fd, SOMAXCONN) == -1) {
		throw std::runtime_error("Error while listening.");
	}
}

		// Ajout du socket d'écoute à l'instance epoll
void	Server::addSocketToEpoll()
{
	_event.events = EPOLLIN | EPOLLRDHUP | EPOLLOUT; // Surveillage des événements de lecture
	_event.data.fd = _server_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _server_fd, &_event) == -1) {
		throw std::runtime_error("Error while calling epoll_ctl().");
	}
}

void	Server::epollWait()
{
	_nfds = epoll_wait(_epoll_fd, _events, MAX_EVENTS, -1);
		if (_nfds == -1) {
			throw std::runtime_error("Error while calling epoll_wait().");
		}
	for (int n = 0; n < _nfds; ++n)
		eventLoop(n);
}

void	Server::eventLoop(int	n)
{
	try
	{
		if (_events[n].events & EPOLLRDHUP)
			epollrdhupEvent(n);
		else if (_events[n].events & EPOLLIN)
			epollinEvent(n);
		else if (_events[n].events & EPOLLOUT)
			epolloutEvent(n);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}


void	Server::epollinEvent(int n)
{
	if (_events[n].data.fd == _server_fd)
	{
		// Nouvelle connexion entrée
		int connFd = accept(_server_fd, (struct sockaddr *)&_server_addr, &_addrLen);
		if (connFd == -1)
		{
			throw std::runtime_error("Error while calling accept().");
		}
		Client *acceptedClient = new Client(connFd);

		_mapClient[acceptedClient->getFd()] = acceptedClient;
		// Ajout du nouveau descripteur Client de fichier à l'instance epoll
		_mapClient[acceptedClient->getFd()]->updateStatus(_epoll_fd);
		// std::cout << "Nouvelle connexion de " << inet_ntoa(_server_addr.sin_addr) << std::endl;
		//TODO PARSE CLIENT
		std::stringstream buff;
		buff << "Nouvelle connexion de " << inet_ntoa(_server_addr.sin_addr) << "\r\n";
		std::map<int, Client *>::iterator curClient = _mapClient.find(connFd);
		for (std::map<int, Client *>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
		{
			if (it != curClient)
				it->second->setMailbox(buff.str(), _epoll_fd);	//ajout de l'input dans la mailbox
		}
	}
	else
	{
		char buff[1024] = {0};
		size_t br = recv(_events[n].data.fd, buff, sizeof(buff) - 1, 0);
		buff[br] = '\0';
		std::cout << "buff is " << buff << std::endl;
		std::vector<std::string>		input;
		input = splitStr(buff, ' ');
		if (_commandList.find(input[0]) != _commandList.end())
			(*_commandList[input[0]])();
		else
			std::cout << "unknown command : " << input[0] << std::endl;
		// Traitement des données entrantes sur une connexion existante
//		char buffer[1024] = {0};
//		ssize_t bytes_read = recv(_events[n].data.fd, buffer, sizeof(buffer) - 1, 0);
//		if (bytes_read > 0)
//		{
//			buffer[bytes_read] = '\0'; // Terminer la chaîne
//			switch (cmdCheck(buffer))
//			{
//			case 0:
//				std::cout << "PASS	[#channel_name]	[user]	[reason]." << std::endl;
//				break;
//			case 1:
//				std::cout << "KICK	[#channel_name]	[user]	[reason]." << std::endl;
//				break;
//
//			case 2:
//				std::cout << "INVITE	[user]	[#channel_name]." << std::endl;
//				break;
//
//			case 3:
//				std::cout << "TOPIC	[#channel_name]	[NewTopic]." << std::endl;
//				break;
//
//			case 4:
//				cmdMode(buffer);
//				break;
//
//			default:
//				std::map<int, Client *>::iterator curClient = _mapClient.find(_events[n].data.fd);
//				for (std::map<int, Client *>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
//				{
//					if (it != curClient)
//						it->second->setMailbox(buffer, _epoll_fd);	//ajout de l'input dans la mailbox
//				}
//			}
//			// std::cout << "Client "<< _events[n].data.fd <<" : " << buffer <<std::endl ;
//		}

	}
}

void	Server::epollrdhupEvent(int n)
{
	std::stringstream buff;
	buff << "Le client " << _events[n].data.fd << " s'est déconnecté.\r\n";
	
	std::map<int, Client *>::iterator curClient = _mapClient.find(_events[n].data.fd);
	for (std::map<int, Client *>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
	{
		if (it != curClient)
			it->second->setMailbox(buff.str(), _epoll_fd);	//ajout de l'input dans la mailbox
	}

	// Le client s'est déconnecté
	// Supprimer le descripteur de fichier de l'instance epoll si nécessaire
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _events[n].data.fd, &_event);
	close(_events[n].data.fd); // Fermer le descripteur de fichier du client déconnecté
	delete(_mapClient[_events[n].data.fd]);
	_mapClient.erase(_events[n].data.fd);
	// _mapClient[_events[n].data.fd]->updateStatus(_epoll_fd);
}

void	Server::epolloutEvent(int n)
{
	_mapClient[_events[n].data.fd]->receiveAll(_epoll_fd);
}

void	Server::closeFd()
{
	close(_server_fd);
	close(_epoll_fd);
}


void	Server::fctPASS()
{
	std::cout << "PASS" << std::endl;
}

// void	Server::fctPASS()
// {
// 	// if (_pass)
// 	std::cout << "PASS" << std::endl;
// }

// int	Server::cmdCheck(char *buffer)					// ANCIENNE FONCTION POUR LE SWITCH CASE
// {
// 	std::string str(buffer);
// 	if (str.find("PASS", 0) == 0)
// 		return (0);
// 	if (str.find("/KICK ",0) == 0)
// 		return (1);
// 	if (str.find("/INVITE ",0) == 0)
// 		return (2);
// 	if (str.find("/TOPIC ",0) == 0)
// 		return (3);
// 	if (str.find("/MODE ",0) == 0)
// 		return (4);
// 	return (-1);
// 	// KICK		INVITE		TOPIC		MODE
// }


// void	Server::cmdMode(char *buffer)					// FONCTION POUR PARSER LES COMMANDES D'APRÈS (MODE, KICK, etc...)
// {
// 	std::string	str(buffer);
// 	std::vector<std::string> command = splitStr(str, ' ');
// 	std::cout << "cmd.size() = " << command.size() << std::endl;
// 	if (command.size() < 3)
// 		throw std::invalid_argument("Missing argument");
// 	if (command.size() == 3)
// 	{
// 		if (command.at(2) == "-k" || command.at(2) == "-o" || command.at(2) == "-l")
// 			throw std::invalid_argument("Missing argument after the flag.");
// 		else if (command.at(2) != "-i" && command.at(2) != "-t")
// 			throw std::invalid_argument("Unknown MODE flag");
// 		else
// 		{
// 			std::cout << "command[0]:" << command.at(0) << ".\tcommand[1]:" << command.at(1) << ".\tcommand[2]:" << command.at(2) << "." << std::endl;
// 			//TODO function for MODE -i -t
// 		}
// 	}
// 	else if (command.size() == 4)
// 	{
// 		if (command.at(2) == "-i" || command.at(2) == "-t")
// 			throw std::invalid_argument("To much arguments.");
// 		else if (command.at(2) != "-k" && command.at(2) != "-o" && command.at(2) != "-l")
// 			throw std::invalid_argument("Unknown MODE flag");
// 		else
// 		{
// 			std::cout << "command[0]:" << command.at(0) << ".\tcommand[1]:" << command.at(1) << ".\tcommand[2]:" << command.at(2) << ".\tcommand[3]:" << command.at(3) << "." << std::endl;
// 			//TODO function for MODE -k -o -l
// 		}
// 	}
// }



//############################# UTILS ##########################################################//


std::string	Server::getPass()
{
	return (_pass);
}

void	Server::initCommand()
{
	_commandList["PASS"] = &Server::fctPASS();
	// cmdLst["NICK"] = &fctNICK();
	// cmdLst["USER"] = &fctUSER();
	// cmdLst["KICK"] = &fctKICK();
	// cmdLst["INVITE"] = &fctINVITE();
	// cmdLst["TOPIC"] = &fctTOPIC();
}

std::vector<std::string>	Server::splitStr(std::string str, char sep)
{
	std::vector<std::string> result;
	std::string word = "";
	std::string::iterator it;
	int	 nb = 0;

	// std::cout << "\"" << str << "\"" << std::endl;
	for (it = str.begin(); *it != '\r'; ++it)
	{	
		if (*it == sep)
		{
			// std::cout << "nb : " << nb << std::endl;
			if (!word.empty())
			{
				result.push_back(word);
				word = "";
			}
		}
		else
			word += *it;
		nb++;
	}

	// Ajoute le dernier mot si la chaîne ne se termine pas par le séparateur
	if (!word.empty()) {
	    result.push_back(word);
	}
	return result;
}

//	-i -t -k -o -l

























/*
//TODO: deconnecter le client si catch un throw dans le client ?
*/



// ---------------------------------------------------------------------//
//    Examples:	KICK

// KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

// KICK #Finnish John :Speaking English       ; Kick John from #Finnish using "Speaking English" as the reason (comment).

//  Numeric Replies:
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//            ERR_NOTONCHANNEL
// ---------------------------------------------------------------------//




// ---------------------------------------------------------------------//
// Examples:	INVITE

//    :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel #Dust

//    INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to #Twilight_zone

//  Numeric Replies:
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
//            ERR_CHANOPRIVSNEEDED
//            RPL_INVITING                    RPL_AWAY
// ---------------------------------------------------------------------//




// ---------------------------------------------------------------------//
//  Examples:	TOPIC

//    :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

//    TOPIC #test :another topic      ;set the topic on #test to "another topic".

//    TOPIC #test                     ; check the topic for #test.

//  Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED
// ---------------------------------------------------------------------//




// ---------------------------------------------------------------------//
//    Examples:	MODE

//            Use of Channel Modes:

// MODE #Finnish +im               ; Makes #Finnish channel moderated and 'invite-only'.

// MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on #Fins.

// MODE #42 +k oulu                ; Set the channel key to "oulu".

// MODE #eu-opers +l 10            ; Set the limit for the number of users on channel to 10.


//         Use of user Modes:

// :Angel MODE Angel +i            ; Message from Angel to make themselves invisible.

// MODE WiZ -o                     ; WiZ 'deopping' (removing operator
//                                 status).  The plain reverse of this
//                                 command ("MODE WiZ +o") must not be
//                                 allowed from users since would bypass
//                                 the OPER command.

// Numeric Replies:
//            ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
//            ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_KEYSET
//            RPL_BANLIST                     RPL_ENDOFBANLIST
//            ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

//            ERR_USERSDONTMATCH              RPL_UMODEIS
//            ERR_UMODEUNKNOWNFLAG
// ---------------------------------------------------------------------//
