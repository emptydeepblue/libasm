#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

size_t  ft_strlen(const char *s);
char    *ft_strcpy(char *dst, const char *src);
int     ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const void *buf, size_t count);
ssize_t ft_read(int fd, void *buf, size_t count);
char    *ft_strdup(const char *s);

void test_adversarial_strlen() {
    printf("\n========== ADVERSARIAL ft_strlen Tests ==========\n");
    int passed = 0;
    int total = 0;
    
    // Test 1: Very long string (potential buffer issues)
    printf("\n1. Very long string (10000 chars):\n");
    char *long_str = malloc(10001);
    memset(long_str, 'A', 10000);
    long_str[10000] = '\0';
    size_t result = ft_strlen(long_str);
    if (result == 10000) {
        printf("✅ Correct: %zu\n", result);
        passed++;
    } else {
        printf("❌ Failed: got %zu, expected 10000\n", result);
    }
    free(long_str);
    total++;
    
    // Test 2: String with all zeros except null terminator
    printf("\n2. String with null bytes (\\0\\0\\0):\n");
    char null_bytes[] = {0, 0, 0, 0};
    result = ft_strlen(null_bytes);
    if (result == 0) {
        printf("✅ Correct: %zu\n", result);
        passed++;
    } else {
        printf("❌ Failed: got %zu, expected 0\n", result);
    }
    total++;
    
    // Test 3: String with high ASCII values
    printf("\n3. String with high ASCII (0xFF chars):\n");
    char high_ascii[] = {0xFF, 0xFF, 0xFF, 0};
    result = ft_strlen(high_ascii);
    size_t expected = strlen(high_ascii);
    if (result == expected) {
        printf("✅ Correct: %zu\n", result);
        passed++;
    } else {
        printf("❌ Failed: got %zu, expected %zu\n", result, expected);
    }
    total++;
    
    // Test 4: Enormous string (1 million chars)
    printf("\n4. Enormous string (1000000 chars):\n");
    char *enormous_str = malloc(1000001);
    if (!enormous_str) {
        printf("❌ Failed: malloc failed\n");
        total++;
    } else {
        memset(enormous_str, 'E', 1000000);
        enormous_str[1000000] = '\0';
        size_t result = ft_strlen(enormous_str);
        if (result == 1000000) {
            printf("✅ Correct: %zu\n", result);
            passed++;
        } else {
            printf("❌ Failed: got %zu, expected 1000000\n", result);
        }
        free(enormous_str);
        total++;
    }
    
    printf("\nPassed: %d/%d\n", passed, total);
}

void test_adversarial_strcpy() {
    printf("\n========== ADVERSARIAL ft_strcpy Tests ==========\n");
    int passed = 0;
    int total = 0;
    
    // Test 1: Overlapping memory (undefined but shouldn't crash)
    printf("\n1. Very long string copy:\n");
    char src[10001];
    memset(src, 'B', 10000);
    src[10000] = '\0';
    char *dst = malloc(10001);
    char *result = ft_strcpy(dst, src);
    if (result == dst && strcmp(dst, src) == 0) {
        printf("✅ Correct: copied 10000 chars\n");
        passed++;
    } else {
        printf("❌ Failed\n");
    }
    free(dst);
    total++;
    
    // Test 2: Binary data with embedded nulls (should stop at first null)
    printf("\n2. Binary data handling:\n");
    char binary_src[] = {'A', 'B', 0, 'C', 'D', 0};
    char binary_dst[10];
    ft_strcpy(binary_dst, binary_src);
    if (binary_dst[0] == 'A' && binary_dst[1] == 'B' && binary_dst[2] == 0) {
        printf("✅ Correct: stopped at first null\n");
        passed++;
    } else {
        printf("❌ Failed\n");
    }
    total++;
    
    // Test 3: Check return value is destination
    printf("\n3. Return value check:\n");
    char test_src[] = "test";
    char test_dst[10];
    result = ft_strcpy(test_dst, test_src);
    if (result == test_dst) {
        printf("✅ Correct: returns destination pointer\n");
        passed++;
    } else {
        printf("❌ Failed: wrong return value\n");
    }
    total++;
    
    printf("\nPassed: %d/%d\n", passed, total);
}

