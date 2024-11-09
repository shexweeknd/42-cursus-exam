#include <stdarg.h>
#include <unistd.h>

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *str)
{
	int	printed;

	if (!str)
	{
		printed = ft_putstr("(null)");
		return (printed);
	}
	printed = 0;
	while (*str)
	{
		printed += ft_putchar(*str);
		str++;
	}
	return (printed);
}

int	ft_isset(const char *str, char *set)
{
	int	index;

	while (*str)
	{
		index = 0;
		while (set[index])
		{
			if (*str == set[index])
				return (1);
			index++;
		}
		str++;
	}
	return (0);
}

int	ft_strlen(char *str)
{
	int	result;

	result = 0;
	while (str[result])
		result++;
	return (result);
}

int	is_decimal(char *base)
{
	while (*base)
	{
		if (*base < 48 || *base > 57)
			return (0);
		base++;
	}
	return (1);
}

int	recurse_nbr(long nbr, char *base)
{
	int	printed;

	printed = 0;
	if (nbr < ft_strlen(base))
		printed += ft_putchar(base[nbr % ft_strlen(base)]);
	else if (nbr >= ft_strlen(base))
	{
		printed += recurse_nbr(nbr / ft_strlen(base), base);
		printed += ft_putchar(base[nbr % ft_strlen(base)]);
	}
	return (printed);
}

int	ft_putnbr_base(int nbr, char *base)
{
	int		printed;
	long	n;

	printed = 0;
	n = nbr;
	if (nbr < 0)
	{
		printed += ft_putchar('-');
		n *= -1;
	}
	printed += recurse_nbr(n, base);
	return (printed);
}

int	ft_putx(unsigned int nbr, char *base)
{
	int		printed;
	long	n;

	printed = 0;
	n = nbr;
	printed += recurse_nbr(n, base);
	return (printed);
}

int	ft_putformat(const char *str, va_list ap)
{
	int		printed;
	char	*set;

	set = "csdx%";
	if (!ft_isset(str, set))
		return (0);
	printed = 0;
	if (*str == 'c')
		printed += ft_putchar(va_arg(ap, int));
	else if (*str == 's')
		printed += ft_putstr(va_arg(ap, char *));
	else if (*str == 'd')
		printed += ft_putnbr_base(va_arg(ap, int), "0123456789");
	else if (*str == 'x')
		printed += ft_putx(va_arg(ap, int), "0123456789abcdef");
	else if (*str == 'X')
		printed += ft_putx(va_arg(ap, int), "0123456789ABCDEF");
	else if (*str == '%')
		printed += ft_putchar('%');
	return (printed);
}

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		printed;

	va_start(ap, str);
	printed = 0;
	while (*str)
	{
		if (*str == '%')
			printed += ft_putformat(++str, ap);
		else
			printed += ft_putchar(*str);
		str++;
	}
	va_end(ap);
	return (printed);
}

#include <stdio.h>

int	main(void)
{
	ft_printf("%x\n", -1);
	return (0);
}

/**
 * EXPLICATIONS DE L'algorithme
 *  1 - lire la chaine str jusqu'a ce qu'on ait un '%'
 *  2
	- faire un put_format a la position de '%' si on a une occurence de '%' au pointeur actuel
 *  3 - si on a aucune occurence de '%' alors on fait un putchar
 */

/**
 * IMPORTANT A VERIFIER LORS DU TEST
 * 		1 - putnbr_base gere les '-'
 * 		2 - put_x prend un cast de unsigned int
 * 		3 - putstr affiche "(null)" quand on fait entrer NULL en parametre
 *		4
			- bien verifier que tous les printed sont initialises a 0 dans chaque fonction
 */
