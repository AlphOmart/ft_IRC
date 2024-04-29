/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:21 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/29 15:37:50 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Channel;
class Client
{
	private:
		std::string				_username;
		bool					_isPass;
		bool					_isRegistered;
		std::string				_nickname;
		int						_fd;
		bool					_isInEpoll;
		std::queue<std::string>	_mailbox;
		struct epoll_event		_clientEvent;
		std::vector<Channel*>	_channels;
		

	public:
		Client(int	connFd);
		~Client();
	
		int			getFd();
		const bool&	getIspass() const;
		const std::string&	getNick() const;
		const std::string&	getUser() const;
		const bool&	getIsRegistered() const;

		void		setMailbox(std::string str, int epoll_fd);
		void		updateStatus(int epoll_fd);
		void		receiveAll(int epoll_fd);

		// void		setClientInfo();

		void		setPass(const bool& i);
		void		setNickname(const std::string& str);
		void		setUser(const std::string& str);
		const bool&	isRegistered(void);
		void		addChannel(Channel *ch);

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
