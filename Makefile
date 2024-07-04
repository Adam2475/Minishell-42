NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRC = main.c
OBJ = $(SRC:.c=.o)
RM = rm -f

BANNER = \
"\n\
  __  __          _____ _          _ _ \n\
 |  \/  |        / ____| |        | | |\n\
 | \  / |___  __| (___ | |__   ___| | |\n\
 | |\/| / _ \/ _ \___ \| '_ \ / _ \ | |\n\
 | |  |  __/  /  ____| ) | | | |  __/ | |\n\
 |_|  |_|\___|  |_____/|_| |_|\___|_|_|\n\
"

all: banner $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(SRC) -lreadline -o $(NAME)

clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) $(NAME)

re: fclean all

banner:
		@echo $(BANNER)