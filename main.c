# include <stdio.h>
# include <string.h>
# include <stdlib.h>

size_t ft_strlen(const char *s);
char *ft_strcpy(char *dst, const char *src);

int main() {
    char **strings = (char *[]){"", "Hello", "Hello, World!", "This is a longer string to test the ft_strlen function."};
    char **different_behaviors = (char *[]){NULL};
    int string_count = 4;

    printf ("\nTesting ft_strlen against man 3 strlen\n");
    for (int i = 0; i < string_count; i++) {
        size_t result = ft_strlen(strings[i]);
        if (result == strlen(strings[i]))
            printf("✅ Test passed for \"%s\"--> %zu\n", strings[i], result);
        else
            printf("❌ Test failed for \"%s\"--> %zu (expected %zu)\n", strings[i], result, strlen(strings[i]));
    }
    printf ("Testing ft_strlen different behaviors\n");
    for (int i = 0; i < 1; i++) {
        size_t result = ft_strlen(different_behaviors[i]);
        printf("Test \"%s\": %zu\n", different_behaviors[i], result);
    }


    printf ("\nTesting ft_strcpy against man 3 strcpy\n");
    for (int i = 0; i < string_count; i++) {
        char *dest = malloc(strlen(strings[i]) + 1);
        char *result = ft_strcpy(dest, strings[i]);
        if (strcmp(result, strings[i]) == 0)
            printf("✅ Test passed for \"%s\": %s\n", strings[i], result);
        else
            printf("❌ Test failed for \"%s\": %s (expected %s)\n", strings[i], result, strings[i]);
        free(dest);
    }

    // printf ("\nTesting ft_strcmp against man 3 strcmp\n");
    // for (int i = 0; i < string_count; i++) {
    //     size_t result = ft_strcmp(strings[i]);
    //     if (result == strcmp(strings[i]))
    //         printf("✅ Test passed for \"%s\"\n", strings[i]);
    //     else
    //         printf("❌ Test failed for \"%s\"\n", strings[i]);
    // }

    // printf ("\nTesting ft_write against man 2 write\n");
    // for (int i = 0; i < string_count; i++) {
    //     size_t result = ft_write(strings[i]);
    //     if (result == write(strings[i]))
    //         printf("✅ Test passed for \"%s\"\n", strings[i]);
    //     else
    //         printf("❌ Test failed for \"%s\"\n", strings[i]);
    // }

    // printf ("\nTesting ft_read against man 2 read\n");
    // for (int i = 0; i < string_count; i++) {
    //     size_t result = ft_read(strings[i]);
    //     if (result == read(strings[i]))
    //         printf("✅ Test passed for \"%s\"\n", strings[i]);
    //     else
    //         printf("❌ Test failed for \"%s\"\n", strings[i]);
    // }

    // printf ("\nTesting ft_strdup against man 3 strdup\n");
    // for (int i = 0; i < string_count; i++) {
    //     size_t result = ft_strdup(strings[i]);
    //     if (result == strdup(strings[i]))
    //         printf("✅ Test passed for \"%s\"\n", strings[i]);
    //     else
    //         printf("❌ Test failed for \"%s\"\n", strings[i]);
    // }
    return 0;
}