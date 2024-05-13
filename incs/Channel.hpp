/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:49:48 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/13 14:19:44 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class Channel
{
	private:
		std::map<std::string, Client *>	_members;
		std::map<std::string, Client *> _moderators;
		std::string						_name;
		std::map<std::string, Client *>	_invitedMembers;


		int								_userLimit;
		std::string						_mdp;

		bool							_isUserLimit;
		bool							_isMdp;
		bool							_invitOnly;
		bool							_topicRestriction;

	public:
		Channel(const std::string& name);
		Channel(const std::string& name, Client& mod);

		~Channel();

		void				setName(std::string name);
		const std::string&	getName() const;
		std::size_t			getMemberSize();
		std::map<std::string, Client *>	getMembers() const;
		void				addMember(Client *newClient);
		void				rmMember(Client *client);
		void				addModerator(Client *newClient);
		void				rmModerator(Client *client);
		void				addInvitMember(Client *newClient);
		void				rmInvitMember(Client *client);

		bool				isMember(std::string client);
		bool				isModerator(std::string client);
		bool				isInvited(std::string client);


		void				setUserLimit(int nb);
		const int&			getUserLimit() const;
		void				setMdp(std::string	mdp);
		const std::string&	getMdp() const;

		bool				getIsUserLimit();
		bool				getIsMdp();
		void				setInvitOnly(bool i);
		bool				getInvitOnly();
		void				setTopicRestriction(bool i);
		bool				getTopicRestriction();
};


#endif
