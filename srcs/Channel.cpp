/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:00:08 by tdutel            #+#    #+#             */
/*   Updated: 2024/07/20 12:54:00 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"


// ----- Constructor Destructor ----- //

Channel::Channel(const std::string& name) : _name(name), _isMdp(false), _isUserLimit(false), _invitOnly(false)
	{
	}

Channel::Channel(const std::string& name, Client& mod) : _name(name), _isMdp(false), _isUserLimit(false), _invitOnly(false)
	{
		addModerator(&mod);
	}

Channel::~Channel()
	{
	}


// ----- SET ----- //

void	Channel::setName(std::string name)
	{
		_name = name;
	}

void	Channel::setTopic(std::string topic)
	{
		_topic = topic;
	}

void	Channel::setMdp(std::string	mdp)
	{
		_mdp = mdp;
		if (mdp.empty())
			_isMdp = false;
		else
			_isMdp = true;
	}

void	Channel::setUserLimit(int nb)
	{
		_userLimit = nb;
		if (nb == 0)
			_isUserLimit = false;
		else
			_isUserLimit = true;
	}

void	Channel::setInvitOnly(bool i)
	{
		_invitOnly = i;
	}

void	Channel::setTopicRestriction(bool i)
	{
		_topicRestriction = i;
	}


// ----- GET ----- //

const std::string&	Channel::getName(void) const
	{
		return (this->_name);
	}

const std::string&	Channel::getTopic(void) const
	{
		return (this->_topic);
	}

const std::string&	Channel::getMdp() const
	{
		return (_mdp);
	}

const int&	Channel::getUserLimit() const
	{
		return (_userLimit);
	}


bool	Channel::getIsMdp()
	{
		return (_isMdp);
	};

bool	Channel::getIsUserLimit()
	{
		return (_isUserLimit);
	};

bool	Channel::getInvitOnly()
	{
		return (_invitOnly);
	};

bool	Channel::getTopicRestriction()
	{
		return (_topicRestriction);
	};


std::map<std::string, Client *>	Channel::getMembers() const
	{
		return (_members);
	}

int	Channel::getMemberSize()
	{
		std::size_t size = _members.size();
		return (static_cast<int>(size));
	}

int	Channel::getModSize()
	{
		std::size_t size = _moderators.size();
		return (static_cast<int>(size));
	}


// ----- Client List ----- //

bool	Channel::addMember(Client *newClient)
	{
		if (_members.find(newClient->getNick()) == _members.end())	//ne l'ajoute pas si est déjà dedans
		{
			_members[newClient->getNick()] = newClient;
			return (false);
		}
		return (true);
	}

void	Channel::addModerator(Client *newClient)
	{
		if (_moderators.find(newClient->getNick()) == _moderators.end())	//ne l'ajoute pas si est déjà dedans
			_moderators[newClient->getNick()] = newClient;
	}

void	Channel::addInvitMember(Client *newClient)
	{
		if (_invitedMembers.find(newClient->getNick()) == _invitedMembers.end())	//ne l'ajoute pas si est déjà dedans
			_invitedMembers[newClient->getNick()] = newClient;
	}


void	Channel::rmMember(Client &client)
	{
		if (_members.find(client.getNick()) != _members.end())	//ne le remove pas si est déjà remove
			_members.erase(client.getNick());
		rmModerator(client);		//si rm des membres alors aussi du moderator et invtmember
		rmInvitMember(client);
	}

void	Channel::rmModerator(Client &client)
	{
		if (_moderators.find(client.getNick()) != _moderators.end())	//ne le remove pas si est déjà remove
			_moderators.erase(client.getNick());
	}

void	Channel::rmInvitMember(Client &client)
	{
		if (_invitedMembers.find(client.getNick()) != _invitedMembers.end())	//ne le remove pas si est déjà remove
			_invitedMembers.erase(client.getNick());
	}


bool	Channel::isMember(std::string client)
	{
		if (_members.find(client) == _members.end())
			return (false);
		return (true);
	}

bool	Channel::isModerator(std::string client)
	{
		if (_moderators.find(client) == _moderators.end())
			return (false);
		return (true);
	}

bool	Channel::isInvited(std::string client)
	{
		if (_invitedMembers.find(client) == _invitedMembers.end())
			return (false);
		return (true);
	}

void 	Channel::addMode(const std::string& flag, bool isActive)
	{
		_modes[flag] = isActive;
	}

std::string	Channel::getModes() const
	{
		std::string str;
		for (std::map<std::string,bool>::const_iterator it = _modes.begin(); it != _modes.end(); it++)
		{
			if (it->second == true)
				str += "+" + it->first;
		}
		return (str);
	}

std::string	Channel::getList() const
{
	std::stringstream str;
	for (std::map<std::string, Client*>::const_iterator it = _moderators.begin(); it!= _moderators.end(); it++)
		str << "@" << it->second->getNick() << " ";
	for (std::map<std::string, Client*>::const_iterator it = _members.begin(); it!= _members.end(); it++)
	{
		if (_moderators.find(it->first) == _moderators.end())
				str << it->second->getNick() << " ";
	}
	return (str.str());
}
