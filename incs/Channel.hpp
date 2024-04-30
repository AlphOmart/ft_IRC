/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:49:48 by tdutel            #+#    #+#             */
/*   Updated: 2024/04/30 12:25:18 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class Channel
{
	private:
		std::vector<Client *>	_members;
		const std::string		_name;
	
	public:
		Channel();
		Channel(const std::string &name);
		~Channel();

		const std::string&	getName() const;
		void	addMember(Client *newClient);
};


#endif
