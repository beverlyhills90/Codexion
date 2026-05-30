NAME = codexion

CC = cc
CFLAGS = -g -lpthread


SRCS = main.c \
	   utils/parsing.c \
	   utils/helper.c \



OBJS = $(SRCS:.c=.o)


HEADER = codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus