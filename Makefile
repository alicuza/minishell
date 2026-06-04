# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/22 21:29:56 by sancuta           #+#    #+#              #
#    Updated: 2026/06/04 20:56:08 by nribakov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---- project ------------------------------------------------------------- #
NAME       = minishell

# ---- compiler / linker --------------------------------------------------- #
CC         = cc
CFLAGS     = -Wall -Wextra -Werror -MMD -MP
ARENA_SIZE ?= 64
CPPFLAGS   = -I inc -I $(LIBFT_DIR) -I $(LIBFT_DIR)/arena -D ARENA_SIZE=$(ARENA_SIZE)
LDFLAGS    =
LDLIBS     = -lreadline

# ---- build directories --------------------------------------------------- #
RELEASE_DIR = build/release
DEBUG_DIR   = build/debug

# ---- libraries ----------------------------------------------------------- #
LIBFT_DIR     = libs/libft
LIBFT_RELEASE = $(LIBFT_DIR)/libft.a
LIBFT_DEBUG   = $(DEBUG_DIR)/libft.a

# ---- sources ------------------------------------------------------------- #
SRCS        = \
			main.c \
			prompt.c \
			input.c \
			token.c \
			token_utils.c \
			token_char.c \
			token_transform.c \
			env/env_utils.c \
			token_processor/token_processor.c \
			builtin/env.c
			
DEBUG_SRCS  = debug_utils.c

RELEASE_OBJS = $(addprefix $(RELEASE_DIR)/, $(SRCS:.c=.o))
DEBUG_OBJS   = $(addprefix $(DEBUG_DIR)/, $(SRCS:.c=.o)) \
                $(addprefix $(DEBUG_DIR)/, $(DEBUG_SRCS:.c=.o))

# ---- source lookup ------------------------------------------------------- #
vpath %.c srcs debug_srcs

# ---- build flags --------------------------------------------------------- #
RELEASE_FLAGS = -O2
DEBUG_FLAGS   = -O0 -g3 -DDEBUG

# ---- targets ------------------------------------------------------------- #
all: $(NAME)

$(NAME): $(RELEASE_OBJS) $(LIBFT_RELEASE)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

debug: $(NAME)-debug

$(NAME)-debug: $(DEBUG_DIR)/$(NAME)
	ln -sf $(DEBUG_DIR)/$(NAME) $@

$(DEBUG_DIR)/$(NAME): $(DEBUG_OBJS) $(LIBFT_DEBUG)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# ---- object compilation -------------------------------------------------- #
$(RELEASE_DIR)/%.o: BUILD_FLAGS = $(RELEASE_FLAGS)
$(DEBUG_DIR)/%.o:   BUILD_FLAGS = $(DEBUG_FLAGS)

$(RELEASE_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(BUILD_FLAGS) -c $< -o $@

$(DEBUG_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(BUILD_FLAGS) -c $< -o $@

# ---- libft --------------------------------------------------------------- #
$(LIBFT_RELEASE): FORCE
	$(MAKE) -C $(LIBFT_DIR)

$(LIBFT_DEBUG): FORCE
	$(MAKE) -C $(LIBFT_DIR) debug
	cp -u $(LIBFT_RELEASE) $@

FORCE:

# ---- utility targets ----------------------------------------------------- #
run: $(NAME)
	./$(NAME)

run-debug: $(NAME)-debug
	./$(NAME)-debug

doc:
	printf '%s\n' \
	  'OUTPUT_DIRECTORY = docs' \
	  'GENERATE_MAN = YES' \
	  'MAN_OUTPUT = man' \
	  'INPUT = inc srcs debug_srcs' \
	  'EXTRACT_ALL = YES' \
	  'OPTIMIZE_OUTPUT_FOR_C = YES' \
	  > docs/Doxyfile
	doxygen docs/Doxyfile

compile_flags.txt:
	@echo setup $@
	@echo -n > $@
	@for flag in $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS); do \
		echo $$flag >> $@ ; \
	done

test: $(NAME)
	./test/runner.sh

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf build docs/Doxyfile

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(NAME)-debug
	rm -rf docs
	rm -f compile_flags
	rm -rf log

re: fclean all

# ---- phony targets ------------------------------------------------------- #
.PHONY: all debug run run-debug doc test clean fclean re FORCE

# ---- dependencies -------------------------------------------------------- #
-include $(RELEASE_OBJS:.o=.d)
-include $(DEBUG_OBJS:.o=.d)
