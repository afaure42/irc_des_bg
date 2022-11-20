NAME	= irc_des_bg
TESTER	= tester
CC		= c++
FLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 -g3
rm		= rm -f

COMMANDS_FILES_PATH =	./src/commands/
NETWORK_FILES_PATH	=	./src/network/
SERVER_FILES_PATH	=	./src/server/
TESTER_FILES_PATH	=	./src/tester/
INCLUDES			=	-I./includes
				
COMMANDS_FILES		=	execCommandUtils.cpp join.cpp nick.cpp part.cpp pass.cpp ping.cpp \
						privmsg.cpp syntaxChecks.cpp user.cpp numericReplies.cpp quit.cpp \
						away.cpp names.cpp die.cpp invite.cpp list.cpp topic.cpp oper.cpp \
						kick.cpp notice.cpp mode.cpp modeUpdates.cpp modeUtils.cpp who.cpp
COMMANDS_PATH		=	$(addprefix ${COMMANDS_FILES_PATH}, ${COMMANDS_FILES})
COMMANDS_OFILES		=	$(COMMANDS_PATH:.cpp=.o)

NETWORK_FILES		=	writeClient.cpp Scheduler.cpp Server.cpp serverInit.cpp \
						customExceptions.cpp readClient.cpp Client.cpp Update.cpp
NETWORK_PATH		=	$(addprefix ${NETWORK_FILES_PATH}, ${NETWORK_FILES})
NETWORK_OFILES		=	$(NETWORK_PATH:.cpp=.o)

SERVER_FILES		=	Command.cpp functionMap.cpp User.cpp Channel.cpp printUtils.cpp \
						operatorMap.cpp

SERVER_PATH			=	$(addprefix ${SERVER_FILES_PATH}, ${SERVER_FILES})
SERVER_OFILES		=	$(SERVER_PATH:.cpp=.o)

SERVER_MAIN			=	$(addprefix ${SERVER_FILES_PATH}, main.cpp)
SERVER_MAIN_OFILE	=	$(SERVER_MAIN:.cpp=.o)
TESTER_MAIN			=	$(addprefix ${TESTER_FILES_PATH}, tester.cpp)
TESTER_MAIN_OFILE	=	$(TESTER_MAIN:.cpp=.o)

ALL_OFILES			+=	$(SERVER_OFILES) $(NETWORK_OFILES) $(COMMANDS_OFILES)
DEP					=	${ALL_OFILES:.o=.d}
MAINS_DEP			=	${TESTER_MAIN_OFILE:.o=.d} + ${SERVER_MAIN_OFILE:.o=.d}

.cpp.o:
	$(CC) $(FLAGS) ${INCLUDES} -c $< -o $@

all : $(NAME)

-include $(DEP)

$(NAME) : $(ALL_OFILES) $(SERVER_MAIN_OFILE)
	$(CC) $(FLAGS) ${INCLUDES} -o $(NAME) $(ALL_OFILES) $(SERVER_MAIN_OFILE)

tester : $(ALL_OFILES) $(TESTER_MAIN_OFILE)
	$(CC) $(FLAGS) ${INCLUDES} -o $(TESTER) $(ALL_OFILES) $(TESTER_MAIN_OFILE)

clean :
	${RM} $(ALL_OFILES) $(SERVER_MAIN_OFILE) $(TESTER_MAIN_OFILE) $(DEP) $(MAINS_DEP)

fclean : clean
	${RM} $(NAME) $(TESTER)

re : fclean all

.PHONY : all clean fclean re .cpp.o
