# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>

size_t  ft_strlen(const char *s);
char    *ft_strcpy(char *dst, const char *src);
int     ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const void *buf, size_t count);
ssize_t ft_read(int fd, void *buf, size_t count);
char    *ft_strdup(const char *s);

// Helper function to compare results
void print_test_result(const char *test_name, int passed) {
    if (passed)
        printf("✅ %s\n", test_name);
    else
        printf("❌ %s\n", test_name);
}

void test_ft_strlen() {
    printf("\n========== Testing ft_strlen ==========\n");
    
    struct {
        const char *str;
        const char *description;
    } tests[] = {
        {"", "empty string"},
        {"a", "single character"},
        {"Hello", "normal string"},
        {"Hello, World!", "string with punctuation"},
        {"This is a longer string to test the ft_strlen function.", "long string"},
        {"String with\ttabs", "string with tabs"},
        {"String with\nnewlines", "string with newlines"},
        {"123456789012345678901234567890", "30 characters"},
        {"Special chars: !@#$%^&*()", "special characters"}
    };
    
    int test_count = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    
    for (int i = 0; i < test_count; i++) {
        size_t result = ft_strlen(tests[i].str);
        size_t expected = strlen(tests[i].str);
        
        if (result == expected) {
            printf("✅ %-40s: %zu\n", tests[i].description, result);
            passed++;
        } else {
            printf("❌ %-40s: got %zu, expected %zu\n", 
                   tests[i].description, result, expected);
        }
    }
    
    printf("\nPassed: %d/%d\n", passed, test_count);
    
    // Edge case: NULL pointer (should return 0 or handle gracefully)
    printf("\nEdge case - NULL pointer:\n");
    size_t null_result = ft_strlen(NULL);
    printf("ft_strlen(NULL) = %zu\n", null_result);
}

void test_ft_strcpy() {
    printf("\n========== Testing ft_strcpy ==========\n");
    
    struct {
        const char *str;
        const char *description;
    } tests[] = {
        {"", "empty string"},
        {"a", "single character"},
        {"Hello", "normal string"},
        {"Hello, World!", "string with punctuation"},
        {"This is a longer string.", "long string"},
        {"String\twith\ttabs", "string with tabs"},
        {"Line1\nLine2\nLine3", "multiline string"},
        {"Special: !@#$%^&*()", "special characters"},
        {"12345", "numeric string"}
    };
    
    int test_count = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    
    for (int i = 0; i < test_count; i++) {
        char *dest = malloc(strlen(tests[i].str) + 1);
        char *result = ft_strcpy(dest, tests[i].str);
        
        int test_passed = (result == dest) && (strcmp(result, tests[i].str) == 0);
        
        if (test_passed) {
            printf("✅ %-40s: \"%s\"\n", tests[i].description, result);
            passed++;
        } else {
            printf("❌ %-40s: got \"%s\", expected \"%s\"\n", 
                   tests[i].description, result, tests[i].str);
        }
        free(dest);
    }
    
    printf("\nPassed: %d/%d\n", passed, test_count);
    
    // Edge case: NULL source
    printf("\nEdge case - NULL source:\n");
    char dest[10];
    ft_strcpy(dest, NULL);
    printf("ft_strcpy with NULL source handled\n");
}

void test_ft_strcmp() {
    printf("\n========== Testing ft_strcmp ==========\n");
    
    struct {
        const char *s1;
        const char *s2;
        const char *description;
    } tests[] = {
        {"", "", "both empty"},
        {"abc", "abc", "identical strings"},
        {"abc", "abd", "differ by one char"},
        {"abc", "ab", "s1 longer"},
        {"ab", "abc", "s2 longer"},
        {"ABC", "abc", "case difference"},
        {"", "a", "s1 empty, s2 not"},
        {"a", "", "s1 not empty, s2 empty"},
        {"hello", "world", "completely different"},
        {"test123", "test123", "identical with numbers"},
        {"test", "Test", "case at start"},
        {"a", "z", "first vs last letter"},
        {"Hello, World!", "Hello, World!", "identical complex string"},
        {"Hello, World!", "Hello, world!", "differ in middle"}
    };
    
    int test_count = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    
    for (int i = 0; i < test_count; i++) {
        int result = ft_strcmp(tests[i].s1, tests[i].s2);
        int expected = strcmp(tests[i].s1, tests[i].s2);
        
        // Check if both have the same sign (or both are zero)
        int same_sign = (result == 0 && expected == 0) ||
                       (result > 0 && expected > 0) ||
                       (result < 0 && expected < 0);
        
        if (same_sign) {
            printf("✅ %-40s: \"%s\" vs \"%s\" = %d\n", 
                   tests[i].description, tests[i].s1, tests[i].s2, result);
            passed++;
        } else {
            printf("❌ %-40s: got %d, expected %d\n", 
                   tests[i].description, result, expected);
        }
    }
    
    printf("\nPassed: %d/%d\n", passed, test_count);
}

