/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymoulou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 00:57:55 by aymoulou          #+#    #+#             */
/*   Updated: 2021/12/22 00:58:04 by aymoulou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_zone
{
    float width;
    float height;
    char ch;
} t_zone;

typedef struct s_circle
{
    char type;
    float x;
    float y;
    float radius;
    char c;
} t_circle;

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_putstr(char *str)
{
    while (*str)
    {
        ft_putchar(*str);
        str++;
    }
}

char *get_zone(FILE *file, t_zone *zone)
{
    int ret;
    char *draw;
    int i;

    if ((ret = fscanf(file, " %f %f %c\n", &zone->width, &zone->height, &zone->ch)) != 3)
        return (NULL);
    if (ret == -1)
        return (NULL);
    if (zone->width <= 0 || zone->width > 300)
        return(NULL);
    if (zone->height <= 0 || zone->height > 300)
        return (NULL);
    if (!(draw = (char *)malloc(sizeof(char) * zone->width * zone->height)))
        return(NULL);
    i = 0;
    while (i < zone->width * zone->height)
        draw[i++] = zone->ch;
    return (draw);
}

int is_in_circle(int x, int y, t_circle *circle)
{
    float distance;

    distance = sqrtf(powf(x - circle->x, 2.) + powf(y - circle->y, 2.));
    if (distance <= circle->radius)
    {
        if ((circle->radius - distance) < 1.)
            return (2);
        return (1);
    }
    return(0);
}

int drawing_shape(FILE *file, t_zone *zone, char *draw)
{
    int ret;
    int ret1;
    t_circle circle;
    int i;
    int j;

    ret = 0;
    while ((ret = fscanf(file, " %c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.c)) == 5)
    {
        if (circle.radius <= 0.00000000 || (circle.type != 'c' && circle.type != 'C'))
            return (0);
        j = 0;
        while (j < zone->height)
        {
            i = 0;
            while (i < zone->width)
            {
                ret1 = is_in_circle(i, j, &circle);
                if ((circle.type == 'c' && ret1 == 2) || (circle.type == 'C' && ret1))
                    draw[j * (int)zone->width + i] = circle.c;
                i++;
            }
            j++;
        }
    }
    if (ret != -1)
        return(0);
    return(1);
}

int ft_free(FILE *file, char *draw, char *msg)
{
    fclose(file);
    if (draw)
    {
        free(draw);
        draw = NULL;
    }
    if (msg)
        ft_putstr(msg);
    return (1);
}

int main(int ac, char **av)
{
    t_zone zone;
    FILE *file;
    char *draw;
    int i;

    if (ac != 2)
    {
        ft_putstr("Error: argument\n");
        return (1);
    }
    if (!(file = fopen(av[1], "r")))
    {
        ft_putstr("Error: Operation file corrupted\n");
        return (1);
    }
    if (!(draw = get_zone(file, &zone)))
        return(ft_free(file, NULL, "Error: Operation file corrupted\n"));
    if (!(drawing_shape(file, &zone, draw)))
        return(ft_free(file, draw, "Error: Operation file corrupted\n"));
    i = 0;
    while (i < zone.height)
    {
        write(1, draw + (i * (int)zone.width), zone.width);
        write(1, "\n", 1);
        i++;
    }
    ft_free(file, draw, NULL);
    return (0);
}
