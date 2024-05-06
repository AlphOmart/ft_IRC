/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:00:08 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/06 14:40:47 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel() : _name("default")
{
}

Channel::Channel(const std::string& name) : _name(name)
{
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
}

void	Channel::rmMember(Client *newClient)
{
	if (_members.find(newClient->getNick()) != _members.end())	//ne le remove pas si est déjà remove
		_members.erase(newClient->getNick());
}