void test_ft_write() {
    printf("\n========== Testing ft_write ==========\n");
    
    struct {
        const char *str;
        const char *description;
    } tests[] = {
        {"Hello\n", "simple string with newline"},
        {"", "empty string"},
        {"Line without newline", "no newline"},
        {"Multiple\nlines\nhere\n", "multiple lines"},
        {"12345", "numbers"}
    };
    
    int test_count = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    
    for (int i = 0; i < test_count; i++) {
        printf("\nTest: %s\n", tests[i].description);
        printf("Output: ");
        fflush(stdout);
        
        ssize_t result = ft_write(1, tests[i].str, strlen(tests[i].str));
        ssize_t expected = strlen(tests[i].str);
        
        if (result == expected) {
            printf(" ✅ (wrote %zd bytes)\n", result);
            passed++;
        } else {
            printf(" ❌ (wrote %zd bytes, expected %zd)\n", result, expected);
        }
    }
    
    // Test error handling: invalid fd
    printf("\nError handling test - invalid fd:\n");
    errno = 0;
    ssize_t result = ft_write(-1, "test", 4);
    if (result == -1 && errno != 0) {
        printf("✅ Correctly returned -1 and set errno to %d\n", errno);
        passed++;
    } else {
        printf("❌ Error handling failed\n");
    }
    
    printf("\nPassed: %d/%d\n", passed, test_count + 1);
}

void test_ft_read() {
    printf("\n========== Testing ft_read ==========\n");
    
    // Create a temporary file for testing
    char temp_filename[] = "/tmp/ft_read_test_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    
    if (temp_fd == -1) {
        printf("❌ Failed to create temp file\n");
        return;
    }
    
    // Write test data to file
    const char *test_data = "Hello, this is test data for ft_read!";
    write(temp_fd, test_data, strlen(test_data));
    close(temp_fd);
    
    int passed = 0;
    int total_tests = 0;
    
    // Test 1: Read entire file
    printf("\nTest 1: Read entire file\n");
    temp_fd = open(temp_filename, O_RDONLY);
    char buffer1[100] = {0};
    ssize_t result = ft_read(temp_fd, buffer1, sizeof(buffer1) - 1);
    
    if (result == (ssize_t)strlen(test_data) && strcmp(buffer1, test_data) == 0) {
        printf("✅ Read %zd bytes: \"%s\"\n", result, buffer1);
        passed++;
    } else {
        printf("❌ Failed to read correctly\n");
    }
    close(temp_fd);
    total_tests++;
    
    // Test 2: Read in chunks
    printf("\nTest 2: Read in small chunks\n");
    temp_fd = open(temp_filename, O_RDONLY);
    char buffer2[10] = {0};
    result = ft_read(temp_fd, buffer2, 5);
    
    if (result == 5 && strncmp(buffer2, test_data, 5) == 0) {
        printf("✅ Read first 5 bytes: \"%.*s\"\n", 5, buffer2);
        passed++;
    } else {
        printf("❌ Failed to read chunk\n");
    }
    close(temp_fd);
    total_tests++;
    
    // Test 3: Error handling - invalid fd
    printf("\nTest 3: Error handling - invalid fd\n");
    errno = 0;
    char buffer3[10];
    result = ft_read(-1, buffer3, sizeof(buffer3));
    
    if (result == -1 && errno != 0) {
        printf("✅ Correctly returned -1 and set errno to %d\n", errno);
        passed++;
    } else {
        printf("❌ Error handling failed\n");
    }
    total_tests++;
    
    // Clean up
    unlink(temp_filename);
    
    printf("\nPassed: %d/%d\n", passed, total_tests);
}

void test_ft_strdup() {
    printf("\n========== Testing ft_strdup ==========\n");
    
    struct {
        const char *str;
        const char *description;
    } tests[] = {
        {"", "empty string"},
        {"a", "single character"},
        {"Hello", "normal string"},
        {"Hello, World!", "string with punctuation"},
        {"This is a longer string to test.", "long string"},
        {"12345", "numeric string"},
        {"Special: !@#$%", "special characters"}
    };
    
    int test_count = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    
    for (int i = 0; i < test_count; i++) {
        char *result = ft_strdup(tests[i].str);
        
        if (result != NULL && strcmp(result, tests[i].str) == 0) {
            printf("✅ %-40s: \"%s\"\n", tests[i].description, result);
            passed++;
        } else {
            printf("❌ %-40s: failed\n", tests[i].description);
        }
        
        free(result);
    }
    
    // Test NULL input
    printf("\nEdge case - NULL input:\n");
    char *null_result = ft_strdup(NULL);
    if (null_result == NULL) {
        printf("✅ Correctly returned NULL for NULL input\n");
        passed++;
        test_count++;
    } else {
        printf("❌ Should return NULL for NULL input\n");
        free(null_result);
        test_count++;
    }
    
    printf("\nPassed: %d/%d\n", passed, test_count);
}

int main() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║         LIBASM COMPREHENSIVE TEST SUITE                ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    test_ft_strlen();
    test_ft_strcpy();
    test_ft_strcmp();
    test_ft_write();
    test_ft_read();
    test_ft_strdup();
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║              ALL TESTS COMPLETED                       ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    return 0;
}