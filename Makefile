# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Program name
NAME = minishell

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCS = -I includes
LDLIBS = -lreadline

# Colors for output
GREEN = \033[0;92m
BLUE = \033[0;94m
CYAN = \033[0;96m
YELLOW = \033[0;93m
WHITE = \033[0;37m

# Source files (all with correct relative paths and .c extensions)
SRC = main.c utils.c utils2.c process_tokens.c quotes.c libft.c libft2.c libft3.c libft4.c  expand2.c handle_quotes.c \
	ft_split.c ft_split_utils.c ft_split2.c ft_itoa.c expand.c chained_list.c chained_list2.c string_utils.c path_utils.c \
	exit_status.c bash_split.c bash_split2.c bash_split3.c syntax_check.c syntax_check2.c \
	heredoc/get_next_line.c heredoc/get_next_line_utils.c heredoc/heredoc.c heredoc/heredoc_utils.c heredoc/heredoc_utils2.c heredoc/heredoc_file_writer.c heredoc/heredoc_check_utils.c heredoc/heredoc_utilts3.c heredoc/split_newline.c heredoc/heredoc_readline_utils.c quotes2.c heredoc/read_heredoc.c \
	execution_posix/exec.c execution_posix/exec_utils.c execution_posix/exec_validation_utils.c execution_posix/exec_redirection.c execution_posix/exec_redirection_helpers.c execution_posix/heredoc_file_finder.c execution_posix/heredoc_path_utils.c \
	cmd/cmd1.c cmd/cmd1_utils.c cmd/cmd2.c cmd/cmd2_exit_helpers.c cmd/cmd3.c cmd/cmd4.c cmd/cmd4_export_helpers.c cmd/cmd5.c cmd/cmd6.c cmd/cmd6_cd_helpers.c cmd/cmd7.c cmd/cmd8.c cmd/cmd9.c cmd/cmd10.c cmd/cmd11.c \
	ast/new_ast.c ast/new_ast2.c ast/new_ast3.c ast/new_ast4.c ast/new_ast5.c signal/signal.c signal/heredoc_signals.c \
	execution_posix/exec2.c execution_posix/exec2_utils.c execution_posix/exec2_pipeline.c execution_posix/exec2_wait.c \
	execution_posix/exec2_command.c execution_posix/exec2_command_helpers.c execution_posix/exec2_command_error_utils.c execution_posix/exec2_main.c

# Object files, with obj/ prefix and .o extension
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Directories needed for object files (unique and sorted)
DIRS := $(sort $(dir $(OBJS)))

# Default target
all: $(NAME)

# Link objects into archive/library
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	@echo "$(GREEN)minishell compiled successfully!$(WHITE)"

# Create directories for object files
$(DIRS):
	@mkdir -p $@

# Compile .c files to .o files, ensure directory exists first
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@echo "$(CYAN)Compiled: $<$(BLUE)"

# Clean object files and directories
clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)minishell object files cleaned!$(BLUE)"

# Clean everything including executable
fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)minishell executable files removed!$(BLUE)"

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re