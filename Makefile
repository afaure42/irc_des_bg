NAME	= irc_des_bg
CC		= c++
FLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 -g3
rm		= rm -f

SRC_FILES_PATH		=	./src/
NETWORK_FILES_PATH	=	./src/network/
INCLUDES			=	-I./includes
				
SRC_FILES			=	Command.cpp functionMap.cpp User.cpp Channel.cpp TEST_MAIN.cpp \
						syntax_checks.cpp privmsg.cpp
SRC_PATH			=	$(addprefix ${SRC_FILES_PATH}, ${SRC_FILES})
SRC_OFILES			=	$(SRC_PATH:.cpp=.o)

NETWORK_FILES		=	write_client.cpp Scheduler.cpp server_loop.cpp Server.cpp server_init.cpp \
						custom_exceptions.cpp read_client.cpp Client.cpp Update.cpp
NETWORK_PATH		=	$(addprefix ${NETWORK_FILES_PATH}, ${NETWORK_FILES})
NETWORK_OFILES		=	$(NETWORK_PATH:.cpp=.o)

ALL_OFILES			+=	$(SRC_OFILES) $(NETWORK_OFILES)
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
