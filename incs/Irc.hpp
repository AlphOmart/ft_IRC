/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:33 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/11 11:12:09 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# define MAX_EVENTS 10

# include <unistd.h>		// pour close
# include <sys/epoll.h>		// pour epoll
# include <iostream>		// pour std::cout
# include <cstring>			// pour memset
# include <cstdlib>			// pour EXIT_FAILURE et strtol
# include <arpa/inet.h>		// pour print laddr ip
# include <sstream>			// pour stringstream
# include <map>				// pour map
# include <vector>			// pour vector
# include <queue>			// pour queue
# include <algorithm>		// pour std::find
# include <csignal>			// pour signal

class Client;
class Server;
class Channel;

# include "Channel.hpp"
# include "Client.hpp"
# include "Color.hpp"
# include "Numeric_replies.hpp"
# include "Server.hpp"
# include "utils.hpp"


#endif

// EPOLLIN = hey je tai envoye un truc 		client qui dit au serveur
// EPOLLOUT = hey je suis pret a recevoir des trucs.
