/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:37:11 by tdutel            #+#    #+#             */
/*   Updated: 2024/05/23 14:12:19 by tdutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../incs/Exceptions.hpp"


//-----fctJOIN-----//

char const*	InvitOnlyException::what()				const throw() {return ("ERROR: Cannot join the channel, need an invitation");}
char const*	WrongPasswordException::what()			const throw() {return ("ERROR: Cannot join the channel, wrong password");}
char const*	NeedPasswordException::what()			const throw() {return ("ERROR: Cannot join the channel, need a password");}
char const*	ChannelIsFullException::what()			const throw() {return ("ERROR: Cannot join the channel, reach user limits");}


//-----fctKICK-----//

char const*	WrongArgsException::what()				const throw() {return ("ERROR: Wrong number of arguments");}
char const*	ChannelDoesNotExistException::what()	const throw() {return ("ERROR: Channel specified does not exist");}
char const*	ClientDoesNotExistException::what()		const throw() {return ("ERROR: Client specified does not exist");}
char const*	ClientIsNotInChannelException::what()	const throw() {return ("ERROR: User is not in the specified channel");}
char const*	NotAllowedException::what()				const throw() {return ("ERROR: You're not allowed to use this command (not a moderator)");}









// char const*	GenericException::what()				const throw() {return ("ERROR: GenericException placeholder");}

// // User input
// char const*	UserInputException::what()				const throw() {return ("ERROR: UserInputException placeholder");}
// char const*	NotEnoughArgumentException::what()		const throw() {return ("ERROR: Usage is ./ircserv <port> <password>");}
// char const*	InvalidArgumentException::what()		const throw() {return ("ERROR: Invalid argument specified");}
// char const*	InvalidPortException::what()			const throw() {return ("ERROR: Invalid port specified");}

// // System call
// char const*	SystemCallException::what()				const throw() {return ("ERROR: SystemCallException placeholder");}
// char const*	SocketFailException::what()				const throw() {return ("ERROR: Socket Failed");}
// char const*	SetSockOptFailException::what()			const throw() {return ("ERROR: SetSockOpt Failed");}
// char const*	BindFailException::what()				const throw() {return ("ERROR: Bind Failed");}
// char const*	ListenFailException::what()				const throw() {return ("ERROR: Listen Failed");}
// char const*	RecvFailException::what()				const throw() {return ("ERROR: Recv Failed");}
// char const*	SelectFailException::what()				const throw() {return ("ERROR: Select Failed");}
// char const*	AcceptFailException::what()				const throw() {return ("ERROR: Accept Failed");}

// // Signal
// char const*	SigIntException::what()					const throw() {return ("SIGINT called, server stopped.");}

// // Code
// char const*	InvalidAuthIdException::what()			const throw() {return ("ERROR: Invalid authentification ID given");}

// // Client Input
// char const*	ClientInputException::what()			const throw() {return ("ERROR: ClientInputException placeholder");}
// char const*	WrongSyntaxException::what()			const throw() {return ("ERROR: Wrong syntax in argument specified");}
// char const*	WrongNicknameException::what()			const throw() {return ("ERROR: Wrong syntax in nickname specified (You can't use #)");}
// char const*	ClientHasQuitException::what()			const throw() {return ("Client has quit");}
// char const*	NicknameTakenException::what()			const throw() {return ("ERROR: The nickname specified is already taken");}
// char const*	IncorrectServerPassException::what()	const throw() {return ("ERROR: Incorrect server password specified");}
// char const*	MessageNotFoundException::what()		const throw() {return ("ERROR: Message not found in PRIVMSG");}
// char const*	ClientDoesNotExistException::what()		const throw() {return ("ERROR: Client specified does not exist");}
// char const*	ChannelDoesNotExistException::what()	const throw() {return ("ERROR: Channel specified does not exist");}
// char const*	ClientIsNotInChannelException::what()	const throw() {return ("ERROR: User is not in the specified channel");}
// char const*	ClientAlreadyInMapException::what()		const throw() {return ("ERROR: Client is already in specified map");}
// char const*	ClientIsNotInMapException::what()		const throw() {return ("ERROR: Client is not in specified map");}
// char const*	MessageToLongException::what()			const throw() {return ("ERROR: Message sent is too long");}

// char const*	NotEnoughParamException::what()			const throw() {return ("ERROR: Not enough parameters");}
// char const*	TooManyParamException::what()			const throw() {return ("ERROR: Too many parameters");}
// char const*	TooManyPassException::what()			const throw() {return ("ERROR: Too many passwords");}
// char const*	NeedPassException::what()				const throw() {return ("ERROR: No password found, need one");}
// char const*	IncorrectChannelPassException::what()	const throw() {return ("ERROR: Incorrect channel password specified");}
// char const*	CannotRemoveOperatorException::what()	const throw() {return ("ERROR: Cannot remove specified operator");}
// char const*	CannotKickOperatorException::what()		const throw() {return ("ERROR: Cannot kick an operator");}
// char const*	NoInvitationException::what()			const throw() {return ("ERROR: Cannot join the channel, need an invitation");}
// char const*	ReachUserLimitException::what()			const throw() {return ("ERROR: Cannot join the channel, reach user limits");}
// char const*	ClientIsNotOperatorException::what()	const throw() {return ("ERROR: Client is not operator");}
// char const*	ClientIsNotInvitedException::what()		const throw() {return ("ERROR: Client is not invited to the specified channel");}
// char const*	ClientAlreadyInvitedException::what()	const throw() {return ("ERROR: Client is already invited to the specified channel");}
