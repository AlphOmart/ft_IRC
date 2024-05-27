/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:37 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/27 12:39:07 by tdutel           ###   ########.fr       */
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
			printERR(ERR_PASSWDMISMATCH, str.str(), client);
			//server.kickClient(client.getFd());
		}
	}
	else
	{
		// std::stringstream str;
		str << client.getNick() << " :You may not reregister";
		printERR(ERR_ALREADYREGISTERED, str.str(), client);
	}
}

void	fctNICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (server.nickAlreadyUsed(i->at(1)) == false)
		client.setNickname(i->at(1));
	else
	{
		std::string response;
		response = ":IRCServ 433 *: Nickname is already used. \r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	if (client.getIsRegistered() == false && client.isRegistered() == true)	//permet de ignorer quand déjà  connecté
	{
		str << client.getNick() << " :Welcome to the " << "IRCServ" << " Network, " << client.getNick() << "[!" << client.getUser() << "@IRCServ]";
		printERR(RPL_WELCOME, str.str(), client);
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
			printERR(RPL_WELCOME, str.str(), client);
			// std::string response;
			// response = ":IRCServ 001 " + client.getNick() + " : Welcome to the IRCServ " + client.getUser() + " @IRCServ\r\n";
			// send(client.getFd(), response.c_str(), response.length(), 0);
		}
	}
	else
	{
		str << client.getNick() << " :You may not reregister";
		printERR(ERR_ALREADYREGISTERED, str.str(), client);
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
			printERR(ERR_NOSUCHCHANNEL, str.str(), client);
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
			printERR(ERR_INVITEONLYCHAN, str.str(), client);
			return ;// throw InvitOnlyException();
		}
		if (server._mapChannel[i->at(1)]->getIsMdp() == true && i->size() == 3)	//si le channel a un mdp et qu'on a passé un mdp
		{
			if (server._mapChannel[i->at(1)]->getMdp() != i->at(2))
			{
				str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+k)";
				printERR(ERR_BADCHANNELKEY, str.str(), client);
				return ;//throw WrongPasswordException();
			}
		}
		else if (server._mapChannel[i->at(1)]->getIsMdp() == true)
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+k)";
			printERR(ERR_BADCHANNELKEY, str.str(), client);
			return ;//throw NeedPasswordException();//todo simplifier
		}
		if (server._mapChannel[i->at(1)]->getIsUserLimit() == true && server._mapChannel[i->at(1)]->getMemberSize() >= server._mapChannel[i->at(1)]->getIsUserLimit())
		{
			str << client.getNick() << " " << server._mapChannel[i->at(1)]->getName() << " :Cannot join channel (+l)";
			printERR(ERR_CHANNELISFULL, str.str(), client);
			return ;//throw ChannelIsFullException();
		}
		client.addChannel(server._mapChannel[i->at(1)]);
		server._mapChannel[i->at(1)]->addMember(&client);
	}
	std::string	server_msg = ":" + client.getNick() + "!" + client.getUser() + "@ircserv JOIN " + ":" +  i->at(1) + "\r\n";
	send(client.getFd(), server_msg.c_str(), server_msg.size(), 0);
}

// : ircserv NR : 



