/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:49:48 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/14 12:58:01 by tdutel           ###   ########.fr       */
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
		std::map<std::string, Client *>	getMembers() const;
		std::size_t						getMemberSize();

		bool							getIsMdp();
		bool							getIsUserLimit();
		bool							getInvitOnly();
		bool							getTopicRestriction();


	// Client List //
		void							addMember(Client *newClient);
		void							addModerator(Client *newClient);
		void							addInvitMember(Client *newClient);

		void							rmMember(Client *client);
		void							rmModerator(Client *client);
		void							rmInvitMember(Client *client);

		bool							isMember(std::string client);
		bool							isModerator(std::string client);
		bool							isInvited(std::string client);

};

#endif
