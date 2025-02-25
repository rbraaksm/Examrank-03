/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_paint.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: renebraaksma <renebraaksma@student.42.f      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/29 20:23:00 by rbraaksm      #+#    #+#                 */
/*   Updated: 2020/06/29 20:41:04 by rbraaksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

typedef struct	s_data
{
	int		w;
	int		h;
	char	b;
	char	t;
	char	c;
	float	x;
	float	y;
	float	r;
}				t_data;

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
	write(1, str, ft_strlen(str));
	return (1);
}

int		circle(t_data *d, float x, float y)
{
	float	dis;

	dis = sqrtf(powf(x - d->x, 2.) + powf(y - d->y, 2.));
	if (dis <= d->r)
	{
		if ((d->r - dis) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	draw(t_data *d, char *str)
{
	int	x;
	int	y;
	int	in;

	y = 0;
	while (y < d->h)
	{
		x = 0;
		while (x < d->w)
		{
			in = circle(d, x, y);
			if ((d->t == 'c' && in == 2) || (d->t == 'C' && in))
				str[(y * d->w) + x] = d->c;
			x++;
		}
		y++;
	}
}

int		shape(FILE *file, t_data *d, char *str)
{
	int		ret;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &d->t, &d->x, &d->y, &d->r, &d->c)) == 5)
	{
		if (d->r <= 0.00000000 || (d->t != 'c' && d->t != 'C'))
			return (0);
		draw(d, str);
	}
	if (ret != -1)
		return (0);
	return (1);
}

int		main(int argc, char **argv)
{
	FILE	*file;
	t_data	d;
	char	*str;
	int		i;

	str = NULL;
	i = 0;
	d.w = 0;
	d.h = 0;
	d.b = 0;
	d.x = 0;
	d.y = 0;
	d.t = 0;
	d.c = 0;
	d.r = 0;
	if (argc != 2)
		return (error("Error: Argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (error("Error: Operation file corrupted\n"));
	if (fscanf(file, "%d %d %c\n", &d.w, &d.h, &d.b) != 3)
		return (error("Error: Operation file corrupted\n"));
	if (d.w <= 0 || d.w > 300 | d.h <= 0 || d.h > 300)
		return (error("Error: Operation file corrupted\n"));
	str = malloc(sizeof(char *) * (d.w * d.h));
	if (str == NULL)
		return (error("Error: Operation file corrupted\n"));
	while (i < (d.w * d.h))
		str[i++] = d.b;
	i = 0;
	if (shape(file, &d, str) == 0)
		return (error("Error: Operation file corrupted\n"));
	while (i < d.h)
	{
		write(1, str + (i * d.w), d.w);
		write(1, "\n", 1);
		i++;
	}
	fclose(file);
	free(str);
	return (0);
}