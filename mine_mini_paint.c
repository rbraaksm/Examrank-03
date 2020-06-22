/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_paint.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: renebraaksma <renebraaksma@student.42.f      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/22 09:23:00 by rbraaksm      #+#    #+#                 */
/*   Updated: 2020/06/22 12:03:40 by rbraaksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct	s_data
{
	int		w;
	int		h;
	char	b;
}				t_data;

typedef struct	s_shape
{
	float	x;
	float	y;
	float	r;
	char	t;
	char	c;
}				t_shape;

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int		error(char *str)
{
	if (str != NULL)
		write(1, str, ft_strlen(str));
	return (1);
}

int		info(FILE **file, t_data *d, char *str, char **draw)
{
	int i;

	i = 0;
	if (!(*file = fopen(str, "r")))
		return (1);
	if (fscanf(*file, "%d %d %c\n", &d->w, &d->h, &d->b) != 3)
		return (1);
	if (d->w < 0 || d->w > 300 || d->h < 0 || d->h > 300)
		return (1);
	*draw = malloc(sizeof(char *) * (d->w * d->h));
	if (*draw == NULL)
		return (1);
	while (i < (d->w * d->h))
		draw[0][i++] = d->b;
	return (0);
}

int		clean(FILE *file, char *str, char *strerror)
{
	if (file)
		fclose(file);
	if (str)
		free(str);
	if (strerror)
		return (error(strerror));
	return (1);
}

int		circle(float x, float y, t_shape *s)
{
	float dis;

	dis = sqrtf(pow(x - s->x, 2.) + powf(y - s->y, 2.));
	if (dis <= s->r)
	{
		if ((s->r - dis) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	draw_shape(t_data *d, char *draw, t_shape *s)
{
	int	x;
	int	y;
	int in;

	y = 0;
	while (y < d->h)
	{
		x = 0;
		while (x < d->w)
		{
			in = circle((float)x, (float)y, s);
			if ((s->t == 'c' && in == 2)
				|| (s->t == 'C' && in))
				draw[(y * d->w) + x] = s->c;
			x++;
		}
		y++;
	}
}

int		shapes(FILE *file, t_data *d, char *draw)
{
	t_shape s;
	int		ret;

	ret = 5;
	while (ret == 5)
	{
		ret = fscanf(file, "%c %f %f %f %c\n", &s.t, &s.x, &s.y, &s.r, &s.c);
		if (s.r <= 0.00000000 || (s.t != 'c' && s.t != 'C'))
			return (0);
		draw_shape(d, draw, &s);
	}
	if (ret != -1)
		return (0);
	return (1);
}

int		main(int argc, char **argv)
{
	t_data	d;
	FILE	*file;
	char	*str;
	int		i;

	d.w = 0;
	d.h = 0;
	d.b = 0;
	i = 0;
	if (argc != 2)
		return (error("Error: Argument\n"));
	if (info(&file, &d, argv[1], &str))
		return (error("Error: Operation file corrupted\n"));
	if (shapes(file, &d, str) == 0)
		return (clean(file, str, "Error: Operation file corrupted\n"));
	while (i < d.h)
	{
		write(1, str + (i * d.w), d.w);
		write(1, "\n", 1);
		i++;
	}
	clean(file, str, NULL);
	return (0);
}