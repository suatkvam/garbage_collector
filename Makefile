NAME = garbage_collecter.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
GC_FLAGS = -DUSE_GC_WRAP

WRAP_FLAGS = -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free

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

normal: all

gc: CFLAGS += $(GC_FLAGS)
gc: all
	@printf '$(GREEN)Built with GC wrapping enabled!$(RESET)\n'

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf '$(GREEN)Compiling %s...$(RESET)\n' "$<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@printf '$(YELLOW)Creating static library %s$(RESET)\n' "$(NAME)"
	$(AR) -rcs $(NAME) $(OBJS)
	@printf '$(GREEN)Build complete!$(RESET)\n'

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@printf '$(RED)Cleaned object files.$(RESET)\n'

fclean: clean
	@$(RM) $(NAME)
	@printf '$(RED)Cleaned library files.$(RESET)\n'

re: fclean all

# ✅ ADDED: Help target
help:
	@printf '$(BLUE)╔════════════════════════════════════════╗$(RESET)\n'
	@printf '$(BLUE)║  Garbage Collector - Build Targets    ║$(RESET)\n'
	@printf '$(BLUE)╚════════════════════════════════════════╝$(RESET)\n'
	@printf '\n'
	@printf '$(YELLOW)Available targets:$(RESET)\n'
	@printf '  $(GREEN)make$(RESET) or $(GREEN)make normal$(RESET)\n'
	@printf '    Build with standard malloc/free\n'
	@printf '\n'
	@printf '  $(GREEN)make gc$(RESET)\n'
	@printf '    Build with garbage collector (-DUSE_GC_WRAP)\n'
	@printf '    Enables automatic memory management\n'
	@printf '\n'
	@printf '  $(GREEN)make clean$(RESET)\n'
	@printf '    Remove object files\n'
	@printf '\n'
	@printf '  $(GREEN)make fclean$(RESET)\n'
	@printf '    Remove all generated files\n'
	@printf '\n'
	@printf '  $(GREEN)make re$(RESET)\n'
	@printf '    Rebuild from scratch\n'
	@printf '\n'
	@printf '$(YELLOW)Linking your program:$(RESET)\n'
	@printf '  $(GREEN)Normal mode:$(RESET)\n'
	@printf '    gcc your_code.c -L. -lgarbage_collecter -o program\n'
	@printf '\n'
	@printf '  $(GREEN)GC mode:$(RESET)\n'
	@printf '    gcc -DUSE_GC_WRAP your_code.c -L. -lgarbage_collecter \\\n'
	@printf '        -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free \\\n'
	@printf '        -o program\n'
	@printf '\n'

.PHONY: all normal gc clean fclean re help