/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:49:48 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/12 11:24:49 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class Channel
{
	private:

	// attributs //
		std::string						_name;
		std::string						_topic;
		std::string						_mdp;
		int								_userLimit;

	// check //
		bool							_isMdp;
		bool							_isUserLimit;
		bool							_invitOnly;
		bool							_topicRestriction;

	// client list //
		std::map<std::string, Client *>	_members;
		std::map<std::string, Client *> _moderators;
		std::map<std::string, Client *>	_invitedMembers;

		std::map<std::string, bool>		_modes;

	public:

	// Constructor Destructor //
		Channel(const std::string& name);
		Channel(const std::string& name, Client& mod);

		~Channel();

	// getter setter //
		void							setName(std::string name);
		void							setTopic(std::string topic);
		void							setMdp(std::string	mdp);
		void							setUserLimit(int nb);
		void							setInvitOnly(bool i);
		void							setTopicRestriction(bool i);

		const std::string&				getName() const;
		const std::string&				getTopic() const;
		const std::string&				getMdp() const;
		const int&						getUserLimit() const;

		bool							getIsMdp();
		bool							getIsUserLimit();
		bool							getInvitOnly();
		bool							getTopicRestriction();

		std::map<std::string, Client *>	getMembers() const;
		int								getMemberSize();


	// Client List //
		void							addMember(Client *newClient);
		void							addModerator(Client *newClient);
		void							addInvitMember(Client *newClient);

		void							rmMember(Client &client);
		void							rmModerator(Client &client);
		void							rmInvitMember(Client &client);

		bool							isMember(std::string client);
		bool							isModerator(std::string client);
		bool							isInvited(std::string client);

		void							addMode(const std::string& modeName, bool isActive);
		std::string						getModes() const;
		std::string						getList() const;

};

#endif
