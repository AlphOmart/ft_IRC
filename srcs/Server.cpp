/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:10:07 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/14 15:03:11 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

Server::Server(char *port, const std::string& pass) :_servName("IRCServ"), _pass(pass), _addrLen(sizeof(_server_addr))
{
	try
	{
		std::string str(port);
		if (str.find_first_not_of("0123456789") != std::string::npos) {
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
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

Server::~Server()
{
	clearMapChannel();
	clearMapClient();
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
	for (int n = 0; n < _nfds; n++)
		eventLoop(n);
}

void	Server::eventLoop(int	n)
{
	_n = n;
	{
		if (_events[n].events & EPOLLOUT)
		{
			std::cout << BLUE << "out" <<RESET<<std::endl; 
			epolloutEvent(n);
		}
		if (_events[n].events & EPOLLIN)
		{
			std::cout << BLUE << "in" <<RESET<<std::endl; 
			epollinEvent(n);
		}
		// if (_events[n].events & EPOLLRDHUP)
		// {
		// 	std::cout << BLUE << "rdhup" <<RESET<<std::endl; 
		// 	epollrdhupEvent(n);
		// }

		// if (_mapClient.find(_events[n].data.fd) != _mapClient.end() && _mapClient[_events[n].data.fd]->getDestroy() == true)
		// {
		// 	epolloutEvent(n);
		// 	delete(_mapClient[_events[n].data.fd]);
		// 	_mapClient.erase(_events[n].data.fd);
		// 	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _events[n].data.fd, &_event);
		// 	close(_events[n].data.fd); // Fermer le descripteur de fichier du client déconnecté
		// }
		std::cout << "----------------------------------------------" << std::endl;
	}
}

// EN GROS : CA MARCHE MAIS IL FAUT BIEN ATTENDRE QUE TOUS LE DEBUG SE FASSE. SI RECONNECT TROP RAPIDEMENT ALORS CLIENT2 ET LOOP (dans epollin car return)
// CE QUI FAIT QUE : DEBUG MARCHE CAR LENT MAIS PAS EN NORMAL.

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
		for (std::map<int, Client*>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
		{
			std::cout << RED << "client : " << it->second->getNick() << "\n" << RESET ;
		}
	}
	else
	{
		char buff[1024] = {0};
		size_t br = recv(_events[n].data.fd, buff, sizeof(buff) - 1, 0);	//peut être 1024 - 1 plutôt//
		if (br < 1 || br > 1023)
		{
			// delete(_mapClient[_events[n].data.fd]);
			std::map<int, Client *>::iterator it = _mapClient.find(_events[n].data.fd);
			if (it == _mapClient.end())
				return;
			Disco(*this, *_mapClient[_events[n].data.fd]);
			epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _events[n].data.fd, &_event);
			delete(it->second);
			_mapClient.erase(it->first);
			close(_events[n].data.fd);
			return ;
		}
		buff[br] = '\0';
		std::cout << "[debug] <-" <<  buff << std::endl;
		std::string tmp = buff;
		if (tmp.find("\r\n") == std::string::npos)
		{
			_mapClient[_events[n].data.fd]->addBuffer(tmp);
			return ;
		}
		std::string tmp2 = _mapClient[_events[n].data.fd]->getBuffer() + tmp;
		_mapClient[_events[n].data.fd]->clearBuffer();
		std::vector<std::string>	line;
		line = splitStr(tmp2.c_str(), "\r\n");
		std::vector< std::vector<std::string> >	input;
		input = splitVector(line, " ");
		for (std::vector<std::vector<std::string> >::iterator i = input.begin(); i < input.end(); i++)
		{
			std::map<int, Client *>::iterator curClient = _mapClient.find(_events[n].data.fd);
			if (i->size() < 2 || curClient == _mapClient.end())
			{
				if (i->size() < 2)
				{
					std::stringstream str;
					str << curClient->second->getNick() << " " << i->at(0) << " :Not enough parameters";
					printRPL(ERR_NEEDMOREPARAMS, str.str(), *curClient->second, *this);
				}	
				return ;
			}
			if (_commandList.find(i->at(0)) != _commandList.end() && (curClient->second->getIspass() == true || i->at(0) == "PASS"))
			{
				if (curClient->second->isRegistered() == true || i->at(0) == "NICK" || i->at(0) == "USER" || i->at(0) == "PASS")
					(*_commandList[i->at(0)])(i, *this, *curClient->second);
				else
					{
						std::string response;
						response = "ERROR : Registration needed\r\n";
						if (send(curClient->second->getFd(), response.c_str(), response.length(), 0) == -1)
							throw std::runtime_error("Error while sending.");
					}
			}
			else if (_commandList.find(i->at(0)) != _commandList.end())
			{
				std::string response;
				response = "ERROR :Password needed\r\n";
				if (send(curClient->second->getFd(), response.c_str(), response.length(), 0) == -1)
					throw std::runtime_error("Error while sending.");
			}
		}
	}
}

void	Server::epollrdhupEvent(int n)
{
	(void)n;
	std::cout << RED << "Debug -> in rdhup event" << RESET<< std::endl;
	// delete(_mapClient[_events[n].data.fd]);
	// _mapClient.erase(_events[n].data.fd);

	// Supprimer le descripteur de fichier de l'instance epoll si nécessaire
	// epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _events[n].data.fd, &_event);
	// close(_events[n].data.fd); // Fermer le descripteur de fichier du client déconnecté
}

void	Server::epolloutEvent(int n)
{
	_mapClient[_events[n].data.fd]->receiveAll(_epoll_fd);
	if (_mapClient[_events[n].data.fd]->getDestroy() == true)
	{
		std::map<int, Client *>::iterator it = _mapClient.find(_events[n].data.fd);
		if (it == _mapClient.end())
			return;
		delete(it->second);
		epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _events[n].data.fd, &_event);
		_mapClient.erase(it->first);
		close(_events[n].data.fd);
	}
}

