/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:31:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 01:04:34 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# define C_PURPLE "\001\033[35m\002"
# define C_RESET "\001\033[0m\002"
# define HEREDOC_MAX_TRIES 1000000

# include "libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <unistd.h>
# include <readline/history.h>
# include <errno.h>

extern volatile sig_atomic_t	g_signal;

typedef enum e_toktype
{
	WORD,
	PIPE,
	R_IN,
	R_OUT,
	APPEND,
	HEREDOC
}								t_toktype;

typedef struct s_token
{
	t_toktype					type;
	char						*value;
	struct s_token				*next;
}								t_token;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_hd
{
	int							fd;
	int							res;
	char						*path;
	char						*clean;
	struct sigaction			old_int;
	struct sigaction			old_quit;
}								t_hd;

typedef struct s_redir
{
	t_toktype					type;
	char						*target;
	char						*heredoc_tmp;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						**argv;
	t_redir						*redirs;
	struct s_cmd				*next;
}								t_cmd;

typedef struct s_pid
{
	pid_t						pid;
	pid_t						last_pid;
	struct s_pid				*next;
}								t_pid;

typedef struct s_exp
{
	t_env						*env;
	int							last_stat;
	int							i;
	int							quote;
}								t_exp;

typedef struct s_xtok
{
	t_token						**tokens;
	t_token						*prev;
	t_token						*cur;
	t_env						*env;
	int							last_stat;
	int							had_quotes;
	char						**words;
	int							wcount;
}								t_tok;

typedef int						(*t_bfn)(char **argv, struct s_env **env,
									int last_status);

typedef struct s_bentry
{
	char	*name;
	t_bfn	fn;
}	t_bentry;

void							fresh_screen(void);
void							prepare_sig(void);
void							reset_signals(void);
void							sig_ctrl(int sig);
void							free_tokenlist(t_token *list);
void							closefds(int fd1, int fd2);
void							waitpids(pid_t child1, pid_t child2);
int								zspace(char c);
int								zchar(char c);
char							**get_paths(char **envp);
char							*find_exec(char *cmd, char **paths);
char							*join_path(char *path, char *cmd);
void							freesplit(char **array);
void							exec_path_error(char **cmd, char **paths);
void							error_execve(char **cmd, char *execpath);
void							exit_if_error(char *error);
void							exit_usage(char *message);
void							close_extra_fds(void);
void							execve_fail(char *cmd0, char **envp,
									char *exec_path, t_env *env);
void							exec_command(char **cmd, t_env **env,
									int lasts);
int								has_slash(char *s);
char							*load_exec_path(char **cmd, t_env **env,
									char ***paths, char ***envp);
pid_t							paf(t_cmd *node, t_env **env, int lasts);
pid_t							exec_final_command(t_cmd *node, t_env **env,
									int lasts);

char							*do_expand(char *s, t_env *env, int last_stat,
									int *had_quotes);
int								expand_tokens(t_token **tokens, t_env *env,
									int last_stat);
int								varname_len(char *s);
char							*get_var_val(char *name, t_env *env,
									int last_stat);
char							*append_str(char *res, char *add);
char							*append_char(char *res, char c);
char							*exp_append_splitable(char *res, char *s);
char							*exp_handle_backslash(char *s, char *res,
									t_exp *e);
int								tok_load_words(t_tok *x);
int								tok_handle_zero(t_tok *x);
int								tok_handle_single(t_tok *x);
int								tok_handle_multi(t_tok *x);
int								tok_step(t_tok *x);
t_token							*tok_free_list(t_token *list);
t_token							*tok_build_extra(t_tok *x);
t_token							*tok_tail(t_token *node);
int								tok_set_empty(t_tok *x);
int								tok_set_first(t_tok *x, char *word);
void							tok_link_extra(t_tok *x, t_token *extra);
void							tok_init(t_tok *x, t_token **tokens, t_env *env,
									int last_stat);

