/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/06/06 16:09:18 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

int		flagCheck(std::string	str);
void	JOIN0(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client);

void	fctPASS(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (client.getIsRegistered() == false)
	{
		if (i->at(1) == server.getPass())
		{
			client.setPass(true);
		}
		else
		{
			str << "*" << " :Password incorrect";
			printRPL(ERR_PASSWDMISMATCH, str.str(), client, server);
		}
	}
	else
	{
		str << client.getNick() << " :You may not reregister";
		printRPL(ERR_ALREADYREGISTERED, str.str(), client, server);
	}
}

void	fctNICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->at(1).find_first_of("., *?!") != std::string::npos || i->at(1).find_first_of("#:$~&@%+") == 0)
	{
		str << client.getNick() << " " << i->at(1) << " :Erroneus nickname";
		printRPL(ERR_ERRONEUSNICKNAME, str.str(), client, server);
		return ;
	}
	if (server.nickAlreadyUsed(i->at(1)) == false)
		client.setNickname(i->at(1));
	else
	{
		str << client.getNick() << " " << i->at(1) << " " << i->at(1) << " is already used.";
		printRPL(ERR_NICKNAMEINUSE, str.str(), client, server);
		return ;
	}
	if (client.getIsRegistered() == false && client.isRegistered() == true)	//permet de ignorer quand déjà  connecté
	{
		str << client.getNick() << " :Welcome to the " << "IRCServ" << " Network, " << client.getNick() << "[!" << client.getUser() << "@IRCServ]";
		printRPL(RPL_WELCOME, str.str(), client, server);


					str.str("");
					str.clear();
					str << client.getNick() << " :Your host is IRCServ, running version 1.0"; 
					printRPL(RPL_YOURHOST, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :This server was created 05/10/24"; 
					printRPL(RPL_CREATED, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " IRCServ 1.0 o itkol";
					printRPL(RPL_MYINFO, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " CHANMODES=,,tkl,i :are supported by this server";
					printRPL(RPL_MYINFO, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :There are " << server.getUserSize() << " users and " << 0 << " invisible on " << 0 << " servers";
					printRPL(RPL_LUSERCLIENT, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " 0 :operator(s) online";
					printRPL(RPL_LUSEROP, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :I have " << server.getUserSize() << " clients and " << 0 << " servers";
					printRPL(RPL_LUSERME, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :- IRCServ Message of the day - ";
					printRPL(RPL_MOTDSTART, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :Bienvenue sur mon serveur IRC";
					printRPL(RPL_MOTD, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :End of /MOTD command.";
					printRPL(RPL_ENDOFMOTD, str.str(), client, server);

		//	avertir autres clients.	//
		std::stringstream buff;
		buff << "Nouvelle connexion de " << client.getNick() << "\r\n";
		for (std::map<int, Client *>::iterator it = server._mapClient.begin(); it != server._mapClient.end(); it++)
		{
			if (it->second != &client)
				it->second->setMailbox(buff.str(), server.getEpollfd());	//ajout de l'input dans la mailbox
		}
	}
}

void	fctUSER(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (client.getIsRegistered() == false)	//permet de ignorer la fonction quand connecté
	{
		client.setUser(i->at(1));
		if (client.isRegistered() == true)
		{
			str << client.getNick() << " :Welcome to the " << "IRCServ" << " Network, " << client.getNick() << "[!" << client.getUser() << "@IRCServ]";
			printRPL(RPL_WELCOME, str.str(), client, server);
		
					str.str("");
					str.clear();
					str << client.getNick() << " :Your host is IRCServ, running version 1.0"; 
					printRPL(RPL_YOURHOST, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :This server was created 05/10/24"; 
					printRPL(RPL_CREATED, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " IRCServ 1.0 o itkol";
					printRPL(RPL_MYINFO, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " CHANMODES=,,tkl,i :are supported by this server";
					printRPL(RPL_MYINFO, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :There are " << server.getUserSize() << " users and " << 0 << " invisible on " << 0 << " servers";
					printRPL(RPL_LUSERCLIENT, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " 0 :operator(s) online";
					printRPL(RPL_LUSEROP, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :I have " << server.getUserSize() << " clients and " << 0 << " servers";
					printRPL(RPL_LUSERME, str.str(), client, server);

					str.str("");
					str.clear();
					str << client.getNick() << " :- IRCServ Message of the day - ";
					printRPL(RPL_MOTDSTART, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :Bienvenue sur mon serveur IRC";
					printRPL(RPL_MOTD, str.str(), client, server);
					str.str("");
					str.clear();
					str << client.getNick() << " :End of /MOTD command.";
					printRPL(RPL_ENDOFMOTD, str.str(), client, server);

				//	avertir autres clients.	//
				std::stringstream buff;
				buff << "Nouvelle connexion de " << client.getNick() << "\r\n";
				for (std::map<int, Client *>::iterator it = server._mapClient.begin(); it != server._mapClient.end(); it++)
				{
					if (it->second != &client)
						it->second->setMailbox(buff.str(), server.getEpollfd());	//ajout de l'input dans la mailbox
				}
		}
	}
	else
	{
		str << client.getNick() << " :You may not reregister";
		printRPL(ERR_ALREADYREGISTERED, str.str(), client, server);
	}
}

void	fctJOIN(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())	//si le channel n'existe pas
	{
		if (i->at(1).find('#') != 0)
		{
			if (i->at(1).find('0') == 0 && i->at(1).size() == 1)
			{
				JOIN0(i, server, client);
				return;
			}
			else
			{
				str << client.getNick() << " " << i->at(1) << " :No such channel";
				printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
				return ;
			}
		}
		Channel *curChannel =  new Channel(i->at(1), client);
		server._mapChannel[curChannel->getName()] = curChannel;
		client.addChannel(curChannel);
		curChannel->addMember(&client);
	}
	else		// si existe deja:
	{
		if (server._mapChannel[i->at(1)]->getInvitOnly() == true && server._mapChannel[i->at(1)]->isInvited(client.getNick()) == false)
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+i)";
			printRPL(ERR_INVITEONLYCHAN, str.str(), client, server);
			return ;
		}
		if (server._mapChannel[i->at(1)]->getIsMdp() == true)	//si le channel a un mdp et qu'on a passé un mdp
		{
			if ((i->size() != 3) || (i->size() == 3 && server._mapChannel[i->at(1)]->getMdp() != i->at(2)))
			{
				str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+k)";
				printRPL(ERR_BADCHANNELKEY, str.str(), client, server);
				return ;
			}
		}
		if (server._mapChannel[i->at(1)]->getIsUserLimit() == true && server._mapChannel[i->at(1)]->getMemberSize() >= server._mapChannel[i->at(1)]->getUserLimit())
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+l)";
			printRPL(ERR_CHANNELISFULL, str.str(), client, server);
			return ;
		}
		client.addChannel(server._mapChannel[i->at(1)]);
		server._mapChannel[i->at(1)]->addMember(&client);
	}
	std::string	server_msg = ":" + client.getNick() + "!" + client.getUser() + "@ircserv JOIN :" +  i->at(1) + "\r\n";
	client.setMailbox(server_msg, server.getEpollfd());

	str << client.getNick() << " " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getTopic();
	printRPL(RPL_TOPIC, str.str(), client, server);

	str.str("");
	str.clear();
	str << client.getNick() << " = " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getList();
	std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); ++it)
	{
		printRPL(RPL_NAMREPLY, str.str(), *it->second, server);
	}

	str.str("");
	str.clear();
	str << client.getNick() << " " << i->at(1) << " :End of /NAMES list";
	std::map<std::string, Client *> ptr2 = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it2 = ptr2.begin(); it2 != ptr2.end(); ++it2)
	{
		printRPL(RPL_ENDOFNAMES, str.str(), *it2->second, server);
	}		//todo nécessaire de l'envoyer a tous aussi je suppose ?


}

// : ircserv NR : 



void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 3)	// HexChat ('kick' '#general' 'tim' ':reason')
	{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
	}
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	std::string com;
	if (i->size() > 3)
		com = i->at(3).substr(1);		//	pour gérer ":" devant motif Hexchat
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(2))
		it++;
	if (it == server._mapClient.end())
	{
		str << client.getNick() << " " << i->at(2) << " " << i->at(1) << " :They aren't on that channel";
		printRPL(ERR_USERNOTINCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isMember(it->second->getNick()) == false)
	{
		str << client.getNick() << " " << i->at(2) << " " << i->at(1) << " :They aren't on that channel";
		printRPL(ERR_USERNOTINCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ;
	}
	it->second->rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(it->second);
	if (server._mapChannel[i->at(1)]->isModerator(it->second->getNick()) == true)
		server._mapChannel[i->at(1)]->rmModerator(it->second);
	if (server._mapChannel[i->at(1)]->isInvited(it->second->getNick()) == true)
		server._mapChannel[i->at(1)]->rmInvitMember(it->second);
	
	str << ":" << client.getNick() << "!" + client.getUser() + "@" << "IRCserv" << " KICK " << i->at(1) << " " << i->at(2) << " " << com << "\r\n";
	
	std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it2 = ptr.begin(); it2 != ptr.end(); ++it2)
	{
			it2->second->setMailbox(str.str(), server.getEpollfd());
	}
	it->second->setMailbox(str.str(), server.getEpollfd());
}

// ---------------------------------------------------------------------//
//    Examples:	KICK

// KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

// KICK #Finnish John :Speaking English       ; Kick John from #Finnish using "Speaking English" as the reason (comment).

//  Numeric Replies:
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//            ERR_NOTONCHANNEL
// ---------------------------------------------------------------------//


void	fctINVITE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 3)
		{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
		}
	if (server._mapChannel.find(i->at(2)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(2) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(2)]->isMember(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(2) << " :You're not on that channel";
		printRPL(ERR_NOTONCHANNEL, str.str(), client, server);
		return ;
	}
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(1))
		it++;
	if (it == server._mapClient.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such nick/channel";
		printRPL(ERR_NOSUCHNICK, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(2)]->isMember(i->at(1)) == true)
	{
		str << client.getNick() << " " << i->at(1) << " " << i->at(2) << " :is already on channel";
		printRPL(ERR_USERONCHANNEL, str.str(), client, server);
		return ;
	}
	server._mapChannel[i->at(2)]->addInvitMember(it->second);
	str << client.getNick() << " " << i->at(1) << " " << i->at(2);
	printRPL(RPL_INVITING, str.str(), client, server);
}

// i->at(1) = user
// i->at(2) = channel

// ---------------------------------------------------------------------//
// Examples:	INVITE

//    :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel #Dust

//    INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to #Twilight_zone

//  Numeric Replies:
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
//            ERR_CHANOPRIVSNEEDED
//            RPL_INVITING                    RPL_AWAY
// ---------------------------------------------------------------------//


void	fctTOPIC(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 2)
	{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
	}
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isMember(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not on that channel";
		printRPL(ERR_NOTONCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->getTopicRestriction() == true && server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ;
	}
	if (i->size() == 2)
	{
		if (server._mapChannel[i->at(1)]->getTopic().empty())
		{
			str << client.getNick() << " " << i->at(1) << " :No topic is set";
			printRPL(RPL_NOTOPIC, str.str(), client, server);
			return ;
		}
		str << client.getNick() << " " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getTopic();
		printRPL(RPL_TOPIC, str.str(), client, server);
		return ;
	}
	std::string newtopic;
	newtopic = i->at(2).substr(1) + " ";
	std::size_t n = 3;
	while (n < i->size())
	{
		newtopic += i->at(n) + " ";
		n++;
	}
	// std::string newtopic = i->at(2).substr(1);
	server._mapChannel[i->at(1)]->setTopic(newtopic);
}

// ---------------------------------------------------------------------//
//  Examples:	TOPIC
// 							i->at(0) = TOPIC
// 							i->at(1) = #channel
// 							i->at(2) = :newTopic

//		TOPIC #test :Newtopic		-> change 	-> if size == 3
//		TOPIC #test					-> print	-> if size == 2

//    :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

//    TOPIC #test :another topic      ;set the topic on #test to "another topic".

//    TOPIC #test                     ; check the topic for #test.

//  Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED
// ---------------------------------------------------------------------//



void	fctMODE(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() < 2 /*&& i->size() != 3 && i->size() != 4*/)
	{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
	}
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	if (i->size() == 2)
	{
		str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " " << server._mapChannel[i->at(1)]->getModes();
		printRPL(RPL_CHANNELMODEIS, str.str(), client, server);
		return ; // :ircserver MODE #channel +m
	}
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ;
	}
	if (i->at(2).at(0) != '+' && i->at(2).at(0) != '-')
	{
		str << client.getNick() << " :Unknown MODE flag";
		printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
		return ;
	}
	if (i->at(2).at(0) == '+')
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(true);
			server._mapChannel[i->at(1)]->addMode("i", true);
			return ;
		case 2:
			server._mapChannel[i->at(1)]->setTopicRestriction(true);
			server._mapChannel[i->at(1)]->addMode("t", true);
			return ;
		case 3:
		{
			if (i->size() < 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			server._mapChannel[i->at(1)]->addMode("k", true);
			return ;
		}
		case 4:
		{
			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
			{
				str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
				printRPL(ERR_NOSUCHNICK, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->addModerator(it->second);
			server._mapChannel[i->at(1)]->addMode("o", true);
							str.str("");
							str.clear();
							str << client.getNick() << " = " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getList();
							std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
							for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); ++it)
							{
								printRPL(RPL_NAMREPLY, str.str(), *it->second, server);
							}
						
							str.str("");
							str.clear();
							str << client.getNick() << " " << i->at(1) << " :End of /NAMES list";
							std::map<std::string, Client *> ptr2 = server._mapChannel[i->at(1)]->getMembers();
							for (std::map<std::string, Client *>::iterator it2 = ptr2.begin(); it2 != ptr2.end(); ++it2)
							{
								printRPL(RPL_ENDOFNAMES, str.str(), *it2->second, server);
							}		//todo nécessaire de l'envoyer a tous aussi je suppose ?

			return ;
		}
		case 5:
			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->setUserLimit(std::atoi(i->at(3).c_str()));
			server._mapChannel[i->at(1)]->addMode("l", true);
			return ;
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		}
	}
	else
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(false);
			server._mapChannel[i->at(1)]->addMode("i", false);
			return ;
		case 2:
			server._mapChannel[i->at(1)]->setTopicRestriction(false);
			server._mapChannel[i->at(1)]->addMode("t", false);
			return ;
		case 3:
			server._mapChannel[i->at(1)]->setMdp("");
			server._mapChannel[i->at(1)]->addMode("k", false);
			return ;
		case 4:
		{

			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				return ;
			}
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
			{

				str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
				printRPL(ERR_NOSUCHNICK, str.str(), client, server);
				return ;
			}
			server._mapChannel[i->at(1)]->rmModerator(it->second);
			server._mapChannel[i->at(1)]->addMode("o", false);
							str.str("");
							str.clear();
							str << client.getNick() << " = " << i->at(1) << " :" << server._mapChannel[i->at(1)]->getList();
							std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
							for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); ++it)
							{
								printRPL(RPL_NAMREPLY, str.str(), *it->second, server);
							}
						
							str.str("");
							str.clear();
							str << client.getNick() << " " << i->at(1) << " :End of /NAMES list";
							std::map<std::string, Client *> ptr2 = server._mapChannel[i->at(1)]->getMembers();
							for (std::map<std::string, Client *>::iterator it2 = ptr2.begin(); it2 != ptr2.end(); ++it2)
							{
								printRPL(RPL_ENDOFNAMES, str.str(), *it2->second, server);
							}		//todo nécessaire de l'envoyer a tous aussi je suppose ?
			return ;
		}
		case 5:
			server._mapChannel[i->at(1)]->setUserLimit(0);
			server._mapChannel[i->at(1)]->addMode("l", false);
			return ;
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ;
		}
	}
}		//	"MODE #l\r\nWHO #l\r\n"