void	fctKICK(std::vector<std::vector<std::string> >::iterator i, Server& server, Client& client)
{
	std::stringstream str;
	if (i->size() != 4)	// 4 instead of 3 because of HexChat ('kick' 'irc' '#general' ':tim')
		{
			
		throw WrongArgsException(); //todo trouver quoi mettre comme NR
		}
	if (server._mapChannel.find(i->at(2)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(2) << " :No such channel";
		printERR(ERR_NOSUCHCHANNEL, str.str(), client);
		return ;
		// throw ChannelDoesNotExistException();
	}
	std::string usr = i->at(3).substr(1);		//	pour gérer ":" devant client pour Hexchat : 'kick' 'irc' '#general' ':tim'
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != usr)
		it++;
	if (it == server._mapClient.end())
	{
		str << client.getNick() << " " << i->at(3) << " " << i->at(2) << " :They aren't on that channel";
		printERR(ERR_USERNOTINCHANNEL, str.str(), client);
		return ;
		// throw ClientDoesNotExistException();
	}
	if (server._mapChannel[i->at(2)]->isMember(it->second->getNick()) == false)
	{
		str << client.getNick() << " " << i->at(3) << " " << i->at(2) << " :They aren't on that channel";
		printERR(ERR_USERNOTINCHANNEL, str.str(), client);
		return ;
		// throw ClientIsNotInChannelException();
	}
	if (server._mapChannel[i->at(2)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " :Permission Denied- You're not an IRC operator";
		printERR(ERR_NOPRIVILEGES, str.str(), client);
		return ;
		// throw NotAllowedException();
	}
	it->second->rmChannel(server._mapChannel[i->at(2)]);
	server._mapChannel[i->at(2)]->rmMember(it->second);
	if (server._mapChannel[i->at(2)]->isModerator(it->second->getNick()) == true)
		server._mapChannel[i->at(2)]->rmModerator(it->second);
	if (server._mapChannel[i->at(2)]->isInvited(it->second->getNick()) == true)
		server._mapChannel[i->at(2)]->rmInvitMember(it->second);
	// throw ("NR : kick successfully.");
	
	str << client.getNick() << " :kicked " << usr << " from " << i->at(2) << ".";
	printERR(9999, str.str(), client);
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
	if (i->size() != 3)
		throw ("NR1 : Wrong numbers of args"); //todo trouver quoi mettre comme NR
	if (server._mapChannel.find(i->at(2)) == server._mapChannel.end())
		throw ("NR : Channel doesn't exist");
	if (server._mapChannel[i->at(2)]->isMember(client.getNick()) == false)
		throw ("NR : You must be in the channel to invite someone (?ERR_NOTONCHANNEL?)");
	std::map<int, Client*>::iterator it = server._mapClient.begin();
	while (it != server._mapClient.end() && it->second->getNick() != i->at(1))
		it++;
	if (it == server._mapClient.end())
		throw ("NR : client doesn't exist.");
	server._mapChannel[i->at(2)]->addInvitMember(it->second);
	throw ("NR : client invited successfully");
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
	if (i->size() < 2)
		throw ("NR3 : Wrong numbers of args"); //todo trouver quoi mettre comme NR
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
		throw ("NR : Channel doesn't exist");
	if (server._mapChannel[i->at(1)]->getTopicRestriction() == true && server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
		throw ("NR : you're not allowed to use this command (not a moderator)");
	if (i->size() == 2)
		throw ("Topic : " + server._mapChannel[i->at(1)]->getTopic());
	std::string newtopics;
	std::size_t n = 2;
	while (n < i->size())
	{
		newtopics += i->at(n) + " ";
		n++;
	}
	// std::string newtopic = i->at(2).substr(1);
	server._mapChannel[i->at(1)]->setTopic(newtopics);
	throw ("Changed Topic successfully : new topic : " + server._mapChannel[i->at(1)]->getTopic());
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
		printERR(ERR_NEEDMOREPARAMS, str.str(), client);
		return ;
	}
	if (server._mapChannel.find(i->at(1)) == server._mapChannel.end())
	{
		str << client.getNick() << " " << i->at(1) << " :No such channel";
		printERR(ERR_NOSUCHCHANNEL, str.str(), client);
		return ; // throw ("NR : Channel doesn't exist");
	}
	if (server._mapChannel[i->at(1)]->isModerator(client.getNick()) == false)
	{
		str << client.getNick() << " :Permission Denied- You're not an IRC operator";
		printERR(ERR_NOPRIVILEGES, str.str(), client);
		return ; // throw ("NR : you're not allowed to use this command (not a moderator)");
	}
	if (i->size() == 2)
	{
		str << "MODE " << server._mapChannel[i->at(1)]->getName() << " " << server._mapChannel[i->at(1)]->getModes();
		printERR(9999, str.str(), client);
		return ; // :ircserver MODE #channel +m
	}
	if (i->at(2).at(0) != '+' && i->at(2).at(0) != '-')
	{
		str << client.getNick() << " :Unknown MODE flag";
		printERR(ERR_UMODEUNKNOWNFLAG, str.str(), client);
		return ;
		// throw ("NR : need operand + or - before flag");
	}
	if (i->at(2).at(0) == '+')
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printERR(ERR_UMODEUNKNOWNFLAG, str.str(), client);
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
			if (i->size() != 4)
			{
				str << client.getNick() << " " << i->at(0) << " :Not enough parameters";
				printERR(ERR_NEEDMOREPARAMS, str.str(), client);
				// str << client.getNick() << " " << i->at(1) << " :Key is not well-formed";
				// printERR(ERR_INVALIDKEY, str.str(), client);
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
				printERR(ERR_NEEDMOREPARAMS, str.str(), client);
				return ;
			}
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
			{
				str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
				printERR(ERR_NOSUCHNICK, str.str(), client);
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
				printERR(ERR_NEEDMOREPARAMS, str.str(), client);
				return ;
			}
			server._mapChannel[i->at(1)]->setUserLimit(std::atoi(i->at(3).c_str()));
			server._mapChannel[i->at(1)]->addMode("l", true);
			return ; //throw ("NR : user limit added successfully");
		default:
			str << client.getNick() << " :Unknown MODE flag";
			printERR(ERR_UMODEUNKNOWNFLAG, str.str(), client);
			return ; // throw("NR : unknown flag");
		}
	}
	else
	{
		switch (flagCheck(i->at(2)))
		{
		case 0:
			str << client.getNick() << " :Unknown MODE flag";
			printERR(ERR_UMODEUNKNOWNFLAG, str.str(), client);
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
				printERR(ERR_NEEDMOREPARAMS, str.str(), client);
				return ;
			}
			std::map<std::string, Client *> members = server._mapChannel[i->at(1)]->getMembers();
			std::map<std::string, Client*>::iterator it = members.begin();
			while (it != members.end() && it->second->getNick() != i->at(3))
				it++;
			if (it == members.end())
			{

				str << client.getNick() << " " << i->at(3) << " :No such nick/channel";
				printERR(ERR_NOSUCHNICK, str.str(), client);
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
			printERR(ERR_UMODEUNKNOWNFLAG, str.str(), client);
			return ; // throw("NR : unknown flag");
		}
	}
}

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

// ---------------------------------------------------------------------//
//    Examples:	MODE

//            Use of Channel Modes:

// MODE #Finnish +i               ; Makes #Finnish channel 'invite-only'.

// MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on #Fins.

// MODE #42 +k oulu                ; Set the channel key to "oulu".

// MODE #eu-opers +l 10            ; Set the limit for the number of users on channel to 10.


void	fctTOPIC()
{
	std::cout << "TOPIC" << std::endl;
}

