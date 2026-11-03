FILES 	=	ft_nmap.c parser.c parser_file.c ft_split.c utils.c scan_jobs.c target.c scan.c sender.c packet.c checksum.c
NAME	=	ft_nmap
CC		=	cc
CFLAGS	+=	-Wall -Wextra -Werror
LDLIBS	=	-pthread
RM		=	rm -rf
INCLUDE	=	-I include/

SRCS_DIR	=	src/
OBJ_DIR 	=	obj/
OBJS		=	$(addprefix $(OBJ_DIR), $(FILES:.c=.o))
DEPS		=	$(OBJS:.o=.d)

TEST_NAME	=	test_parser
TEST_SRCS	=	tests/test_parser.c
# every object except the one holding main()
TEST_OBJS	=	$(filter-out $(OBJ_DIR)ft_nmap.o, $(OBJS))

all   : header message_all $(NAME)

$(NAME)	:	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDLIBS)
	@printf '%b\n' "$(COLOUR_GREEN)\n╔═════════════════════════════╗$(NO_COLOUR)"
	@printf '%b\n' "$(COLOUR_GREEN)║   COMPILATION SUCCESS !!!   ║$(NO_COLOUR)"
	@printf '%b\n' "$(COLOUR_GREEN)╚═════════════════════════════╝$(NO_COLOUR)"

# -MMD -MP: emit .d files so editing a header recompiles the objects that include it
$(OBJ_DIR)%.o 	:	$(SRCS_DIR)%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -MP $(INCLUDE) -c $< -o $@
	@printf '%b\n' "$(COLOUR_YELLOW)Compiling ... $(COLOUR_GREEN)[✔]	$<$(NO_COLOUR)"

$(OBJ_DIR)	:
	@mkdir -p $@

test	:	$(TEST_NAME)
	@./$(TEST_NAME)

$(TEST_NAME)	:	$(TEST_SRCS) $(TEST_OBJS) include/ft_nmap.h
	@$(CC) $(CFLAGS) $(INCLUDE) $(TEST_SRCS) $(TEST_OBJS) -o $@ $(LDLIBS)

clean	:	header message_clean
	@$(RM) $(OBJ_DIR)

fclean	:	header message_fclean
	@$(RM) $(OBJ_DIR) $(NAME) $(TEST_NAME)

re		:	fclean all

message_all:
	@printf '%b\n' "EXECUTION	: $(COLOUR_RED) [all]\n$(NO_COLOUR)"

message_clean:
	@printf '%b\n' "EXECUTION	: $(COLOUR_RED) [clean]\n$(NO_COLOUR)"

message_fclean:
	@printf '%b\n' "EXECUTION	: $(COLOUR_RED) [fclean]\n$(NO_COLOUR)"

header:
	clear
	@printf '%b\n' "\033[32m  _______   _______   _______   _______   _______   _______   _______   _______   _______ "
	@printf '%b\n' " /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\"
	@printf '%b\n' " \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/"
	@printf '%b\n' "\033[34m ___       ___                                                   ___       __             "
	@printf '%b\n' "/\\_ \\     /\\_ \\                                                 /\\_ \\     /\\ \\            "
	@printf '%b\n' "\\//\\ \\    \\//\\ \\       __              ____     __      __  __  \\//\\ \\    \\ \\/      ____  "
	@printf '%b\n' "  \\ \\ \\     \\ \\ \\    /'__'\\  _______  /',__\\  /'__'\\   /\\ \\/\\ \\   \\ \\ \\    \\/      /',__\\ "
	@printf '%b\n' "   \\_\\ \\_    \\_\\ \\_ /\\  __/ /\\______\\/\\__, '\\/\\ \\L\\.\\_ \\ \\ \\_\\ \\   \\_\\ \\_         /\\__, '\\"
	@printf '%b\n' "   /\\____\\   /\\____\\\\\\_\\___\\ \\\\/______/\\/\\____/\\ \\__/.\\_\\ \\ \\____/   /\\____\\        \\/\\____/"
	@printf '%b\n' "   \\/____/   \\/____/ \\/____/          \\/___/  \\/__/\\/_/  \\/___/    \\/____/         \\/___/ "
	@printf '%b\n' "                       __                   ___          ___                             "
	@printf '%b\n' " /'\\_/'\\              /\\ \\                /'___\\  __    /\\_ \\                            "
	@printf '%b\n' "/\\      \\      __     \\ \\ \\/'\\      __   /\\ \\__/ /\\_\\   \\//\\ \\       __                  "
	@printf '%b\n' "\\ \\ \\__\\ \\   /'__'\\    \\ \\ , <    /'__'\\ \\ \\,__\ \\\/\\ \\    \\ \\ \\    /'__'\\                "
	@printf '%b\n' " \\ \\ \\_/\\ \\ /\\ \\L\\.\\_   \\ \\ \\\\\\'\\ /\\  __/  \\ \\ \\_/ \\ \\ \\    \\_\\ \\_ /\\  __/                "
	@printf '%b\n' "  \\ \\_\\\\  \\_\\\\\\ \\__/.\\_\\   \\ \\_\\ \\_\\\\ \\____\\  \\ \\_\\   \\ \\_\\   /\\____\\\\  \\____\\               "
	@printf '%b\n' "   \\/_/ \\/_/ \\/__/\\/_/    \\/_/\\/_/\\/____/   \\/_/    \\/_/   \\/____/ \\/____/               "
	@printf '%b\n' "\033[32m  _______   _______   _______   _______   _______   _______   _______   _______   _______ "
	@printf '%b\n' " /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\"
	@printf '%b\n' " \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \033[0m \n"
	@printf '%b\n' "PROGRAM		: $(NAME)\n"

.PHONY	:	all clean fclean re test header message_all message_clean message_fclean

-include $(DEPS)

# colors

COLOUR_RED		= \033[31m
COLOUR_YELLOW	= \033[33m
COLOUR_BLUE		= \033[34m
COLOUR_GREEN	= \033[32m
NO_COLOUR		= \033[0m