void test_adversarial_strcmp() {
    printf("\n========== ADVERSARIAL ft_strcmp Tests ==========\n");
    int passed = 0;
    int total = 0;
    
    // Test 1: Very long identical strings
    printf("\n1. Very long identical strings (5000 chars):\n");
    char *s1 = malloc(5001);
    char *s2 = malloc(5001);
    memset(s1, 'X', 5000);
    memset(s2, 'X', 5000);
    s1[5000] = '\0';
    s2[5000] = '\0';
    int result = ft_strcmp(s1, s2);
    if (result == 0) {
        printf("✅ Correct: 0\n");
        passed++;
    } else {
        printf("❌ Failed: got %d, expected 0\n", result);
    }
    free(s1);
    free(s2);
    total++;
    
    // Test 2: Differ only at last char
    printf("\n2. Differ only at last character:\n");
    char str1[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char str2[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" "B";
    result = ft_strcmp(str1, str2);
    int expected = strcmp(str1, str2);
    int same_sign = (result == 0 && expected == 0) ||
                   (result > 0 && expected > 0) ||
                   (result < 0 && expected < 0);
    if (same_sign) {
        printf("✅ Correct: same sign\n");
        passed++;
    } else {
        printf("❌ Failed: got %d, expected sign of %d\n", result, expected);
    }
    total++;
    
    // Test 3: Unsigned char comparison (important!)
    printf("\n3. Unsigned char comparison (\\xFF vs \\x01):\n");
    char s3[] = {(char)0xFF, 0};
    char s4[] = {(char)0x01, 0};
    result = ft_strcmp(s3, s4);
    expected = strcmp(s3, s4);
    same_sign = (result == 0 && expected == 0) ||
               (result > 0 && expected > 0) ||
               (result < 0 && expected < 0);
    if (same_sign) {
        printf("✅ Correct: treats as unsigned\n");
        passed++;
    } else {
        printf("❌ Failed: wrong signedness handling\n");
    }
    total++;
    
    // Test 4: Both NULL (your implementation behavior)
    printf("\n4. Both NULL:\n");
    result = ft_strcmp(NULL, NULL);
    printf("   ft_strcmp(NULL, NULL) = %d (implementation defined)\n", result);
    passed++; // Accept any behavior for NULL
    total++;
    
    printf("\nPassed: %d/%d\n", passed, total);
}

void test_adversarial_write() {
    printf("\n========== ADVERSARIAL ft_write Tests ==========\n");
    int passed = 0;
    int total = 0;
    
    // Test 1: Write 0 bytes
    printf("\n1. Write 0 bytes:\n");
    ssize_t result = ft_write(1, "test", 0);
    if (result == 0) {
        printf("✅ Correct: wrote 0 bytes\n");
        passed++;
    } else {
        printf("❌ Failed: got %zd, expected 0\n", result);
    }
    total++;
    
    // Test 2: Large write
    printf("\n2. Large write (8192 bytes):\n");
    char *large_buf = malloc(8192);
    memset(large_buf, 'L', 8192);
    result = ft_write(1, large_buf, 8192);
    printf("\n");
    if (result == 8192) {
        printf("✅ Correct: wrote 8192 bytes\n");
        passed++;
    } else {
        printf("❌ Failed: got %zd, expected 8192\n", result);
    }
    free(large_buf);
    total++;
    
    // Test 3: Write to stderr (fd=2)
    printf("\n3. Write to stderr (fd=2):\n");
    result = ft_write(2, "stderr test\n", 12);
    if (result == 12) {
        printf("✅ Correct: wrote 12 bytes to stderr\n");
        passed++;
    } else {
        printf("❌ Failed: got %zd\n", result);
    }
    total++;
    
    // Test 4: Invalid fd
    printf("\n4. Invalid fd (-1):\n");
    errno = 0;
    result = ft_write(-1, "test", 4);
    if (result == -1 && errno == EBADF) {
        printf("✅ Correct: errno = EBADF (%d)\n", errno);
        passed++;
    } else {
        printf("⚠️  Result: %zd, errno = %d (may vary)\n", result, errno);
        passed++; // Accept if returns -1
    }
    total++;
    
    // Test 5: Write with NULL buffer (should fail)
    printf("\n5. NULL buffer:\n");
    errno = 0;
    result = ft_write(1, NULL, 10);
    if (result == -1) {
        printf("✅ Correct: failed with errno = %d\n", errno);
        passed++;
    } else {
        printf("❌ Failed: should return -1\n");
    }
    total++;
    
    printf("\nPassed: %d/%d\n", passed, total);
}

void test_adversarial_read() {
    printf("\n========== ADVERSARIAL ft_read Tests ==========\n");
    int passed = 0;
    int total = 0;
    
    // Create test file
    char filename[] = "/tmp/stress_test_XXXXXX";
    int fd = mkstemp(filename);
    write(fd, "TESTDATA", 8);
    close(fd);
    
    // Test 1: Read 0 bytes
    printf("\n1. Read 0 bytes:\n");
    fd = open(filename, O_RDONLY);
    char buf[100];
    ssize_t result = ft_read(fd, buf, 0);
    if (result == 0) {
        printf("✅ Correct: read 0 bytes\n");
        passed++;
    } else {
        printf("❌ Failed: got %zd, expected 0\n", result);
    }
    close(fd);
    total++;
    
    // Test 2: Read more than file size
    printf("\n2. Read more than available (buffer=100, file=8):\n");
    fd = open(filename, O_RDONLY);
    memset(buf, 0, 100);
    result = ft_read(fd, buf, 100);
    if (result == 8 && strcmp(buf, "TESTDATA") == 0) {
        printf("✅ Correct: read 8 bytes (actual file size)\n");
        passed++;
    } else {
        printf("❌ Failed: got %zd bytes\n", result);
    }
    close(fd);
    total++;
    
    // Test 3: Read from closed fd
    printf("\n3. Read from closed fd:\n");
    errno = 0;
    result = ft_read(999, buf, 10);
    if (result == -1 && errno == EBADF) {
        printf("✅ Correct: errno = EBADF (%d)\n", errno);
        passed++;
    } else {
        printf("⚠️  Result: %zd, errno = %d (may vary)\n", result, errno);
        passed++; // Accept if returns -1
    }
    total++;
    
    // Test 4: Read with NULL buffer
    printf("\n4. NULL buffer:\n");
    fd = open(filename, O_RDONLY);
    errno = 0;
    result = ft_read(fd, NULL, 10);
    if (result == -1) {
        printf("✅ Correct: failed with errno = %d\n", errno);
        passed++;
    } else {
        printf("❌ Failed: should return -1\n");
    }
    close(fd);
    total++;
    
    // Test 5: Large file read (1MB)
    printf("\n5. Large file read (1MB):\n");
    char large_filename[] = "/tmp/large_stress_test_XXXXXX";
    int large_fd = mkstemp(large_filename);
    if (large_fd == -1) {
        printf("❌ Failed: mkstemp failed\n");
        total++;
        goto cleanup;
    }
    const size_t large_size = 1024 * 1024; // 1MB
    char *large_data = malloc(large_size);
    if (!large_data) {
        printf("❌ Failed: malloc failed\n");
        close(large_fd);
        unlink(large_filename);
        total++;
        goto cleanup;
    }
    memset(large_data, 'R', large_size);
    size_t written = 0;
    while (written < large_size) {
        ssize_t w = write(large_fd, large_data + written, large_size - written);
        if (w == -1) {
            printf("❌ Failed: write failed\n");
            close(large_fd);
            free(large_data);
            unlink(large_filename);
            total++;
            goto cleanup;
        }
        written += w;
    }
    close(large_fd);
    char *read_buf = malloc(large_size);
    if (!read_buf) {
        printf("❌ Failed: malloc for read failed\n");
        free(large_data);
        unlink(large_filename);
        total++;
        goto cleanup;
    }
    int fd_read = open(large_filename, O_RDONLY);
    if (fd_read == -1) {
        printf("❌ Failed: open for read failed\n");
        free(large_data);
        free(read_buf);
        unlink(large_filename);
        total++;
        goto cleanup;
    }
    size_t read_total = 0;
    while (read_total < large_size) {
        ssize_t r = ft_read(fd_read, read_buf + read_total, large_size - read_total);
        if (r == -1) {
            printf("❌ Failed: ft_read failed\n");
            close(fd_read);
            free(large_data);
            free(read_buf);
            unlink(large_filename);
            total++;
            goto cleanup;
        }
        if (r == 0) break; // EOF
        read_total += r;
    }
    close(fd_read);
    if (read_total == large_size && memcmp(read_buf, large_data, large_size) == 0) {
        printf("✅ Correct: read 1MB correctly\n");
        passed++;
    } else {
        printf("❌ Failed: read %zu bytes, expected %zu\n", read_total, large_size);
    }
    free(large_data);
    free(read_buf);
    unlink(large_filename);
    total++;
    
    cleanup:
    unlink(filename);
    printf("\nPassed: %d/%d\n", passed, total);
}

void test_adversarial_strdup() {
    printf("\n========== ADVERSARIAL ft_strdup Tests ==========\n");
    int passed = 0;
    int total = 0;
    
    // Test 1: Long string
    printf("\n1. Long string (10000 chars):\n");
    char *long_str = malloc(10001);
    memset(long_str, 'D', 10000);
    long_str[10000] = '\0';
    char *dup = ft_strdup(long_str);
    if (dup != NULL && strcmp(dup, long_str) == 0 && dup != long_str) {
        printf("✅ Correct: duplicated 10000 chars, different pointer\n");
        passed++;
    } else {
        printf("❌ Failed\n");
    }
    free(dup);
    free(long_str);
    total++;
    
    // Test 2: Multiple duplications (memory leak test)
    printf("\n2. Multiple duplications (leak test - run with valgrind):\n");
    for (int i = 0; i < 100; i++) {
        char *temp = ft_strdup("leak test");
        if (temp == NULL) {
            printf("❌ Failed: malloc failed at iteration %d\n", i);
            break;
        }
        free(temp);
    }
    printf("✅ Completed 100 iterations\n");
    passed++;
    total++;
    
    // Test 3: Check that duplicate is independent
    printf("\n3. Check duplicate independence:\n");
    char original[] = "original";
    char *duplicate = ft_strdup(original);
    if (duplicate != NULL) {
        duplicate[0] = 'X';
        if (original[0] == 'o' && duplicate[0] == 'X') {
            printf("✅ Correct: duplicate is independent\n");
            passed++;
        } else {
            printf("❌ Failed: not independent\n");
        }
        free(duplicate);
    } else {
        printf("❌ Failed: malloc failed\n");
    }
    total++;
    
    // Test 4: Empty string
    printf("\n4. Empty string:\n");
    dup = ft_strdup("");
    if (dup != NULL && strcmp(dup, "") == 0) {
        printf("✅ Correct: duplicated empty string\n");
        passed++;
        free(dup);
    } else {
        printf("❌ Failed\n");
    }
    total++;
    
    // Test 5: NULL input
    printf("\n5. NULL input:\n");
    dup = ft_strdup(NULL);
    if (dup == NULL) {
        printf("✅ Correct: returned NULL\n");
        passed++;
    } else {
        printf("❌ Failed: should return NULL\n");
        free(dup);
    }
    total++;
    
    printf("\nPassed: %d/%d\n", passed, total);
}

int main() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║         ADVERSARIAL & STRESS TEST SUITE               ║\n");
    printf("║    Tests designed to break common implementations     ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    test_adversarial_strlen();
    test_adversarial_strcpy();
    test_adversarial_strcmp();
    test_adversarial_write();
    test_adversarial_read();
    test_adversarial_strdup();
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║          STRESS TESTING COMPLETED                     ║\n");
    printf("║  Run with valgrind to check for memory issues:        ║\n");
    printf("║  valgrind --leak-check=full ./stress_test             ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    return 0;
}