t_token							*tokenizer(char *line);
t_token							*newtoken(t_toktype type, char *value);
void							addtoken(t_token **lst, t_token *new);
int								validate_tokens(t_token *tokens);
t_cmd							*parse_line(char *line, t_env *env,
									int last_stat);
void							free_cmds(t_cmd *cmds);
int								builtin(char **argv, t_env **env,
									int last_status);
int								is_builtin_cmd(char *cmd);
t_bentry						*builtin_table(void);
t_bfn							find_builtin(char *cmd);
int								b_cd(char **argv, t_env **env,
									int last_status);
int								b_export(char **argv, t_env **env,
									int last_status);
int								b_exit(char **argv, t_env **env,
									int last_status);
int								checkflag(char *s);
int								our_echo(char **argv);
int								our_pwd(void);
int								our_env(t_env *env);
int								valid_ident(char *s);
void							unset_key(t_env **env, char *key);
int								our_unset(char **argv, t_env **env);
int								our_cd(t_env *env, char **argv);
int								our_export(char **argv, t_env **env);
int								export_valid_ident(char *arg);
char							*export_key(char *arg);
int								export_add_new(char *key, char *arg,
									t_env **env);
int								env_count(t_env *env);
void							sort_env_arr(t_env **arr, int n);
void							print_export(t_env *env);
int								export_error(char *arg);
int								process_arg(char *arg, t_env **env);
int								our_exit(char **argv, int last_status);
void							printnonnumer(char *s);
char							*find_key(char *s);
t_env							*env_find(t_env *env, const char *key);
void							env_add_back(t_env **env, t_env *new);
t_env							*new_node(char *s);
void							update_env(t_env *existing, char *arg);
void							free_pid_list(t_pid **pid_list);
int								wait_pids(t_pid *pid_list, pid_t last_pid,
									int *last_stat);
int								pipeline(t_cmd *cmds, t_env **env,
									int *last_stat);
t_cmd							*getg_last_cmd(t_cmd *cmd);
pid_t							get_last_cmd(t_cmd *cmd, t_env **env,
									t_pid **pid_list, int last_stat);
void							get_middle_cmds(t_cmd *cmds, t_env **env,
									t_pid **pid_list, int last_stat);
int								do_redirs(t_redir *redir);
char							*make_heredoc(char *limiter);
void							hd_sigint(int sig);
void							hd_set_signals(struct sigaction *old_int,
									struct sigaction *old_quit);
void							hd_restore_signals(struct sigaction *old_int,
									struct sigaction *old_quit);
void							hd_cleanup(t_hd *hd);
void							pid_add_back(t_pid **pid_list, t_pid *node);
t_pid							*pid_node(pid_t pid);
void							env_add_front(t_env **env, t_env *new);
void							env_free(t_env *env);
void							env_free_one(t_env *n);
t_env							*env_new_node(char *s);
t_env							*envptoenv(char **envp);
char							**listtoarr(t_env *env);
void							free_arr(char **arr);
int								count_args(char **av);
void							add_redir(t_cmd *cmd, t_toktype type,
									char *target, char *hd_tmp);
int								handle_redir(t_cmd *cmd, t_token **tok);
void							restore_fds(int save_in, int save_out);
int								exec_cmds(t_cmd *cmds, t_env **env,
									int *last_stat);
int								process_command(char *shell, t_env **env,
									int *last_stat);
int								main_loop(t_env *env, int *status);
void							handle_input(char *shell, int *last_stat);
t_cmd							*parse_input(char *shell, t_env *env,
									int *last_stat);
int								endofword2(char *s, int i, int quote);
int								endofword(char *s, int i);
char							*nodevalue(t_toktype type, char *s, int *i);
t_toktype						ident(char *s, int *i);
t_token							*handle_word_token(char *line, int *i,
									t_token *tokenlist);
t_token							*tokenizer2(char *line);
t_cmd							*parse_line2(char *line, t_env *env,
									int last_stat);
t_cmd							*new_cmd(void);
void							add_argv(t_cmd *cmd, char *arg);
t_cmd							*parse_tokens2(t_token *tokens);
int								validate_tokens2(t_token *tokens);

#endif
