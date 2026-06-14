NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread

SRCS = main.c \
	   utils/parsing.c \
	   utils/helper.c \
	   utils/monitor.c \
	   utils/free_all.c \
	   init_all/dongles_allocation.c \
	   init_all/world_data.c \
	   init_all/coders_alllocation.c \
	   coders/coders_action.c \
	   coders/dongles_action.c \
	   coders/coders.c \
	   scheduler/fifo.c \
	   scheduler/edf.c \
	   scheduler/scheduler.c




OBJS = $(SRCS:.c=.o)


HEADER = codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lpthread


%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

fun:
	@make re CC=x86_64-linux-gnu-gcc CFLAGS="$(filter-out -m64,$(CFLAGS))"

re: fclean all


.PHONY: all clean fclean re bonus