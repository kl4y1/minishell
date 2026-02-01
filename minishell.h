#ifndef MINISHELL_H
# define MINISHELL_H
# define _POSIX_C_SOURCE 200809L

//prompt color
#define C_PURPLE "\001\033[35m\002"
#define C_RESET  "\001\033[0m\002"

#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termcap.h>

//global var decl
extern volatile sig_atomic_t g_signal;

typedef enum e_toktype
{
	WORD,
	PIPE,        //|
	R_IN,    //<
	R_OUT,   //>
	APPEND,      //>>
	HEREDOC      //<<
}	t_toktype;

typedef struct s_token
{
	t_toktype        type;
	char            *value;   //only for WORD and filenames || limiters after redirects
	struct s_token  *next;
}	t_token;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_redir
{
    t_toktype              type;       
    char                *target;     //filename or heredoc limiter
    char                *heredoc_tmp;//path to temp file if heredoc
    struct s_redir      *next;
}   t_redir;

typedef struct s_cmd
{
    char            **argv;      //["echo","hi",NULL]
    t_redir          *redirs;
    struct s_cmd     *next;      //pipeline chain
}   t_cmd;

typedef struct s_pid
{
	pid_t pid;
	pid_t last_pid;
	struct s_pid 	*next;
}	t_pid;

//signals
void fresh_screen(void);
void prepare_sig(void);
void sig_ctrl(int sig);
//utils
void	free_tokenlist(t_token *list);
void	closefds(int fd1, int fd2);
void	waitpids(pid_t child1, pid_t child2);
int		zspace(char c);
int		zchar(char c);
//path
char	**get_paths(char **envp);
char	*find_exec(char *cmd, char **paths);
char	*join_path(char *path, char *cmd);
void	freesplit(char **array);
//errors
void	exec_path_error(char **cmd, char **paths);
void	error_execve(char **cmd, char *execpath);
void	exit_if_error(char *error);
void	exit_usage(char *message);
//exec
void	exec_command(char **cmd, char **envp);
void	paf_child_process(t_cmd *node, char **envp, int w_fd);
pid_t	paf(t_cmd *node, char **envp);
pid_t	exec_final_command(t_cmd *node, char **envp);
//token
t_token	*tokenizer(char *line);
t_token *newtoken(t_toktype type, char *value);
void	addtoken(t_token **lst, t_token *new);
//parser
t_cmd	*parse_line(char *line);
void	free_cmds(t_cmd *cmds);
//builtins
int		isbuiltin(char *s);
int		checkflag(char *s);
int		our_echo(char **argv);
int		our_pwd(void);
int		our_env(t_env *env);
int		valid_ident(char *s);
void	unset_key(t_env **env, char *key);
int		our_unset(char **argv, t_env **env);
int		our_cd(char **argv, t_env **env);
int		our_export(char **argv, t_env **env);
int		our_exit(char **argv);
//pipework
void	free_pid_list(t_pid **pid_list);
int		wait_pids(t_pid *pid_list, pid_t last_pid);
int		pipeline(t_cmd *cmds, char **env);
//pipeworkutils
t_cmd	*getg_last_cmd(t_cmd *cmd);
pid_t	get_last_cmd(t_cmd *cmd, char **env, t_pid **pid_list);
void	get_middle_cmds(t_cmd *cmds, char **env, t_pid **pid_list);
void	do_redirs(t_redir *redir);
void	pid_add_back(t_pid **pid_list, t_pid *node);
t_pid	*pid_node(pid_t pid);


#endif
