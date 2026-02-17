/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: everonel <everonel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 00:00:00 by emma              #+#    #+#             */
/*   Updated: 2026/02/17 12:24:22 by everonel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

size_t	ft_strlen(const char *s);
char	*ft_strcpy(char *dst, const char *src);
int		ft_strcmp(const char *s1, const char *s2);
ssize_t	ft_write(int fd, const void *buf, size_t count);
ssize_t	ft_read(int fd, void *buf, size_t count);
char	*ft_strdup(const char *s);

void	test_strlen(void)
{
	char	*test1;
	char	*test2;
    
    test1 = "Hello, World!";
    test2 = "";
	printf("\n=== ft_strlen ===\n");
	printf("ft_strlen(\"%s\") = %zu (expected %zu)\n",
		test1, ft_strlen(test1), strlen(test1));
	printf("ft_strlen(\"%s\") = %zu (expected %zu)\n",
		test2, ft_strlen(test2), strlen(test2));
}

void	test_strcpy(void)
{
	char	dest[50];
	char	*src;
    
    src = "Test string";
	printf("\n=== ft_strcpy ===\n");
	ft_strcpy(dest, src);
	printf("ft_strcpy result: \"%s\" (expected \"%s\")\n", dest, src);
}

void	test_strcmp(void)
{
	printf("\n=== ft_strcmp ===\n");
	printf("ft_strcmp(\"%s\", \"%s\") = %d (expected %d)\n",
		"abc", "abc", ft_strcmp("abc", "abc"), strcmp("abc", "abc"));
	printf("ft_strcmp(\"%s\", \"%s\") = %d (expected %d)\n",
		"abc", "abd", ft_strcmp("abc", "abd"), strcmp("abc", "abd"));
}

void	test_write(void)
{
	ssize_t	ret;
	
	printf("\n=== ft_write ===\n");
	printf("Writing to stdout: ");
	ret = ft_write(1, "Hello!\n", 7);
	printf("Bytes written: %zd (expected 7)\n", ret);
}

void	test_read(void)
{
	int		fd;
	char	buf[100];
	ssize_t	ret;
	
	printf("\n=== ft_read ===\n");
	fd = open("/tmp/test_read.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(fd, "Test content", 12);
	close(fd);
	fd = open("/tmp/test_read.txt", O_RDONLY);
	ret = ft_read(fd, buf, 12);
	buf[ret] = '\0';
	close(fd);
	unlink("/tmp/test_read.txt");
	printf("ft_read result: \"%s\" (read %zd bytes)\n", buf, ret);
}

void	test_strdup(void)
{
	char	*original;
    char	*duplicate;
	original = "Duplicate me!";

	printf("\n=== ft_strdup ===\n");
	duplicate = ft_strdup(original);
	printf("Original:  \"%s\"\n", original);
	printf("Duplicate: \"%s\"\n", duplicate);
	printf("Match: %s\n", strcmp(original, duplicate) == 0 ? "OK" : "KO");
	free(duplicate);
}

int	main(void)
{
	printf("\n");
	printf("╔════════════════════════════════════════════════════════╗\n");
	printf("║              LIBASM - DEMONSTRATION                    ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n");
	
	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write();
	test_read();
	test_strdup();
	
	printf("\n");
	printf("╔════════════════════════════════════════════════════════╗\n");
	printf("║              ALL FUNCTIONS DEMONSTRATED                ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n");
	printf("\n");
	
	return (0);
}
