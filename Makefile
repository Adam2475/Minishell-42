NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
# Source files
SRC_DIR = src/
SRCS = main parsing exit_handle
SRC = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRCS)))
# Object file generation
OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
#OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRCS)))
# Special target for creating obj directory
OBJF = .cache_exists
RM = rm -f

DEFAULT = \033[0;39m
GREEN = \033[0;92m
RED = \033[0;91m
CIANO = \033[0;96m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
GRAY = \033[0;90m
WHITE = \033[0;97m

BANNER = \
"$(GREEN)\
\n  __  __          _____ _          _ _ \n\
	|  \/  |        / ____| |        | | |\n\
	| \  / |___  __| (___ | |__   ___| | |\n\
 	| |\/| / _ \/ _ \___ \| '_ \ / _ \ | |\n\
 	| |  |  __/  /  ____| ) | | | |__/ | |\n\
 	|_|  |_|\___|  |_____/|_| |_|\___|_|_|\n\
$(DEFAULT)"

all: banner $(NAME)

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -lreadline -o $(NAME)
		@echo "$(GREEN)$(NAME) compilata con successo❗️ 📁$(DEFAULT)"

$(OBJ_DIR)/%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJ)
		@echo "$(CIANO)file oggetto di $(NAME) rimossi con successo❗️ 🪦$(DEFAULT)"

fclean: clean
		$(RM) $(NAME)
		$(RM) -r $(OBJ_DIR)
		@echo "$(CIANO)directory OBJ rimossa...$(DEFAULT)"
		@echo "$(RED)$(NAME) rimossa con successo❗️ ❌$(DEFAULT)"

re: fclean all

banner:
		@echo $(BANNER)