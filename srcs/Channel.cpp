/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:00:08 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/30 12:46:08 by tdutel           ###   ########.fr       */
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
	if (std::find(_members.begin(), _members.end(), newClient) == _members.end())	//ne l'ajoute pas si est déjà dedans
		_members.push_back(newClient);
}