int	flagCheck(std::string	str)
{
	if (str.size() > 2)
		return (0);
	if (str.find("i",0) == 1)
		return (1);
	if (str.find("t",0) == 1)
		return (2);
	if (str.find("k",0) == 1)
		return (3);
	if (str.find("o",0) == 1)
		return (4);
	if (str.find("l",0) == 1)
		return (5);
	return (-1);
}

void	fctPRIVMSG(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	std::string msg;
	if (i->at(2).at(0) == ':')
		msg = i->at(2).substr(1) + " ";
	else
		msg = i->at(2) + " ";
	std::size_t n = 3;
	while (n < i->size())
	{
		msg += i->at(n) + " ";
		n++;
	}
	if (i->size() < 3)
	{
		str << client.getNick() << " :No text to send";
		printRPL(ERR_NOTEXTTOSEND, str.str(), client, server);
		return ; 
	}
	if (i->at(1).at(0) != '#')	//si PRIVMSG a un autre client
	{
		std::map<int, Client*>::iterator it = server._mapClient.begin();
		while (it != server._mapClient.end() && it->second->getNick() != i->at(1))
			it++;
		if (it == server._mapClient.end())
		{
			str << client.getNick() << " " << i->at(1) << " :No such nick/channel";
			printRPL(ERR_NOSUCHNICK, str.str(), client, server);
			return ;
		}
		printMSG(msg, client, *it->second, server);
	}
	else		// channel
	{
		printChannel(msg, client, *server._mapChannel[i->at(1)], server);
	}
}


