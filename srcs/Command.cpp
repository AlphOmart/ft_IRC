/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/31 14:43:03 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

int		flagCheck(std::string	str);

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
			//server.kickClient(client.getFd());
		}
	}
	else
	{
		// std::stringstream str;
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
		// std::string response;
		// response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + "@IRCServ\r\n";
		// send(client.getFd(), response.c_str(), response.length(), 0);
	}
}

void	fctUSER(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	(void)server;
	std::stringstream str;
	if (client.getIsRegistered() == false)	//permet de ignorer la fonction quand connecté
	{
		client.setUser(i->at(1));
		if (client.isRegistered() == true)
		{
			str << client.getNick() << " :Welcome to the " << "IRCServ" << " Network, " << client.getNick() << "[!" << client.getUser() << "@IRCServ]";
			printRPL(RPL_WELCOME, str.str(), client, server);
			// std::string response;
			// response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + " @IRCServ\r\n";
			// send(client.getFd(), response.c_str(), response.length(), 0);
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
			str << client.getNick() << " " << i->at(1) << " :No such channel";
			printRPL(ERR_NOSUCHCHANNEL, str.str(), client, server);
			return ;
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
			return ;// throw InvitOnlyException();
		}
		if (server._mapChannel[i->at(1)]->getIsMdp() == true)	//si le channel a un mdp et qu'on a passé un mdp
		{
			if ((i->size() != 3) || (i->size() == 3 && server._mapChannel[i->at(1)]->getMdp() != i->at(2)))
			{
				str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+k)";
				printRPL(ERR_BADCHANNELKEY, str.str(), client, server);
				return ;//throw WrongPasswordException();
			}
		}
		if (server._mapChannel[i->at(1)]->getIsUserLimit() == true && server._mapChannel[i->at(1)]->getMemberSize() >= server._mapChannel[i->at(1)]->getIsUserLimit())
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+l)";
			printRPL(ERR_CHANNELISFULL, str.str(), client, server);
			return ;//throw ChannelIsFullException();
		}
		client.addChannel(server._mapChannel[i->at(1)]);
		server._mapChannel[i->at(1)]->addMember(&client);
	}
	std::string	server_msg = ":" + client.getNick() + "!" + client.getUser() + "@ircserv JOIN :" +  i->at(1) + "\r\n";
	client.setMailbox(server_msg, server.getEpollfd());
	// if (send(client.getFd(), server_msg.c_str(), server_msg.size(), 0) == -1)
	// 	throw std::runtime_error("Error while sending.");

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
		return ; // throw ChannelDoesNotExistException();
	}
	std::string com = i->at(3).substr(1);		//	pour gérer ":" devant motif Hexchat
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(2))
		it++;
	if (it == server._mapClient.end())
	{
		str << client.getNick() << " " << i->at(2) << " " << i->at(1) << " :They aren't on that channel";
		printRPL(ERR_USERNOTINCHANNEL, str.str(), client, server);
		return ; // throw ClientDoesNotExistException();
	}
	if (server._mapChannel[i->at(1)]->isMember(it->second->getNick()) == false)
	{
		str << client.getNick() << " " << i->at(2) << " " << i->at(1) << " :They aren't on that channel";
		printRPL(ERR_USERNOTINCHANNEL, str.str(), client, server);
		return ; // throw ClientIsNotInChannelException();
	}
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " " << i->at(1) << " :You're not channel operator";
		printRPL(ERR_CHANOPRIVSNEEDED, str.str(), client, server);
		return ; // throw NotAllowedException();
	}
	it->second->rmChannel(server._mapChannel[i->at(1)]);
	server._mapChannel[i->at(1)]->rmMember(it->second);
	if (server._mapChannel[i->at(1)]->isModerator(it->second->getNick()) == true)
		server._mapChannel[i->at(1)]->rmModerator(it->second);
	if (server._mapChannel[i->at(1)]->isInvited(it->second->getNick()) == true)
		server._mapChannel[i->at(1)]->rmInvitMember(it->second);
	// throw ("NR : kick successfully.");
	
	str << ":" << client.getNick() << "!" + client.getUser() + "@" << "IRCserv" << " KICK " << i->at(1) << " " << i->at(2) << " " << com << "\r\n";
	
	std::map<std::string, Client *> ptr = server._mapChannel[i->at(1)]->getMembers();
	for (std::map<std::string, Client *>::iterator it2 = ptr.begin(); it2 != ptr.end(); ++it2)
	{
			it2->second->setMailbox(str.str(), server.getEpollfd());
	}
	it->second->setMailbox(str.str(), server.getEpollfd());

	// str << client.getNick() << " :kicked " << usr << " from " << i->at(2) << ".";
	// printRPL(9999, str.str(), client, server);
	// std::string	server_msg = ":" + client.getNick() + "!" + client.getUser() + "@ircserv KICK " + ":" +  it->second->getUser() + "successfully.\r\n";
	// send(client.getFd(), server_msg.c_str(), server_msg.size(), 0);
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
		return ; // throw ("NR : Channel doesn't exist");
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
		return ; // throw ("NR : you're not allowed to use this command (not a moderator)");
	}
	if (i->at(2).at(0) != '+' && i->at(2).at(0) != '-')
	{
		str << client.getNick() << " :Unknown MODE flag";
		printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
		return ;
		// throw ("NR : need operand + or - before flag");
	}
	if (i->at(2).at(0) == '+')
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ; // throw ("NR : too many args, only 1 letter");
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(true);
			server._mapChannel[i->at(1)]->addMode("i", true);
			return ; // throw ("NR : invit only mode added successfully");
		case 2:
			server._mapChannel[i->at(1)]->setTopicRestriction(true);
			server._mapChannel[i->at(1)]->addMode("t", true);
			return ; // throw ("NR : topic restriction added successfully");
		case 3:
		{
			if (i->size() < 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printRPL(ERR_NEEDMOREPARAMS, str.str(), client, server);
				// str << client.getNick() << " " << i->at(1) << " :Key is not well-formed";
				// printRPL(ERR_INVALIDKEY, str.str(), client, server);
				return ; // throw ("NR : password missed");
			}
			server._mapChannel[i->at(1)]->setMdp(i->at(3));
			server._mapChannel[i->at(1)]->addMode("k", true);
			return ; //throw ("NR : password added successfully");
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
			return ; // throw ("NR : moderator added successfully");
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
			return ; //throw ("NR : user limit added successfully");
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ; // throw("NR : unknown flag");
		}
	}
	else
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ; // throw ("NR : too many args, only 1 letter");
		case 1:
			server._mapChannel[i->at(1)]->setInvitOnly(false);
			server._mapChannel[i->at(1)]->addMode("i", false);
			return ; //throw ("NR : invit only mode removed successfully");
		case 2:
			// std::cout << "-t. everyone can change topic" << std::endl;
			server._mapChannel[i->at(1)]->setTopicRestriction(false);
			server._mapChannel[i->at(1)]->addMode("t", false);
			return ; //throw ("NR : topic restriction removed successfully");
		case 3:
			server._mapChannel[i->at(1)]->setMdp("");
			server._mapChannel[i->at(1)]->addMode("k", false);
			return ; //throw ("NR : password removed successfully");
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
			return ; //throw ("NR : moderator removed successfully");
		}
		case 5:
			server._mapChannel[i->at(1)]->setUserLimit(0);
			server._mapChannel[i->at(1)]->addMode("l", false);
			return ; //throw ("NR : user limit removed successfully");
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printRPL(ERR_UMODEUNKNOWNFLAG, str.str(), client, server);
			return ; // throw("NR : unknown flag");
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

// ---------------------------------------------------------------------//
//    Examples:	MODE

//            Use of Channel Modes:

// MODE #Finnish +i               ; Makes #Finnish channel 'invite-only'.

// MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on #Fins.

// MODE #42 +k oulu                ; Set the channel key to "oulu".

// MODE #eu-opers +l 10            ; Set the limit for the number of users on channel to 10.
