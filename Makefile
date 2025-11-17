
NAME = garbage_collecter.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar
RM = rm -f

# ANSI color codes
GREEN := \033[0;32m
YELLOW := \033[1;33m
BLUE := \033[1;34m
RESET := \033[0m

SRC =

SRCS = $(SRC)
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf '$(GREEN)Compiling %s...$(RESET)\n' "$<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@printf '$(YELLOW)Creating static library %s$(RESET)\n' "$(NAME)"
	$(AR) -rcs $(NAME) $(OBJS)

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@printf '$(BLUE)Cleaned object files.$(RESET)\n'

fclean: clean
	@$(RM) $(NAME)
	@printf '$(BLUE)Cleaned library files.$(RESET)\n'

re: fclean all

.PHONY: all clean fclean re