void	Server::closeFd()
{
	close(_server_fd);
	close(_epoll_fd);
}


//############################# UTILS ##########################################################//

int		Server::getEpollfd()
{
	return (_epoll_fd);
}

std::string	Server::getPass()
{
	return (_pass);
}

int	Server::getUserSize()
{
	int	c = 0;
	for (std::map<int, Client*>::const_iterator it = _mapClient.begin(); it!= _mapClient.end(); it++)
		c++;
	return (c);
}

void	Server::initCommand()
{
	_commandList["PASS"] = &fctPASS;
	_commandList["NICK"] = &fctNICK;
	_commandList["USER"] = &fctUSER;
	_commandList["JOIN"] = &fctJOIN;
	_commandList["KICK"] = &fctKICK;
	_commandList["INVITE"] = &fctINVITE;
	_commandList["MODE"] = &fctMODE;
	_commandList["TOPIC"] = &fctTOPIC;
	_commandList["PRIVMSG"] = &fctPRIVMSG;
	_commandList["PART"] = &fctPART;
	_commandList["QUIT"] = &fctQUIT;
	_commandList["PING"] = &fctPONG;
}


bool	Server::nickAlreadyUsed(const std::string& str)
{
	for (std::map<int, Client*>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
	{
		if (it->second->getNick() == str)
			return (true);
	}
	return (false);
}


//----- Clear Functions -----//

void	Server::clearMapClient()
{
	for (std::map<int, Client*>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
	{
		delete(it->second);
	}
}

void	Server::clearMapChannel()
{
	for (std::map<std::string, Channel*>::iterator it = _mapChannel.begin(); it != _mapChannel.end(); it++)
	{
		delete(it->second);
	}
}

// 	std::cout << "c'est ciao le client " << fd << std::endl;
