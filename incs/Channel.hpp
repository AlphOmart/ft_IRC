/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:49:48 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/07 12:32:07 by tdutel           ###   ########.fr       */
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
	
	public:
		Channel();
		Channel(const std::string& name, Client& mod);

		~Channel();

		const std::string&	getName() const;
		void				addMember(Client *newClient);
		void				addModerator(Client *newClient);
		void				rmMember(Client *newClient);
};


#endif
