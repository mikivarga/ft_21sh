/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_21sh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 14:17:30 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 18:54:07 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_21SH_H
# define FT_21SH_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <termios.h>
# include <term.h>
# include <sys/ioctl.h>
# include <errno.h>

# define TRUE 1
# define FALSE 0
# define WARNING -1
# define BUF 2048

# define ESC 0x1B
# define KEY_UP 0x415B
# define KEY_DWN 0x425B
# define KEY_RGHT 0x435B
# define KEY_LFT 0x445B
# define KEY_DEL 0x7E335B
# define KEY_BACKSP 0x7F
# define KEY_ALT_LFT 0x445b1b
# define KEY_ALT_RGHT 0x435b1b
# define KEY_ALT_UP 0x415b1b
# define KEY_ALT_DWN 0x425b1b
# define KEY_ALT_BEG 0x48393b315b
# define KEY_ALT_END 0x46393b315b

# define APPEND 0x01
# define AGRE 0x02

typedef struct winsize		t_win;

typedef struct				s_cursor
{
	int						x_prom;
	int						x;
	int						y;
}							t_cursor;

typedef struct				s_lst
{
	unsigned char			item;
	struct s_lst			*next;
	struct s_lst			*prev;
}							t_lst;

typedef struct				s_leters
{
	t_lst					*beg;
	t_lst					*cur;
	t_lst					*end;
	int						cnt;
}							t_leters;

typedef struct				s_history
{
	t_leters				*hist_cmd;
	struct s_history		*next;
	struct s_history		*prev;
}							t_history;

typedef struct				s_cmd_hist
{
	t_history				*beg;
	t_history				*cur;
	t_history				*end;
	int						cnt;
}							t_cmd_hist;

typedef struct				s_term
{
	int						fd_tty;
	int						col;
	int						lin;
	t_cmd_hist				hist;
	t_cursor				poz;
	struct termios			new;
	struct termios			old;
	char					buf[BUF];
	char					**arg;
	char					**env;
	char					*fin;
	char					*fout;
	int						fd[2];
	int						fd_agr;
	int						rout;
	char					*firstfound;
}							t_term;

void						ft_setcwd(char *dir, t_term *cmd);
void						ft_new_env(t_term *cmd, char **env);
char						*ft_get_env(char *name, t_term *env);
void						ft_setenv(char *name, char *value, t_term *cmd);
void						ft_unsetenv(char *name, t_term *cmd);
int							ft_envname_cmp(char *name, char *envstr);
void						ft_show_env(char **env);
void						ft_free(char **pp);
void						ft_free_hist_str(t_term *t);
void						ft_free_hist(t_term *t);

void						ft_show_prom(t_term *term);
int							ft_put(int c);
void						ft_open_visual_mode(t_term *term);
void						ft_close_visual_mode(t_term *term);

void						ft_init_history(t_term *phist);
void						ft_add_history(t_cmd_hist *phist);
void						ft_ch_hist(t_term *t, t_cursor *poz);
void						ft_check_hist(t_term *t, t_cursor *poz);
void						ft_add_letters_end\
(t_leters *plst, unsigned char c);
void						ft_add_letters_middle\
(t_leters *plst, unsigned char c);

int							ft_read_input(t_term *term);
void						ft_add_let(t_term *term, unsigned char key);
void						ft_show_lst_str(t_leters *plet, t_term *term);
void						ft_show_str(t_leters *plet, t_term *term);
void						ft_windsize(t_term *term);
void						ft_poz_x_y(t_term *term);
void						ft_key_lft(t_term *term);
void						ft_key_l(t_term *term);
void						ft_key_rght(t_term *term);
void						ft_key_r(t_term *term);
void						ft_key_del(t_term *term);
void						ft_let_del(t_term *term);
void						ft_key_up_dw(t_term *term, unsigned long key);
void						ft_alt_beg_end(t_term *term, unsigned long key);
void						ft_alt_rght(t_term *term);
void						ft_alt_lft(t_term *term);
void						ft_alt_up_dwn(t_term *term, unsigned long key);
void						ft_show_hist(t_term *term);

void						ft_save_buf(t_term *t, char *buf);
int							ft_check(t_term *term);
int							ft_check_cmd(char *str, int fl, t_term *term);
int							ft_isspace(char c);
int							ft_new_arg(char c);

int							ft_parser(t_term *cmd, char *buf);
int							ft_parse_pipe(t_term *env, char **arr);
int							ft_redirection(t_term *env, char **arr);
char						**ft_redirections(char **str, t_term *cmd);
int							ft_input(t_term *env);
char						**ft_heredoc(char **str, t_term *term);
int							ft_output(t_term *env);
int							ft_select_cmd(char **arr, t_term *env, int flag);
void						ft_echo(char **arr);
void						ft_cd(char **arr, t_term *env);
#endif
