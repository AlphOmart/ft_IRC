# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tdutel <tdutel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/26 13:52:16 by tdutel            #+#    #+#              #
#    Updated: 2024/04/19 09:05:40 by tdutel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#---COMMON_VAR-----------------------------------
NAME		=	ircserv
CXX			=	c++
FLAGS		=	-std=c++98 -Wall -Werror -Wextra -g3	-glldb
#---PROJECT+VAR----------------------------------
SRC			=	main.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Command.cpp

OBJS_DIR	=	.OBJS/
OBJS		=	$(addprefix $(OBJS_DIR), $(SRC:.cpp=.o))
HEADER		=	incs/Irc.hpp \
				incs/Server.hpp \
				incs/Client.hpp \
				incs/Color.hpp
DEP			=	$(addprefix $(OBJS_DIR), $(SRCS:.c=.d))
#---RULES----------------------------------------

all:			$(NAME)

$(NAME):		$(OBJS) $(HEADER)
				@echo "\033[0;33m\nCOMPILING $(NAME)...\n"
				@$(CXX) $(FLAGS) $(OBJS) -o $@
				@echo "\033[1;32m./$(NAME) created\n\033[0m"

-include $(DEP)

$(OBJS_DIR)%.o:	%.cpp | dir
				@$(CXX) $(FLAGS) -c $< -o $@

dir:			$(OBJS_DIR)

$(OBJS_DIR):
				@mkdir -p $(OBJS_DIR)
				@mkdir -p $(OBJS_DIR)srcs

clean:
				@echo "\033[0;31mDeleting $(NAME) object...\n"
				@$(RM) -rf $(OBJS_DIR)
				@echo "\033[1;32mDone\n"

fclean:			clean
				@echo "\033[0;31mDeleting $(NAME) executable..."
				@rm -rf $(NAME)
				@echo "\033[1;32mDone\n"

re:				fclean
				$(MAKE) all

.PHONY:			all clean fclean re
