/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:57:34 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 21:03:46 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# define ECHOCTL 0001000

/*
** Global variable g_exit_code is used to track signal-triggered exits
** across the entire program, particularly for signal handling in minishell
*/
extern int				g_exit_code;

# define BUILTIN "echo:pwd:cd:export:unset:env:exit"
# define METACHAR "\t:\n:|:&:;:(:):<:>"
# define REDIRECTION "|:<:>:<<:>>"

# define PARENT 0
# define CHILD 1

# define BUFFER_SIZE 1024
# define INVALID 0
# define COMMAND 1
# define PIPE 2
# define INPUT_REDIRECT 3
# define OUTPUT_REDIRECT 4
# define HEREDOC 5
# define APPEND 6
# define WORD 7
# define END 8

# define NODE_COMMAND 0
# define NODE_ARGUMENT 1
# define NODE_REDIRECTION 2
# define NODE_TARGET 3
# define NODE_PIPE 4

# define LEFT 0
# define RIGHT 1

typedef struct s_ast	t_ast;

typedef struct s_token_info
{
	char				*value;
	int					was_quoted;
}						t_token_info;

typedef struct s_token
{
	char				*value;
	int					type;
	int					was_quoted;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_qtstate
{
	bool				single_open;
	bool				double_open;
	int					i;
	char				first_quote;
}						t_qtstate;

typedef struct s_env
{
	char				**env;
	char				**export;
	int					error_code;
	pid_t				last_pid;
}						t_env;

typedef struct s_extract_state
{
	const char			*p;
	char				*token;
	int					i;
	int					in_single_quote;
	int					in_double_quote;
	t_token_info		info;
}						t_extract_state;

typedef struct s_ast
{
	int					type;
	char				*value;
	t_ast				*left;
	t_ast				*right;
	t_ast				**children;
	int					child_count;
	t_ast				*target;
	t_ast				*parent;
	t_env				*env;
}						t_ast;

typedef struct s_pipe_data
{
	int					*fd;
	pid_t				*pids;
	int					input_fd;
	int					output_fd;
	t_ast				**head;
}						t_pipe_data;

typedef struct s_pipeline_data
{
	int					cmds;
	pid_t				*children;
	int					idx;
	int					cur_in;
	t_ast				*cur;
	int					pipefd[2];
}						t_pipeline_data;

typedef struct s_cmd_params
{
	int					input_fd;
	int					output_fd;
}						t_cmd_params;

typedef struct s_cleanup_params
{
	t_ast				**head;
	t_ast				**env;
	char				**tab;
	char				*path;
}						t_cleanup_params;

typedef struct s_segment
{
	const char			*s;
	int					start;
	int					len;
	int					word;
}						t_segment;

// ast
t_ast					*create_ast_node(int type, char *value);
void					add_ast_child(t_ast *parent, t_ast *child);
char					*get_cmd_path(const char *cmd, t_ast **env);
int						define_type(char *str, t_ast **env);
void					ast_print(t_ast *node, int indent);
void					ast_free(t_ast *node);
void					set_ast_left(t_ast *parent, t_ast *child);
void					set_ast_right(t_ast *parent, t_ast *child);
void					add_redirection(t_ast *cmd, t_ast *redir);
void					set_redirection_target(t_ast *redir, t_ast *target);
t_ast					*parse_command(t_token **lst_ptr, t_ast **env);
t_ast					*parse_pipeline(t_token **lst_ptr, t_ast **env);
t_ast					**combine(t_ast **head, t_ast *cmd);
void					free_split(char **split);
void					handle_backslash(t_extract_state *state);
void					handle_quote(t_extract_state *state, char quote_type);
t_ast					**build_and_print_ast(t_token *lst, t_ast **env);
void					handle_special_chars(t_extract_state *state);
int						should_break(t_extract_state *state);
void					init_extract_state(t_extract_state *state,
							const char *p);
char					**ft_split(char *s, const char *delim);
int						is_special_char(char c);
/* Functions from ft_split_utils.c */
int						is_delimiter(char c, const char *delim);
int						skip_spaces(const char *s, int i);
int						segment_length(const char *s, const char *delim, int i);
int						count_words(const char *s, const char *delim);
void					free_all(char **psplit);
/* End of ft_split_utils.c functions */
char					*ft_strdup(const char *s1);
size_t					ft_strlen(char const *src);
int						ft_strcmp(char *s1, char *s2);
int						check_type(char *str);
int						is_meta_character(char c);
int						contains_meta_character(char *str);
char					*ft_strjoin_buffer(char const *s1, char const *s2,
							char buffer);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t					ft_strlcat(char *dst, const char *src, size_t dstsize);
int						process_redirection_child(t_ast *child);
void					builtin_env(t_ast **env);
char					*env_get_entry(t_ast *env, char *key);
char					*env_get_value(t_ast *env, char *key);
char					*readline_open_quotes(char *str);
int						open_quotes(const char *str);
char					*clean_line(char *str, t_ast **env);
char					**ft_split_once_range(const char *s, char sep,
							int start, int end);
char					**ft_split_dollar_range(const char *s, int start,
							int end);
void					tab_to_file(char **lines, const char *filename);
int						tab_to_file_with_path(char **lines,
							const char *filename, char *temp_path_out,
							size_t path_size);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					*ft_calloc(size_t nmemb, size_t size);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strchr(const char *s, int c);
int						open_tempfile(const char *filename, char *temp_path,
							size_t path_size);
void					process_tokens(t_token **lst, char *line, t_ast **env);
char					*get_input(void);
void					infinite_read(t_token **lst, t_ast **env);
char					*get_value(char *var, int n, t_ast **env);
int						write_all_lines(int fd, char **lines);
void					link_or_copy_temp(const char *temp_path,
							const char *filename);
char					*unquoted_var_expansion(char *str, t_ast **env);
char					*expand_unquoted_var_at(char *str, int start, int len,
							t_ast **env);
char					*expand_variable(char *str, t_ast **env);
int						find_next_expand(const char *str, int *start, int *len);
char					*expand_one(const char *str, int start, int len,
							t_ast **env);
int						is_limiter_line(char *line, char *limiter);
char					*heredoc_readline(void);
char					*ft_strjoin_slash(char const *s1, char const *s2);
int						is_directory(const char *path);
int						is_var_char(char c);
int						is_var_char2(char c);
void					handle_quotes(const char *str, int i,
							int *in_single_quotes, int *in_double_quotes);
int						handle_var_expansion(char **str, int *i, t_ast **env,
							int in_single_quotes);
int						ft_strnstr(char *big, char *little);
int						check_redirection_without_file(t_token *lst);
int						check_invalid_combinations(t_token *lst);
void					show_list(t_token *list);
void					free_stack(t_token **stack);
int						ft_lstsize(t_token *lst);
void					ft_lstadd_back(t_token **lst, t_token *new, char *str);
void					ft_lstadd_back_with_quote_info(t_token **lst,
							t_token *new, int was_quoted);
int						create_list(t_token **start, char **str);
int						create_list_with_quote_info(t_token **start,
							t_token_info *tokens, int token_count);
t_token					*ft_lstnew(char *str);
t_token					*ft_lstnew_with_quote_info(char *str, int was_quoted);
t_token_info			extract_token_with_quote_info(const char **s);
void					execute_nodes(t_ast **head, t_ast **env);
void					execute_nodes2(t_ast **head, t_ast **env);
int						validate_command_exists(const char *cmd, t_ast **env);
void					exec_pipe_right(t_ast *node, t_ast **head, t_ast **env,
							t_pipe_data *data);
void					exec_pipe_left(t_ast *node, t_ast **head, t_ast **env,
							t_pipe_data *data);
void					exec_command_node(t_ast **node, t_ast **env,
							int input_fd, int output_fd);
void					exec_pipe_node(t_ast **head, t_ast **env, int input_fd,
							int output_fd);
void					exec_ast(t_ast **node, t_ast **env, int input_fd,
							int output_fd);
void					exec_cmd(t_ast **node, t_ast **env, int child);
void					exec2_exec_ast(t_ast **node, t_ast **head, t_ast **env,
							t_cmd_params *params);
int						apply_redirections(t_ast *node);
void					unlink_redirection(t_token **lst);

// Pour les test
void					pwd_recognition(t_ast **env);
void					env_recognition(char **tab, int j, t_ast **env);
char					*create_env_entry(char *name, char *value);
void					echo_recognition(char **argv, int i, t_ast **env);
void					cd_recognition(char **argv, int i, t_ast **env);
void					build_in(char **argv, int i, t_ast **env,
							t_cleanup_params *cleanup);
void					export_recognition(char **argv, int i, t_ast **env);
void					add_export(char *argv, t_ast **env);
void					show_env(t_ast **env);
void					unset_env(char *argv, t_ast **env);
void					unset_recognition(char **argv, int i, t_ast **env);
void					show_export(t_ast **env);
char					*cat_dup(char *s1);
int						parse_exp(char *argv);
int						check_dbl_equal(char *argv);
int						skip_isspace(char *argv);
void					add_env(char *argv, t_ast **env);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					unset_exp(char *argv, t_ast **env);
void					redirection(char **argv, int i);
void					output_recognition(char **argv, int i);
void					initialise_env(t_ast **env, char **envp);
void					initialise_exp(t_ast **env, char **envp);
int						cmd(char **tab, char *path, t_ast **env,
							t_cleanup_params *cleanup);
int						cmd_recognize(char *tab);
void					free_split(char **split);
void					pwd_change(char *pwd, char *oldpwd, t_ast **env);
void					initialise_shlvl(t_ast **env);
char					*number_shlvl(t_ast **env);
int						validate_shlvl_export(int value);
char					*create_validated_shlvl_entry(char *argv);
int						ft_atoi(const char *nptr);
char					*ft_itoa(int n);
void					print_error(int num, char *tab, t_ast **env);
int						access_error(char *tab);
int						ft_isdigit(int i);
int						search_value(char *str, t_ast **env);
void					exit_recognition(char **argv, int i, t_ast **env,
							t_cleanup_params *cleanup);
void					free_env_complete(t_ast *env);
void					valid_number_fail(t_ast **env, char *arg);
void					num_has_sign(t_ast **env);
void					free_both(char *target, t_ast *current);
int						tab_len(t_ast *current);
char					*path_var_set(t_ast *env, const char *key);
char					*full_path(char **paths, const char *cmd);
void					unset_exp_fnc(t_ast *current, char *target, char **temp,
							int j);
void					del_export(t_ast **env);
void					unset_env_fnc(t_ast *current, char *argv, char **temp,
							int j);
void					add_env_fnc(t_ast *current, char **temp, char *argv);
void					add_exp_fnc(t_ast *current, char **temp, char *argv);
char					*get_env_var(t_ast **env, char *str);
void					free_buffer(char *buffer, char *buffer2, t_ast **env);
void					cd_rec_fnc(char *tab, t_ast **env);
void					free_tab1(char *buffer, char *buffer2);
void					cd_only(t_ast **env);
void					free_env_env(t_ast *env);
void					number_has_sign(char **argv, int i, t_ast **env,
							t_cleanup_params *cleanup);
void					too_much_exit(t_ast **env);
void					number_not_valid(char **argv, int i, t_ast **env,
							t_cleanup_params *cleanup);
void					handle_external_command(char **tab, char *path,
							t_ast **env, t_ast **head);

// cmd1_utils.c
int						count_envp_entries(char **envp);
int						allocate_export_array(t_ast *current, int count);
int						copy_envp_entries(t_ast **env, char **envp);

// gnl
char					*ft_strjoin(char const *s1, char const *s2);
int						ft_newline(const char *str);
char					*ft_replace(char *buffer);
char					*ft_extract(char *buffer);
char					*ft_buffer(int fd, char *buffer);
char					*get_next_line(int fd);
void					cleanup_get_next_line(void);

// get_next_line_utils.c
void					cleanup_buffer(char **buffer);
int						validate_fd_and_cleanup(int fd, char **buffer);
char					*read_and_join(char *buffer, int fd, int *should_break);

// Heredoc
void					clean_heredoc(char **argv);
void					read_heredoc(char *limiter, int quoted_limiter,
							t_ast **env);
void					read_heredoc_consume_only(char *limiter);
void					free_tab(char **tab);
void					check_heredoc(t_token **lst, t_ast **env);
void					copy_tmp_to_file(const char *temp_path,
							const char *filename);
void					start_heredoc(char *limiter, int quoted_limiter,
							t_ast **env);

void					setup_sigint_handler(void);
void					setup_sigquit_handler(void);
void					disable_echoctl(void);
void					setup_heredoc_signals(void);
void					handle_sigint_heredoc(int sig_num);
void					restore_parent_signals(void);
char					**split_quote_aware(const char *s, const char *delims);
char					**split_bash_style(const char *input);
t_token_info			*split_bash_style_with_quotes(const char *input,
							int *token_count);
void					exit_status(t_token **lst, t_ast **env);
int						check_syntax_errors(t_token *lst);
void					ft_putstr_fd(char *s, int fd);
void					print_exit_code(t_ast **env);
void					handle_errno_error(const char *path);
void					command_not_found_error(const char *cmd);
void					exit_child(int exit_code, int child);
t_ast					*parse_pipeline(t_token **lst_ptr, t_ast **env);
void					free_tokens(char **tokens);
void					free_token_info_array(t_token_info *tokens, int count);
void					setup_shell_signals(void);
void					setup_child_signals(void);
void					ignore_signals_during_execution(void);
void					restore_parent_signals(void);
// exec2 helper functions
void					free_ast_full(t_ast **head);
void					free_argv_shallow(char **tab);
void					cleanup_and_exit(int status, t_cleanup_params *params);
void					redirect_io2(int input_fd, int output_fd);
int						count_pipeline_cmds(t_ast *node);
void					run_pipeline_loop(t_ast **head, t_ast **env,
							t_pipeline_data *data);
void					wait_for_children(t_pipeline_data *data, pid_t last_pid,
							t_ast **env);
void					wait_for_all_pipeline_children(int cmd_count,
							pid_t last_pid, t_ast **env);
void					run_command(t_ast *node, t_ast **head, t_ast **env,
							t_cmd_params *params);
char					**build_argv(t_ast *node);
int						apply_redirections2(t_ast *node);
int						is_input_redirection(t_ast *node);
int						is_heredoc_redirection(t_ast *node);
int						find_last_heredoc_redirection(t_ast *node);
int						process_single_redirection(t_ast *node, int i,
							int last_heredoc_redir);
void					handle_command_execution(t_ast *node, t_ast **head,
							t_ast **env, t_cmd_params *params);
void					execute_builtin_command(t_ast *node, t_ast **head,
							t_ast **env, t_cmd_params *params);

// exec2_command_helpers.c
void					write_error_msg(const char *cmd, const char *msg);
void					print_exec_error(const char *cmd, int code,
							int has_slash);
int						classify_error(const char *cmd, char *resolved);
void					handle_empty_cmd_fork(t_ast *node, t_ast **head,
							t_ast **env, t_cmd_params *params);

// String utilities
void					build_temp_path(char *temp_path, size_t path_size,
							const char *filename, int pid);
void					build_pattern(char *pattern, size_t size,
							const char *trimmed_limiter);
void					ft_strcpy_safe(char *dst, const char *src, size_t size);
void					ft_strcat_safe(char *dst, const char *src, size_t size);

// Heredoc file finder
char					*find_heredoc_file(const char *limiter);

// Heredoc path utilities
void					build_prefix(const char *limiter, char *prefix,
							size_t size);
void					build_fullpath(const char *name, char *full,
							size_t size);

// Redirection helpers
int						print_redir_error(const char *path);
int						open_output_redir(t_ast *redir);
int						open_input_redir(t_ast *redir);
int						handle_input_redirection(t_ast *child);
int						handle_output_redirection(t_ast *child);

// Command error utilities
int						classify_error(const char *cmd, char *resolved);

// Validation utilities
int						validate_command_exists(const char *cmd, t_ast **env);

// Split utilities
char					**split_by_newline(char *str);
char					**split_newline_alloc(char *str, int line_count);
char					**finish_last_line(char *str, char **result, int j,
							int start);

// Heredoc readline utilities
void					maybe_expand_line(char **line, int quoted_limiter,
							t_ast **env);
void					append_line(char **acc, char *line);
void					finalize_heredoc(char *str, char *filename);

// Heredoc check utilities
void					check_heredoc(t_token **lst, t_ast **env);

// Cleanup functions
void					cleanup_readline_resources(void);

// CMD2 Exit helpers
int						is_valid_number(char *str);
int						check_has_sign(char *str);
void					exit_number_fail(char *str);

// CMD4 Export helpers
void					process_export_args(char **argv, int *i, t_ast **env,
							int *invalid);
void					create_initial_env(char *argv, t_ast **env);

// CMD6 CD helpers
int						handle_home_directory(t_ast **env);
int						change_to_path(char *path);
int						handle_specific_path(char *path, char *resolved_path);

// Environment helpers
void					set_env_var(t_ast *env, char *name, char *value);
void					update_pwd_at_startup(t_ast *env);
void					update_pwd_after_cd(t_ast *env, char *old_pwd);
void					initialise_env(t_ast **env, char **envp);
int						validate_shlvl_export(int value);
char					*create_validated_shlvl_entry(char *argv);
void					execute_external_command(t_ast *node, t_ast **head,
							t_ast **env, t_cmd_params *params);

// PRE PARSING UTILS HUGO
int						count_total_quotes(char *line);
int						count_sing_quotes(const char *line);
int						count_double_quotes(const char *line);
void					init_qt_state(t_qtstate *state);
void					update_quotes_state(t_qtstate *state, char c);

// PRE PARSING HUGO
bool					return_quotes_error(char *line);
bool					too_much_redir(char *line);

#endif