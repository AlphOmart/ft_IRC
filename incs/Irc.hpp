/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:33 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/29 14:56:56 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# define MAX_EVENTS 10
# include "Color.hpp"
# include <unistd.h>		// pour close
# include <sys/epoll.h>		// pour epoll
# include <iostream>		// pour std::cout
# include <cstring>
# include <cstdlib>			// pour EXIT_FAILURE et strtol
# include <arpa/inet.h>		// pour print laddr ip
# include <sstream>
# include <map>
# include <vector>
# include <queue>
# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"


// # include <string.h>		// pour memset
// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <iomanip>

// EPOLLIN = hey je tai envoye un truc 		client qui dit au serveur

// EPOLLOUT = hey je suis pret a recevoir des trucs.

#endif
