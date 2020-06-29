/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   micro_paint.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: renebraaksma <renebraaksma@student.42.f      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/23 19:57:16 by rbraaksm      #+#    #+#                 */
/*   Updated: 2020/06/29 19:57:41 by rbraaksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_data
{
	int		width;
	int		height;
	char	back;
	char	t;
	float	x;
	float	y;
	float	w;
	float	h;
	char	c;
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

int		clean(FILE *file, char *str)
{
	fclose(file);
	if (str != NULL)
		free(str);
	return (error("Error: Operation file corrupted\n"));
}

int		rectangle(t_data *d, float x, float y)
{
	if (x < d->x || (d->x + d->w) < x || y < d->y || (d->y + d->h) < y)
		return (0);
	if (x - d->x < 1.000000000 || (d->x + d->w) - x < 1.00000000 ||
		y - d->y < 1.000000000 || (d->y + d->h) - y < 1.00000000)
		return (2);
	return (1);
}

void	draw_shape(t_data *d, char *str)
{
	int	x;
	int	y;
	int	in;

	y = 0;
	while (y < d->height)
	{
		x = 0;
		while (x < d->width)
		{
			in = rectangle(d, x, y);
			if ((d->t == 'r' && in == 2) || (d->t == 'R' && in))
				str[(y * d->width) + x] = d->c;
			x++;
		}
		y++;
	}
}

int		shape(FILE *file, t_data *d, char *str)
{
	int		ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &d->t, &d->x, &d->y, &d->w, &d->h, &d->c)) == 6)
	{
		if (d->w <= 0 || d->h <= 0 || (d->t != 'r' && d->t != 'R'))
			return (0);
		draw_shape(d, str);
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
	if (argc != 2)
		return (error("Error: Argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (error("Error: Operation file corrupted\n"));
	if (fscanf(file, "%d %d %c\n", &d.width, &d.height, &d.back) != 3)
		return (clean(file, NULL));
	if (d.width <= 0 || d.width > 300 || d.height <= 0 || d.height > 300)
		return (clean(file, NULL));
	str = malloc(sizeof(char *) * (d.width * d.height));
	if (str == NULL)
		return (clean(file, NULL));
	while (i < (d.width * d.height))
		str[i++] = d.back;
	if (!shape(file, &d, str))
		return (clean(file, str));
	i = 0;
	while (i < d.height)
	{
		write(1, str + (i * d.width), d.width);
		write(1, "\n", 1);
		i++;
	}
	fclose(file);
	free(str);
	return (0);
}
