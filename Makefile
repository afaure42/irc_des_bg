NAME	= irc_des_bg
CC		= c++
FLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 -g3
rm		= rm -f

COMMANDS_FILES_PATH =	./src/commands/
NETWORK_FILES_PATH	=	./src/network/
SERVER_FILES_PATH	=	./src/server/
INCLUDES			=	-I./includes
				
COMMANDS_FILES		=	execCommandUtils.cpp join.cpp nick.cpp part.cpp pass.cpp ping.cpp \
						privmsg.cpp syntaxChecks.cpp user.cpp numericReplies.cpp
COMMANDS_PATH		=	$(addprefix ${COMMANDS_FILES_PATH}, ${COMMANDS_FILES})
COMMANDS_OFILES		=	$(COMMANDS_PATH:.cpp=.o)

NETWORK_FILES		=	writeClient.cpp Scheduler.cpp serverLoop.cpp Server.cpp serverInit.cpp \
						customExceptions.cpp readClient.cpp Client.cpp Update.cpp
NETWORK_PATH		=	$(addprefix ${NETWORK_FILES_PATH}, ${NETWORK_FILES})
NETWORK_OFILES		=	$(NETWORK_PATH:.cpp=.o)

SERVER_FILES		=	Command.cpp functionMap.cpp User.cpp Channel.cpp TEST_MAIN.cpp
SERVER_PATH			=	$(addprefix ${SERVER_FILES_PATH}, ${SERVER_FILES})
SERVER_OFILES		=	$(SERVER_PATH:.cpp=.o)


ALL_OFILES			+=	$(SERVER_OFILES) $(NETWORK_OFILES) $(COMMANDS_OFILES)
DEP					=	${ALL_OFILES:.o=.d}

.cpp.o:
	$(CC) $(FLAGS) ${INCLUDES} -c $< -o $@

all : $(NAME)

-include $(DEP)

$(NAME) : $(ALL_OFILES)
	$(CC) $(FLAGS) ${INCLUDES} -o $(NAME) $(ALL_OFILES)

clean :
	${RM} $(ALL_OFILES) $(DEP)

fclean : clean
	${RM} $(NAME)

re : fclean all

.PHONY : all clean fclean re .cpp.o
