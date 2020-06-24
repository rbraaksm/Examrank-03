/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_paint.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: renebraaksma <renebraaksma@student.42.f      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/24 08:19:48 by rbraaksm      #+#    #+#                 */
/*   Updated: 2020/06/24 09:19:33 by rbraaksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct	s_data
{
	int			w;
	int			h;
	char		b;
	char		t;
	float		r;
	float		x;
	float		y;
	char		c;
}				t_data;

int				ft_strlen(char *str)
{
	int			i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int				error(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int				clean(FILE *file, char *str)
{
	if (file)
		fclose(file);
	if (str != NULL)
		free(str);
	return (error("Error: Operation file corrupted\n"));
}

void			set_struct(t_data *d)
{
	d->w = 0;
	d->h = 0;
	d->b = 0;
	d->t = 0;
	d->r = 0;
	d->x = 0;
	d->y = 0;
	d->c = 0;
}

int				circle(t_data *d, float x, float y)
{
	float		dis;

	dis = sqrtf(powf(x - d->x, 2.) + powf(y - d->y, 2.));
	if (dis <= d->r)
	{
		if ((d->r - dis) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void			draw_shape(t_data *d, char *str)
{
	int			x;
	int			y;
	int			in;

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

int				shape(FILE *file, t_data *d, char *str)
{
	int			ret;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &d->t, &d->x, &d->y, &d->r, &d->c)) == 5)
	{
		if (d->r <= 0.00000000 || (d->t != 'c' && d->t != 'C'))
			return (0);
		draw_shape(d, str);
	}
	if (ret != -1)
		return (0);
	return (1);
}

int				main(int argc, char **argv)
{
	t_data		d;
	FILE		*file;
	char		*str;
	int			i;

	str = NULL;
	i = 0;
	set_struct(&d);
	if (argc != 2)
		return (error("Error: Argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (clean(file, NULL));
	if (fscanf(file, "%d %d %c\n", &d.w, &d.h, &d.b) != 3)
		return (clean(file, NULL));
	if (d.w < 0 || d.w > 300 || d.h < 0 || d.h > 300)
		return (clean(file, NULL));
	str = malloc(sizeof(char *) * (d.w * d.h));
	if (str == NULL)
		return (clean(file, NULL));
	while (i < (d.w * d.h))
		str[i++] = d.b;
	if (!shape(file, &d, str))
		return (clean(file, str));
	i = 0;
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
