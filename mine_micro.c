/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mine_micro.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: renebraaksma <renebraaksma@student.42.f      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/22 13:14:00 by rbraaksm      #+#    #+#                 */
/*   Updated: 2020/06/22 16:29:54 by rbraaksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_data
{
	int		w;
	int		h;
	char	b;
}				t_data;

typedef struct	s_shape
{
	char	t;
	float	x;
	float	y;
	float	w;
	float	h;
	char	c;
	struct s_shape	*next;
}				t_shape;

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int		error(char *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}

int		clean(FILE *file, char *str, char *errorstr)
{
	fclose(file);
	free(str);
	return (error(errorstr));
}

int
	in_rectangle(float x, float y, t_shape *shape)
{
	if (((x < shape->x || (shape->x + shape->w < x))
		|| (y < shape->y)) || (shape->y + shape->h < y))
		return (0);
	if (((x - shape->x < 1.00000000) || ((shape->x + shape->w) - x < 1.00000000)) ||
	((y - shape->y < 1.00000000 || ((shape->y + shape->h) - y < 1.00000000))))
		return (2);
	return (1);
}

void
	draw_shape(char **drawing, t_shape *shape, t_data *zone)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	while (i < zone->h)
	{
		j = 0;
		while (j< zone->w)
		{
			ret = in_rectangle(j, i, shape);
			if ((shape->t == 'r' && ret == 2)
				|| (shape->t == 'R' && ret))
				(*drawing)[(i * zone->w) + j] = shape->c;
			j++;
		}
		i++;
	}
}

int
	check_shape(t_shape *s)
{
	return (s->w > 0.00000000 && s->h > 0.00000000
			&& (s->t == 'r' || s->t == 'R'));
}

int		shape(FILE *file, t_data *d, char **draw)
{
	t_shape	s;
	int		ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &s.t, &s.x, &s.y, &s.w, &s.h, &s.c)) == 6)
	{
		if (!check_shape(&s))
			return (0);
		draw_shape(draw, &s, d);
	}
	if (ret != -1)
		return (0);
	return (1);
}

int		info(FILE **file, t_data *d, char *str, char **draw)
{
	int	i;

	if (!(*file = fopen(str, "r")))
		return (0);
	if (fscanf(*file, "%d %d %c\n", &d->w, &d->h, &d->b) != 3)
		return (0);
	if (d->w < 0 || d->h < 0 || d->w > 300 || d->h > 300)
		return (0);
	*draw = malloc(sizeof(char *) * (d->w * d->h));
	if (*draw == NULL)
		return (0);
	i = 0;
	while (i < (d->w * d->h))
		draw[0][i++] = d->b;
	return (1);
}

int		main(int argc, char **argv)
{
	FILE	*file;
	t_data	d;
	char	*str;

	d.w = 0;
	d.h = 0;
	d.b = 0;
	if (argc != 2)
		return (error("Error: Argument\n"));
	if (info(&file, &d, argv[1], &str) == 0)
		return (error("Error: Operation file corrupted\n"));
	if (shape(file, &d, &str) == 0)
		return (clean(file, str, "Error: Operation file corrupted\n"));
	int i = 0;
	while (i < d.h)
	{
		write(1, str + (i * d.w), d.w);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}