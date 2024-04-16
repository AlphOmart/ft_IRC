/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:16 by tdutel            #+#    #+#             */
/*   Updated: 2024/03/27 14:37:36 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client(int	fd) : _fd(fd), _isInEpoll(false)
{
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
