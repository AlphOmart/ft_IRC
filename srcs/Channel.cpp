/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:00:08 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/07 12:32:15 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel() : _name("default")
{
}


Channel::Channel(const std::string& name, Client& mod) : _name(name)
{
	addModerator(&mod);
}

Channel::~Channel()
{
}

const std::string&	Channel::getName(void) const
{
	return (this->_name);
}

void	Channel::addMember(Client *newClient)
{
	if (_members.find(newClient->getNick()) == _members.end())	//ne l'ajoute pas si est déjà dedans
		_members[newClient->getNick()] = newClient;
	else
		throw ("from client NR : channel already joined !");
}

void	Channel::addModerator(Client *newClient)
{
	if (_moderators.find(newClient->getNick()) == _moderators.end())	//ne l'ajoute pas si est déjà dedans
		_moderators[newClient->getNick()] = newClient;
	else
		throw ("NR : client already is moderator");
}

void	Channel::rmMember(Client *newClient)
{
	if (_members.find(newClient->getNick()) != _members.end())	//ne le remove pas si est déjà remove
		_members.erase(newClient->getNick());
	else
		throw ("NR : client is not in the channel");
}
