/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:00:08 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/10 14:51:03 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel() : _name("default"), _isUserLimit(false), _isMdp(false), _invitOnly(false)
{
}


Channel::Channel(const std::string& name, Client& mod) : _name(name), _isUserLimit(false), _isMdp(false), _invitOnly(false)
{
	addModerator(&mod);
}

Channel::~Channel()
{
}

std::size_t	Channel::getMemberSize()
{
	std::size_t size = _members.size();
	return (size);
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


void	Channel::setUserLimit(int nb)
{
	_userLimit = nb;
	if (nb == 0)
		_isUserLimit = false;
	else
		_isUserLimit = true;
};

const int&	Channel::getUserLimit() const
{
	return (_userLimit);
};

void	Channel::setMdp(std::string	mdp)
{
	_mdp = mdp;
	if (mdp.empty())
		_isMdp = false;
	else
		_isMdp = true;
};

const std::string&	Channel::getMdp() const
{
	return (_mdp);
};

bool	Channel::getIsUserLimit()
{
	return (_isUserLimit);
};

bool	Channel::getIsMdp()
{
	return (_isMdp);
};

void	Channel::setInvitOnly(bool i)
{
	_invitOnly = i;
};

bool	Channel::getInvitOnly()
{
	return (_invitOnly);
};
