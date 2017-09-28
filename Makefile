#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvarga <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/08/13 12:05:56 by mvarga            #+#    #+#              #
#    Updated: 2017/08/13 12:15:32 by mvarga           ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ft_21sh

CC = gcc

CFLAGS = -Wall -Wextra -Werror

LFLAGS = -ltermcap

SOURCE = ft_alt_key.c\
			ft_cd.c\
			ft_check.c\
			ft_check_cmd.c\
			ft_draw_str.c\
			ft_echo.c\
			ft_env.c\
			ft_free.c\
			ft_heredoc.c\
			ft_in_out.c\
			ft_isspace_new_arg.c\
			ft_key.c\
			ft_lst.c\
			ft_parce_pipe.c\
			ft_parcer.c\
			ft_read_input.c\
			ft_redirection.c\
			ft_select_cmd.c\
			ft_setenv.c\
			ft_show.c\
			ft_unsetenv.c\
			term.c\
			main.c

OBJECTS = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
			@make -C ./libft
			@gcc -o $(NAME) $(CFLAGS) $(SOURCE) -I ft_21sh.h ./libft/libft.a $(LFLAGS)

%.o: ./%.c
			@gcc $(CFALGS) -o $@ -c $< -I ft_21sh.h

clean:
			@make clean -C ./libft
			@ $(RM) $(OBJECTS)

fclean: clean
			@ $(RM) $(OBJ) $(NAME)
			@ make fclean -C ./libft

re: fclean all