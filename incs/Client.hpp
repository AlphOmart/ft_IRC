/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:21 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/17 16:22:27 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Client
{
	private:
		std::string				_username;
		std::string				_password;
		std::string				_nickname;
		int						_fd;
		bool					_isInEpoll;
		std::queue<std::string>	_mailbox;
		struct epoll_event		_clientEvent;
		

	public:
		Client(int	connFd);
		~Client();
	
		int			getFd();
		void		setMailbox(std::string str, int epoll_fd);
		void		updateStatus(int epoll_fd);
		void		receiveAll(int epoll_fd);
		void		setClientInfo();
	
	class err : public std::exception
	{
		public:
			virtual const char* what(void) const throw()
			{
				return ("error.exception");
			}
	};
	
};

#endif
