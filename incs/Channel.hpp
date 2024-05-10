/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:49:48 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/10 14:50:25 by tdutel           ###   ########.fr       */
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
		const std::string				_name;


		int								_userLimit;
		std::string						_mdp;

		bool							_isUserLimit;
		bool							_isMdp;
		bool							_invitOnly;

	public:
		Channel();
		Channel(const std::string& name, Client& mod);

		~Channel();

		std::size_t	getMemberSize();
		const std::string&	getName() const;
		void				addMember(Client *newClient);
		void				addModerator(Client *newClient);
		void				rmMember(Client *newClient);


		void				setUserLimit(int nb);
		const int&			getUserLimit() const;
		void				setMdp(std::string	mdp);
		const std::string&	getMdp() const;

		bool				getIsUserLimit();
		bool				getIsMdp();
		void				setInvitOnly(bool i);
		bool				getInvitOnly();
};


#endif
