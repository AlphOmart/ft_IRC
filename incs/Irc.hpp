/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:27:33 by tdutel            #+#    #+#             */
/*   Updated: 2024/03/27 14:33:22 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <unistd.h>		// pour close
# include <sys/epoll.h>		// pour epoll
# include <iostream>		// pour std::cout
#include <cstring>
# include <cstdlib>			// pour EXIT_FAILURE et strtol
# include <arpa/inet.h>		// pour print laddr ip
#include <sstream>

# include <map>
# include <vector>
# include <queue>

// # include <cstdio>			// pour perror()
// # include <string.h>		// pour memset
// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <iomanip>

// EPOLLIN = hey je tai envoye un truc 		client qui dit au serveur

// EPOLLOUT = hey je suis pret a recevoir des trucs.

#endif
