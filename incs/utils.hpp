/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:48:17 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/14 14:34:29 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "Irc.hpp"


void									printRPL(int nr, std::string str, Client &client, Server& server);
void									printMSG(std::string str, Client &client, Client &target, Server& server);
void									printChannel(std::string str, Client &client, Channel &chan, Server& server);
void									printChanRPL(int nr, std::string str, Channel &chan, Server &server);

std::vector<std::string>				splitStr(const char *str, std::string sep);
std::vector< std::vector<std::string> >	splitVector(std::vector<std::string> &line, std::string sep);

#endif
