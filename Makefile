NAME = pipex

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

FILES = main \
		fork_exec_cmd \
		pipe_while \
		utils

SRCS_DIR = src

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))

OBJS_DIR = .build

OBJS = $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

INCLUDES_DIR = includes

INCLUDES_FILES = pipex

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(addsuffix .h, $(INCLUDES_FILES)))

LIBS_DIR = libs

LIBLP_DIR = $(LIBS_DIR)/liblp_c

LIBLP = $(LIBLP_DIR)/liblp.a

$(NAME): $(LIBLP) $(OBJS_DIR) $(OBJS)
	$(CC) $(OBJS) $(LIBLP) -o $(NAME)

$(LIBLP):
	@make -C ${LIBLP_DIR}

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDES_DIR)

all: $(NAME)

clean: cleanlibs
	rm -rf $(OBJS_DIR)

cleanlibs:
	make fclean -C ${LIBLP_DIR}

fclean: clean
	rm -f $(NAME)

re: clean all

bonus: all

.PHONY: all clean fclean re cleanlibs