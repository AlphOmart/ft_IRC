/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:00:08 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/27 14:22:35 by tdutel           ###   ########.fr       */
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


// ----- setter ----- //

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


// ----- getter ----- //

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

std::size_t	Channel::getMemberSize()
	{
		std::size_t size = _members.size();
		return (size);
	}


// ----- Client List ----- //

void	Channel::addMember(Client *newClient)
	{
		if (_members.find(newClient->getNick()) == _members.end())	//ne l'ajoute pas si est déjà dedans
			_members[newClient->getNick()] = newClient;
		// else
		// 	throw ("from client NR : channel already joined !");
	}

void	Channel::addModerator(Client *newClient)
	{
		if (_moderators.find(newClient->getNick()) == _moderators.end())	//ne l'ajoute pas si est déjà dedans
			_moderators[newClient->getNick()] = newClient;
		// else
		// 	throw ("NR : client already is moderator");
	}

void	Channel::addInvitMember(Client *newClient)
	{
		if (_invitedMembers.find(newClient->getNick()) == _invitedMembers.end())	//ne l'ajoute pas si est déjà dedans
			_invitedMembers[newClient->getNick()] = newClient;
		// else
		// 	throw ("from client NR : channel already joined !");
	}


void	Channel::rmMember(Client *client)
	{
		if (_members.find(client->getNick()) != _members.end())	//ne le remove pas si est déjà remove
			_members.erase(client->getNick());
		// else
		// 	throw ("NR : client is not in the channel");
	}

void	Channel::rmModerator(Client *client)
	{
		if (_moderators.find(client->getNick()) != _moderators.end())	//ne le remove pas si est déjà remove
			_moderators.erase(client->getNick());
		// else
		// 	throw ("NR : client is not moderator");
	}

void	Channel::rmInvitMember(Client *client)
	{
		if (_invitedMembers.find(client->getNick()) != _invitedMembers.end())	//ne le remove pas si est déjà remove
			_invitedMembers.erase(client->getNick());
		// else
		// 	throw ("NR : client is not invited");
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
