/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:10:07 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/10 17:18:44 by tdutel           ###   ########.fr       */
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
	for (int n = 0; n < _nfds; ++n)
		eventLoop(n);
}

void	Server::eventLoop(int	n)
{
	_n = n;
	{
		if (_events[n].events & EPOLLRDHUP)
			epollrdhupEvent(n);
		else if (_events[n].events & EPOLLIN)
			epollinEvent(n);
		else if (_events[n].events & EPOLLOUT)
			epolloutEvent(n);
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
		// std::stringstream buff;
		// buff << "Nouvelle connexion de " << inet_ntoa(_server_addr.sin_addr) << _mapClient[connFd]->getNick() << "\r\n";
		// std::map<int, Client *>::iterator curClient = _mapClient.find(connFd);
		// for (std::map<int, Client *>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
		// {
		// 	if (it != curClient)
		// 		it->second->setMailbox(buff.str(), _epoll_fd);	//ajout de l'input dans la mailbox
		// }
	}
	else /*if (_mapClient.find(_events[n].data.fd)->second->getIsRegistered() == false)	// PRE-CONNEXION : PASS USER NICK only*/
	{
		char buff[1024] = {0};
		size_t br = recv(_events[n].data.fd, buff, sizeof(buff) - 1, 0);
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
		for (std::vector<std::vector<std::string> >::iterator i = input.begin(); i < input.end(); ++i)
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
				return ;		//A VERIFIER : on veut minimum 2 arg : la commande (PASS,NICK,USER,...) et la valeur (mdp, tdutel, mwubneh,...)
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
	std::stringstream buff;
	// buff << "Le client " << _mapClient[_events[n].data.fd]->getNick() << " s'est déconnecté.\r\n";

	// std::map<int, Client *>::iterator curClient = _mapClient.find(_events[n].data.fd);
	// for (std::map<int, Client *>::iterator it = _mapClient.begin(); it != _mapClient.end(); it++)
	// {
	// 	if (it != curClient)
	// 		it->second->setMailbox(buff.str(), _epoll_fd);	//ajout de l'input dans la mailbox
	// }

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
	if (_mapClient[_events[n].data.fd]->getDestroy() == true)
	{
		delete(_mapClient[_events[n].data.fd]);
		_mapClient.erase(_events[n].data.fd);
	}
}

void	Server::closeFd()
{
	close(_server_fd);
	close(_epoll_fd);
}

// void	Server::kickClient(int fd)
// {
// 	std::map<int, Client*>::iterator it = _mapClient.find(fd);
// 	_mapClient.erase(it);
// 	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &_event) == -1) {
// 		throw std::runtime_error("Error while calling epoll_ctl().");
// 	}
// 	close(fd);
// 	std::cout << "c'est ciao le client " << fd << std::endl;
// }


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
	for (std::map<int, Client*>::const_iterator it = _mapClient.begin(); it!= _mapClient.end(); ++it)
		c++;
	return (c);
}
// std::string	Server::getUlist() const
// {
// 	std::stringstream str;
// 	for (std::map<int, Client*>::const_iterator it = _mapClient.begin(); it!= _mapClient.end(); ++it)
// 		str  << it->second->getNick() << " ";
// 	return (str.str());
// }

// std::string	Server::getClist() const
// {
// 	std::stringstream str;
// 	for (std::map<std::string, Channel*>::const_iterator it = _mapChannel.begin(); it!= _mapChannel.end(); ++it)
// 		str << it->second->getName() << " ";
// 	return (str.str());
// }

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
	for (std::map<int, Client*>::iterator it = _mapClient.begin(); it != _mapClient.end(); ++it)
	{
		if (it->second->getNick() == str)
			return (true);
	}
	return (false);
}



void	Server::clearMapClient()
{
	for (std::map<int, Client*>::iterator it = _mapClient.begin(); it != _mapClient.end(); ++it)
	{
		delete(it->second);
	}
}

void	Server::clearMapChannel()
{
	for (std::map<std::string, Channel*>::iterator it = _mapChannel.begin(); it != _mapChannel.end(); ++it)
	{
		delete(it->second);
	}
}
















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

// MODE #Finnish +i               ; Makes #Finnish channel 'invite-only'.

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