// i->at(1) : target
// i->at(2+) : msg

// "PRIVMSG #ok :hola ?\r\n"


// ERR_NOSUCHNICK (401)
// ERR_CANNOTSENDTOCHAN (404)
// ERR_NOTEXTTOSEND (412)



void	fctPART(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;

	if (i->size() < 3)	// HexChat ('part' '#general' ':reason')
	{
		str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
		printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
		return ;
	}
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such channel";
		printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
		return ;
	}
	if (server._mapChannel[i->at(1)]->isMember(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not on that channel";
		printRPL(ERR_NOTONCHANNEL, str.str(), client, server);
		return ;
	}

	client.rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(&client);
	
	str << ":" << client.getNick() << "!" + client.getUser() + "@" << "IRCserv" << " PART " << i->at(1) << " " << i->at(2) << "\r\n";
	
	std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it = ptr.begin(); it != ptr.end(); ++it)
	{
			it->second->setMailbox(str.str(), server.getEpollfd());
	}
	client.setMailbox(str.str(), server.getEpollfd());
}


void	JOIN0(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::vector<std::string> str;
	std::string tmp;
	std::vector<std::vector<std::string> > j;

	(void)i;
	for (std::map<std::string,Channel*>::iterator	it = server._mapChannel.begin(); it != server._mapChannel.end(); ++it)
	{
		str.clear();
		tmp = "PART " + it->second->getName() + " leaving";
		str.push_back(tmp);
		j = splitVector(str, " ");
		if (it->second->isMember(client.getNick()))
		{
			fctPART(j.begin(), server, client);
			// it->second->rmMember(&client);
		}
			
	}
	
}


