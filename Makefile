FILES 	=	ft_nmap.c parser.c parser_file.c ft_split.c utils.c scan_jobs.c target.c scan.c sender.c packet.c checksum.c
NAME	=	ft_nmap
CC		=	cc
CFLAGS	+=	-W -Wall -Wextra -Werror
RM		=	rm -rf
INCLUDE	=	-I include/

SRCS_DIR	=	src/
OBJ_DIR 	=	obj/
OBJS		=	$(addprefix $(OBJ_DIR), $(FILES:.c=.o))
SRCS 		=	$(addprefix $(SRCS_DIR), $(FILES))

$(OBJ_DIR)%.o 	:	$(SRCS_DIR)%.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "$(COLOUR_YELLOW)Compiling ... $(COLOUR_GREEN)[✔]	$<$(NO_COLOUR)"

all   : header message_all $(NAME)

make_OBJS_DIR	:
	@mkdir -p $(OBJ_DIR)

$(NAME)	:	make_OBJS_DIR $(OBJS) message_end
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@

clean	:	header message_clean
	@$(RM) $(OBJ_DIR)

fclean	:	header message_fclean
	@$(RM) $(OBJ_DIR) $(NAME)

fclean_re	: header message_re
	@$(RM) $(OBJ_DIR)
	@$(RM) $(NAME)

re		:	fclean_re $(NAME)

message_all:
	@echo "EXECUTION	: $(COLOUR_RED) [all]\n$(NO_COLOUR)"

message_clean:
	@echo "EXECUTION	: $(COLOUR_RED) [clean]\n$(NO_COLOUR)"

message_fclean:
	@echo "EXECUTION	: $(COLOUR_RED) [fclean]\n$(NO_COLOUR)"

message_re:
	@echo "EXECUTION	: $(COLOUR_RED) [re]\n$(NO_COLOUR)"

message_end:
	@echo "$(COLOUR_GREEN)\n╔═════════════════════════════╗$(NO_COLOUR)"
	@echo "$(COLOUR_GREEN)║   COMPILATION SUCCESS !!!   ║$(NO_COLOUR)"
	@echo "$(COLOUR_GREEN)╚═════════════════════════════╝$(NO_COLOUR)"

header:
	clear
	@echo "\033[32m  _______   _______   _______   _______   _______   _______   _______   _______   _______ "
	@echo " /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\"
	@echo " \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/"
	@echo "\033[34m ___       ___                                                   ___       __             "
	@echo "/\\_ \\     /\\_ \\                                                 /\\_ \\     /\\ \\            "
	@echo "\\//\\ \\    \\//\\ \\       __              ____     __      __  __  \\//\\ \\    \\ \\/      ____  "
	@echo "  \\ \\ \\     \\ \\ \\    /'__'\\  _______  /',__\\  /'__'\\   /\\ \\/\\ \\   \\ \\ \\    \\/      /',__\\ "
	@echo "   \\_\\ \\_    \\_\\ \\_ /\\  __/ /\\______\\/\\__, '\\/\\ \\L\\.\\_ \\ \\ \\_\\ \\   \\_\\ \\_         /\\__, '\\"
	@echo "   /\\____\\   /\\____\\\\\\_\\___\\ \\\\/______/\\/\\____/\\ \\__/.\\_\\ \\ \\____/   /\\____\\        \\/\\____/"
	@echo "   \\/____/   \\/____/ \\/____/          \\/___/  \\/__/\\/_/  \\/___/    \\/____/         \\/___/ "
	@echo "                       __                   ___          ___                             "
	@echo " /'\\_/'\\              /\\ \\                /'___\\  __    /\\_ \\                            "
	@echo "/\\      \\      __     \\ \\ \\/'\\      __   /\\ \\__/ /\\_\\   \\//\\ \\       __                  "
	@echo "\\ \\ \\__\\ \\   /'__'\\    \\ \\ , <    /'__'\\ \\ \\,__\ \\\/\\ \\    \\ \\ \\    /'__'\\                "
	@echo " \\ \\ \\_/\\ \\ /\\ \\L\\.\\_   \\ \\ \\\\\\'\\ /\\  __/  \\ \\ \\_/ \\ \\ \\    \\_\\ \\_ /\\  __/                "
	@echo "  \\ \\_\\\\  \\_\\\\\\ \\__/.\\_\\   \\ \\_\\ \\_\\\\ \\____\\  \\ \\_\\   \\ \\_\\   /\\____\\\\  \\____\\               "
	@echo "   \\/_/ \\/_/ \\/__/\\/_/    \\/_/\\/_/\\/____/   \\/_/    \\/_/   \\/____/ \\/____/               "
	@echo "\033[32m  _______   _______   _______   _______   _______   _______   _______   _______   _______ "
	@echo " /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\ /\\______\\"
	@echo " \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \\/______/ \033[0m \n"
	@echo "PROGRAM		: $(NAME)\n"

# colors

COLOUR_RED		= \033[31m
COLOUR_YELLOW	= \033[33m
COLOUR_BLUE		= \033[34m
COLOUR_GREEN	= \033[32m
NO_COLOUR		= \033[0m
