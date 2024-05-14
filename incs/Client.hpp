/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:21 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/14 13:24:05 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Channel;
class Client
{
	private:

	// attributs //
		std::string						_nickname;
		std::string						_username;
		int								_fd;
		std::map<std::string, Channel*>	_channels;
		std::queue<std::string>			_mailbox;
		struct epoll_event				_clientEvent;
	
	// check //
		bool							_isPass;
		bool							_isRegistered;
		bool							_isInEpoll;


	public:

	// Constructor Destructor //
		Client(int	connFd);
		~Client();

	// getter setter //
		void				setNickname(const std::string& str);
		void				setUser(const std::string& str);
		void				setMailbox(std::string str, int epoll_fd);
		void				setPass(const bool& i);

		const std::string&	getNick() const;
		const std::string&	getUser() const;
		int					getFd();
		const bool&			getIspass() const;
		const bool&			getIsRegistered() const;

	// Channel List //
		void				addChannel(Channel *ch);
		void				rmChannel(Channel *ch);

	// utils //
		const bool&			isRegistered(void);
		void				updateStatus(int epoll_fd);
		void				receiveAll(int epoll_fd);

	// class err : public std::exception
	// {
	// 	public:
	// 		virtual const char* what(void) const throw()
	// 		{
	// 			return ("error.exception");
	// 		}
	// };
};

#endif
