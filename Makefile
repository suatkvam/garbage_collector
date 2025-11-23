NAME = garbage_collecter.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
GC_FLAGS = -DUSE_GC_WRAP

# Linker wrap flags (Linux/MinGW)
#! bu kaldırılabilir belki
ifeq ($(OS),Windows_NT)
	WRAP_FLAGS = -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free
else
	WRAP_FLAGS = -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free
endif

AR = ar
RM = rm -f

# ANSI color codes
GREEN := \033[0;32m
YELLOW := \033[1;33m
BLUE := \033[1;34m
RED := \033[0;31m
RESET := \033[0m

SRC = collector_close.c \
      collector_init.c \
      gc_calloc.c \
      gc_collect.c \
      gc_malloc.c \
      gc_mark.c \
      gc_mark_utils.c \
      gc_memory_utils.c \
      gc_realloc.c \
      gc_state.c \
      gc_sweep.c \
      gc_wrap.c \
      get_header_from_ptr.c

SRCS = $(SRC)
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf '$(GREEN)Compiling %s...$(RESET)\n' "$<"
	@$(CC) $(CFLAGS) $(GC_FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@printf '$(YELLOW)Creating static library %s$(RESET)\n' "$(NAME)"
	$(AR) -rcs $(NAME) $(OBJS)

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@printf '$(RED)Cleaned object files.$(RESET)\n'

fclean: clean
	@$(RM) $(NAME)
	@printf '$(RED)Cleaned library files.$(RESET)\n'

re: fclean all

.PHONY: all clean fclean re