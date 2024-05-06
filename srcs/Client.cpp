/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:16 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/06 14:39:30 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client(int	fd) : _fd(fd), _isInEpoll(false)
{
	_isPass = false;
	_username = "";
	_nickname = "";
	_isRegistered = false;
	_clientEvent.data.fd = this->_fd;
	_clientEvent.events = EPOLLIN | EPOLLRDHUP;
}

Client::~Client()
{
}

int	Client::getFd()
{
	return (_fd);
}

void	Client::setMailbox(std::string str, int epoll_fd)
{
	_mailbox.push(str);
	updateStatus(epoll_fd);
}

void	Client::updateStatus(int epoll_fd)
{
	if (_isInEpoll == false)
	{
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _fd, &_clientEvent) == -1)
		{
			throw std::runtime_error("error in epoll_ctl");
		}
		_isInEpoll = true;
		return;
	}
		if (_mailbox.empty())
			_clientEvent.events = EPOLLIN | EPOLLRDHUP;
		else
			_clientEvent.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, _fd, &_clientEvent) == -1)
		{
			throw std::runtime_error("error in epoll_ctl ok");
		}
}

void	Client::receiveAll(int epoll_fd)
{
	while (!_mailbox.empty())
	{
		std::string toSend = this->_mailbox.front();
		if (send(_fd, toSend.c_str(), toSend.size(), 0) == -1)
			throw std::runtime_error("Error while sending.");
		_mailbox.pop();
	}
	updateStatus(epoll_fd);
}


//	--------------- SET ------------//

void	Client::setPass(const bool& i)
{
	this->_isPass = i;
}

void	Client::setNickname(const std::string& str)
{
	this->_nickname = str;
}

void	Client::setUser(const std::string& str)
{
	this->_username = str;
}


//	--------------- GET ------------//

const bool&	Client::getIspass(void) const
{
	return (this->_isPass);
}

const bool&	Client::getIsRegistered(void) const
{
	return (this->_isRegistered);
}

const std::string&	Client::getNick(void) const
{
	return (this->_nickname);
}

const std::string&	Client::getUser(void) const
{
	return (this->_username);
}

const bool&	Client::isRegistered(void)
{
	if (!_nickname.empty() && !_username.empty() && _isPass == true)
			_isRegistered = true;
	return (_isRegistered);
}

void	Client::addChannel(Channel *ch)
{
	if (_channels.find(ch->getName()) == _channels.end())	//ne l'ajoute pas si est déjà dedans
		_channels[ch->getName()] = ch;
}

void	Client::rmChannel(Channel *ch)
{
	if (_channels.find(ch->getName()) != _channels.end())
		_channels.erase(ch->getName());
}