void	fctQUIT(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;

	JOIN0(i, server, client);
	client.clearChannel();	//maybe leaks for later
	for (std::map<std::string,Channel*>::iterator	it = server._mapChannel.begin(); it != server._mapChannel.end(); it++)
	{
		if (it->second->isMember(client.getNick()))
			it->second->rmMember(&client);
	}

	str << client.getNick() << " is exiting the network with the message: Quit: " << i->at(1) << "\r\n";

	std::map<int, Client *>::iterator curClient = server._mapClient.find(client.getFd());
	for (std::map<int, Client *>::iterator it2 = server._mapClient.begin(); it2 != server._mapClient.end(); it2++)
	{
		if (it2 != curClient)
			it2->second->setMailbox(str.str(), server.getEpollfd());
	}
}
//i->at(0) "QUIT"
//i->at(1) ":leaving"


/*TODO :
	fctQUIT pour avoi segfault
	fctPART pour kick soi meme proprement
	finir la registration (below)	pour avoir le bouton join channel
	leaks
*/



// Upon successful completion of the registration process, the server MUST send, in this order:

// RPL_WELCOME (001),
// RPL_YOURHOST (002),
// RPL_CREATED (003),
// RPL_MYINFO (004),
// at least one RPL_ISUPPORT (005) numeric to the client.
// The server MAY then send other numerics and messages.
// The server SHOULD then respond as though the client sent the LUSERS command and return the appropriate numerics.
// The server MUST then respond as though the client sent it the MOTD command, i.e. it must send either the successful Message of the Day numerics or the ERR_NOMOTD (422) numeric.
// If the user has client modes set on them automatically upon joining the network, the server SHOULD send the client the RPL_UMODEIS (221) reply or a MODE message with the client as target, preferably the former.
// The first parameter of the RPL_WELCOME (001) message is the nickname assigned by the network to the client. Since it may differ from the nickname the client requested with the NICK command (due to, e.g. length limits or policy restrictions on nicknames), the client SHOULD use this parameter to determine its actual nickname at the time of connection. Subsequent nickname changes, client-initiated or not, will be communicated by the server sending a NICK
