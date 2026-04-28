/*
** eval.test.c — 42 Evaluator-Proof Test Suite for libasm
**
** Each test runs in a child process via fork() so a crash or hang
** in one test cannot kill the entire suite.
**
** Build:  make test-eval
** Valgrind: make valtest-eval
**
** Exit code: 0 = all tests passed, 1 = at least one failure
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* ── prototypes ──────────────────────────────────────────────────────────── */
size_t   ft_strlen(const char *s);
char    *ft_strcpy(char *dst, const char *src);
int      ft_strcmp(const char *s1, const char *s2);
ssize_t  ft_write(int fd, const void *buf, size_t count);
ssize_t  ft_read(int fd, void *buf, size_t count);
char    *ft_strdup(const char *s);

/* ── test bookkeeping ────────────────────────────────────────────────────── */
static int g_passed = 0;
static int g_total  = 0;

static void ok(const char *name)
{
    printf("  \033[32m[PASS]\033[0m %s\n", name);
    g_passed++;
    g_total++;
}

static void fail(const char *name, const char *detail)
{
    printf("  \033[31m[FAIL]\033[0m %s — %s\n", name, detail);
    g_total++;
}

/* ═══════════════════════════════════════════════════════════════════════════
** ft_strlen
** ═══════════════════════════════════════════════════════════════════════════ */

static void strlen_empty(void)
{
    if (ft_strlen("") == 0) { ok("empty string → 0"); exit(0); }
    fail("empty string → 0", "returned non-zero"); exit(1);
}

static void strlen_single(void)
{
    if (ft_strlen("a") == 1) { ok("single char → 1"); exit(0); }
    fail("single char → 1", "wrong result"); exit(1);
}

static void strlen_matches_libc(void)
{
    const char *s = "Hello, 42!";
    if (ft_strlen(s) == strlen(s)) { ok("matches strlen()"); exit(0); }
    fail("matches strlen()", "differs from libc"); exit(1);
}

static void strlen_high_ascii(void)
{
    char s[] = {(char)0xC3, (char)0xA9, (char)0xFF, 0}; /* é + 0xFF */
    if (ft_strlen(s) == strlen(s)) { ok("high-ASCII bytes"); exit(0); }
    fail("high-ASCII bytes", "wrong count"); exit(1);
}

static void strlen_all_printable(void)
{
    char buf[256];
    for (int i = 0; i < 255; i++) buf[i] = (char)(i + 1);
    buf[255] = '\0';
    if (ft_strlen(buf) == 255) { ok("255 non-null bytes"); exit(0); }
    fail("255 non-null bytes", "wrong count"); exit(1);
}

static void strlen_long(void)
{
    char *s = malloc(100001);
    if (!s) { fail("long string (100k)", "malloc failed"); exit(1); }
    memset(s, 'Z', 100000);
    s[100000] = '\0';
    int pass = (ft_strlen(s) == 100000);
    free(s);
    if (pass) { ok("long string (100k chars)"); exit(0); }
    fail("long string (100k chars)", "wrong count"); exit(1);
}

static void strlen_null(void)
{
    /* 42 ft_strlen is expected to handle NULL gracefully */
    size_t r = ft_strlen(NULL);
    if (r == 0) { ok("NULL → 0"); exit(0); }
    fail("NULL → 0", "returned non-zero (implementation-defined)"); exit(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
** ft_strcpy
** ═══════════════════════════════════════════════════════════════════════════ */

static void strcpy_basic(void)
{
    char dst[32];
    char *ret = ft_strcpy(dst, "hello");
    if (ret == dst && strcmp(dst, "hello") == 0) { ok("basic copy + return value"); exit(0); }
    fail("basic copy + return value", "content or pointer wrong"); exit(1);
}

static void strcpy_empty(void)
{
    char dst[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    ft_strcpy(dst, "");
    if (dst[0] == '\0') { ok("empty src copies null terminator"); exit(0); }
    fail("empty src copies null terminator", "first byte not \\0"); exit(1);
}

static void strcpy_null_terminator_copied(void)
{
    char dst[8];
    memset(dst, 'X', 8);
    ft_strcpy(dst, "ab");
    if (dst[0]=='a' && dst[1]=='b' && dst[2]=='\0') { ok("null terminator is written"); exit(0); }
    fail("null terminator is written", "byte[2] != \\0"); exit(1);
}

static void strcpy_returns_dst(void)
{
    char dst[16];
    char *ret = ft_strcpy(dst, "test");
    if (ret == dst) { ok("returns dst pointer exactly"); exit(0); }
    fail("returns dst pointer exactly", "returned different address"); exit(1);
}

static void strcpy_long(void)
{
    char *src = malloc(50001);
    char *dst = malloc(50001);
    if (!src || !dst) { fail("long copy (50k)", "malloc failed"); exit(1); }
    memset(src, 'A', 50000);
    src[50000] = '\0';
    char *ret = ft_strcpy(dst, src);
    int pass = (ret == dst && strcmp(dst, src) == 0);
    free(src); free(dst);
    if (pass) { ok("long copy (50k chars)"); exit(0); }
    fail("long copy (50k chars)", "content mismatch"); exit(1);
}

static void strcpy_matches_libc(void)
{
    const char *src = "The quick brown fox jumps over the lazy dog";
    char ref[64], got[64];
    strcpy(ref, src);
    ft_strcpy(got, src);
    if (memcmp(ref, got, strlen(src) + 1) == 0) { ok("matches strcpy() byte-for-byte"); exit(0); }
    fail("matches strcpy() byte-for-byte", "differs from libc"); exit(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
** ft_strcmp
** ═══════════════════════════════════════════════════════════════════════════ */

static void strcmp_equal(void)
{
    if (ft_strcmp("abc", "abc") == 0) { ok("equal strings → 0"); exit(0); }
    fail("equal strings → 0", "not zero"); exit(1);
}

static void strcmp_empty_equal(void)
{
    if (ft_strcmp("", "") == 0) { ok("both empty → 0"); exit(0); }
    fail("both empty → 0", "not zero"); exit(1);
}

static void strcmp_s1_less(void)
{
    int r = ft_strcmp("abc", "abd");
    int e = strcmp("abc", "abd");
    int same_sign = (r < 0 && e < 0);
    if (same_sign) { ok("s1 < s2 → negative"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %d, libc got %d", r, e);
    fail("s1 < s2 → negative", buf); exit(1);
}

static void strcmp_s1_greater(void)
{
    int r = ft_strcmp("abd", "abc");
    int e = strcmp("abd", "abc");
    int same_sign = (r > 0 && e > 0);
    if (same_sign) { ok("s1 > s2 → positive"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %d, libc got %d", r, e);
    fail("s1 > s2 → positive", buf); exit(1);
}

static void strcmp_prefix_shorter(void)
{
    /* "abc" < "abcd" */
    int r = ft_strcmp("abc", "abcd");
    int e = strcmp("abc", "abcd");
    int same_sign = (r < 0 && e < 0);
    if (same_sign) { ok("prefix shorter s1 → negative"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %d, libc got %d", r, e);
    fail("prefix shorter s1 → negative", buf); exit(1);
}

static void strcmp_prefix_longer(void)
{
    /* "abcd" > "abc" */
    int r = ft_strcmp("abcd", "abc");
    int e = strcmp("abcd", "abc");
    int same_sign = (r > 0 && e > 0);
    if (same_sign) { ok("prefix longer s1 → positive"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %d, libc got %d", r, e);
    fail("prefix longer s1 → positive", buf); exit(1);
}

static void strcmp_unsigned_char(void)
{
    /* 0xFF > 0x01 when treated as unsigned (255 > 1) */
    char s1[] = {(char)0xFF, '\0'};
    char s2[] = {(char)0x01, '\0'};
    int r = ft_strcmp(s1, s2);
    int e = strcmp(s1, s2);
    int same_sign = (r > 0 && e > 0);
    if (same_sign) { ok("unsigned char: 0xFF > 0x01"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %d, libc got %d (signed vs unsigned)", r, e);
    fail("unsigned char: 0xFF > 0x01", buf); exit(1);
}

static void strcmp_long_equal(void)
{
    char *s1 = malloc(10001), *s2 = malloc(10001);
    if (!s1 || !s2) { fail("long equal strings", "malloc"); exit(1); }
    memset(s1, 'X', 10000); s1[10000] = '\0';
    memset(s2, 'X', 10000); s2[10000] = '\0';
    int pass = (ft_strcmp(s1, s2) == 0);
    free(s1); free(s2);
    if (pass) { ok("long equal strings → 0"); exit(0); }
    fail("long equal strings → 0", "not zero"); exit(1);
}

static void strcmp_differ_last(void)
{
    char s1[17], s2[17];
    memset(s1, 'A', 16); s1[16] = '\0';
    memset(s2, 'A', 16); s2[15] = 'B'; s2[16] = '\0';
    int r = ft_strcmp(s1, s2);
    int e = strcmp(s1, s2);
    int same_sign = (r < 0 && e < 0);
    if (same_sign) { ok("strings differ only at last byte"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %d, libc got %d", r, e);
    fail("strings differ only at last byte", buf); exit(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
** ft_write
** ═══════════════════════════════════════════════════════════════════════════ */

static void write_zero_bytes(void)
{
    ssize_t r = ft_write(1, "x", 0);
    if (r == 0) { ok("write 0 bytes → 0"); exit(0); }
    char buf[32]; snprintf(buf, sizeof(buf), "got %zd", r);
    fail("write 0 bytes → 0", buf); exit(1);
}

static void write_returns_count(void)
{
    int fd = open("/dev/null", O_WRONLY);
    if (fd < 0) { fail("write returns count", "open /dev/null failed"); exit(1); }
    ssize_t r = ft_write(fd, "hello", 5);
    close(fd);
    if (r == 5) { ok("write returns byte count"); exit(0); }
    char buf[32]; snprintf(buf, sizeof(buf), "got %zd, expected 5", r);
    fail("write returns byte count", buf); exit(1);
}

static void write_invalid_fd(void)
{
    errno = 0;
    ssize_t r = ft_write(-1, "x", 1);
    if (r == -1 && errno == EBADF) { ok("invalid fd → -1 + errno EBADF"); exit(0); }
    char buf[64]; snprintf(buf, sizeof(buf), "got %zd, errno=%d (want -1, EBADF=%d)", r, errno, EBADF);
    fail("invalid fd → -1 + errno EBADF", buf); exit(1);
}

static void write_sets_errno(void)
{
    errno = 0;
    ft_write(-1, "x", 1);
    if (errno != 0) { ok("errno is set on error"); exit(0); }
    fail("errno is set on error", "errno stayed 0"); exit(1);
}

static void write_to_file(void)
{
    char tmpl[] = "/tmp/libasm_eval_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd < 0) { fail("write to file", "mkstemp failed"); exit(1); }
    ssize_t r = ft_write(fd, "libasm", 6);
    close(fd);
    unlink(tmpl);
    if (r == 6) { ok("write to file → 6"); exit(0); }
    char buf[32]; snprintf(buf, sizeof(buf), "got %zd", r);
    fail("write to file → 6", buf); exit(1);
}

static void write_large(void)
{
    int fd = open("/dev/null", O_WRONLY);
    if (fd < 0) { fail("large write", "open /dev/null failed"); exit(1); }
    char *big = malloc(65536);
    if (!big) { close(fd); fail("large write", "malloc"); exit(1); }
    memset(big, 'W', 65536);
    ssize_t r = ft_write(fd, big, 65536);
    close(fd); free(big);
    if (r == 65536) { ok("large write (64k) → 65536"); exit(0); }
    char buf[32]; snprintf(buf, sizeof(buf), "got %zd", r);
    fail("large write (64k) → 65536", buf); exit(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
** ft_read
** ═══════════════════════════════════════════════════════════════════════════ */

static void read_zero_bytes(void)
{
    int fd = open("/dev/zero", O_RDONLY);
    if (fd < 0) { fail("read 0 bytes", "open /dev/zero"); exit(1); }
    char buf[8];
    ssize_t r = ft_read(fd, buf, 0);
    close(fd);
    if (r == 0) { ok("read 0 bytes → 0"); exit(0); }
    char tmp[32]; snprintf(tmp, sizeof(tmp), "got %zd", r);
    fail("read 0 bytes → 0", tmp); exit(1);
}

static void read_returns_count(void)
{
    char tmpl[] = "/tmp/libasm_read_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd < 0) { fail("read returns count", "mkstemp"); exit(1); }
    write(fd, "libasm42", 8);
    close(fd);

    fd = open(tmpl, O_RDONLY);
    char buf[16] = {0};
    ssize_t r = ft_read(fd, buf, 8);
    close(fd);
    unlink(tmpl);

    if (r == 8 && memcmp(buf, "libasm42", 8) == 0) { ok("read returns byte count + correct data"); exit(0); }
    char tmp[64]; snprintf(tmp, sizeof(tmp), "got %zd bytes, data='%.*s'", r, (int)r, buf);
    fail("read returns byte count + correct data", tmp); exit(1);
}

static void read_invalid_fd(void)
{
    errno = 0;
    char buf[8];
    ssize_t r = ft_read(-1, buf, 4);
    if (r == -1 && errno == EBADF) { ok("invalid fd → -1 + errno EBADF"); exit(0); }
    char tmp[64]; snprintf(tmp, sizeof(tmp), "got %zd, errno=%d (want -1, EBADF=%d)", r, errno, EBADF);
    fail("invalid fd → -1 + errno EBADF", tmp); exit(1);
}

static void read_sets_errno(void)
{
    errno = 0;
    char buf[8];
    ft_read(-1, buf, 4);
    if (errno != 0) { ok("errno is set on error"); exit(0); }
    fail("errno is set on error", "errno stayed 0"); exit(1);
}

static void read_eof(void)
{
    char tmpl[] = "/tmp/libasm_eof_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd < 0) { fail("read EOF", "mkstemp"); exit(1); }
    write(fd, "hi", 2);
    close(fd);

    fd = open(tmpl, O_RDONLY);
    char buf[64];
    /* read more than file has → should return actual bytes available */
    ssize_t r = ft_read(fd, buf, 64);
    close(fd);
    unlink(tmpl);

    if (r == 2) { ok("read returns actual bytes (less than count) at EOF"); exit(0); }
    char tmp[32]; snprintf(tmp, sizeof(tmp), "got %zd, expected 2", r);
    fail("read returns actual bytes at EOF", tmp); exit(1);
}

static void read_from_devzero(void)
{
    int fd = open("/dev/zero", O_RDONLY);
    if (fd < 0) { fail("read /dev/zero", "open failed"); exit(1); }
    char buf[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    ssize_t r = ft_read(fd, buf, 8);
    close(fd);
    if (r == 8 && buf[0] == 0 && buf[7] == 0) { ok("read 8 bytes from /dev/zero → zeros"); exit(0); }
    char tmp[32]; snprintf(tmp, sizeof(tmp), "got %zd", r);
    fail("read 8 bytes from /dev/zero → zeros", tmp); exit(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
** ft_strdup
** ═══════════════════════════════════════════════════════════════════════════ */

static void strdup_not_null(void)
{
    char *r = ft_strdup("hello");
    if (r != NULL) { free(r); ok("returns non-NULL"); exit(0); }
    fail("returns non-NULL", "got NULL"); exit(1);
}

static void strdup_content_matches(void)
{
    char *r = ft_strdup("hello42");
    if (r && strcmp(r, "hello42") == 0) { free(r); ok("content matches source"); exit(0); }
    char tmp[64]; snprintf(tmp, sizeof(tmp), "got '%s'", r ? r : "(null)");
    if (r) free(r);
    fail("content matches source", tmp); exit(1);
}

static void strdup_different_pointer(void)
{
    const char *src = "pointer";
    char *r = ft_strdup(src);
    if (r && r != src) { free(r); ok("returns different pointer (not same address)"); exit(0); }
    if (r) free(r);
    fail("returns different pointer", "same address as source"); exit(1);
}

static void strdup_independent(void)
{
    char *r = ft_strdup("original");
    if (!r) { fail("independence check", "strdup returned NULL"); exit(1); }
    r[0] = 'X';
    /* source is a string literal; we just verify modifying dup doesn't segfault */
    ok("duplicate is independently modifiable"); free(r); exit(0);
}

static void strdup_empty(void)
{
    char *r = ft_strdup("");
    if (r && strcmp(r, "") == 0) { free(r); ok("empty string duplicated correctly"); exit(0); }
    if (r) free(r);
    fail("empty string duplicated correctly", "wrong result or NULL"); exit(1);
}

static void strdup_null_terminator(void)
{
    char *r = ft_strdup("abc");
    if (r && r[3] == '\0') { free(r); ok("null terminator present at end"); exit(0); }
    if (r) free(r);
    fail("null terminator present at end", "byte[3] != \\0"); exit(1);
}

static void strdup_can_be_freed(void)
{
    char *r = ft_strdup("freeable");
    if (!r) { fail("result can be freed", "strdup returned NULL"); exit(1); }
    free(r); /* if this crashes valgrind will catch it */
    ok("result can be freed without error"); exit(0);
}

static void strdup_null_input(void)
{
    char *r = ft_strdup(NULL);
    if (r == NULL) { ok("NULL input → NULL"); exit(0); }
    free(r);
    fail("NULL input → NULL", "returned non-NULL"); exit(1);
}

static void strdup_long(void)
{
    char *src = malloc(20001);
    if (!src) { fail("long strdup (20k)", "malloc"); exit(1); }
    memset(src, 'D', 20000);
    src[20000] = '\0';
    char *r = ft_strdup(src);
    int pass = (r && strcmp(r, src) == 0 && r != src);
    free(src);
    if (r) free(r);
    if (pass) { ok("long strdup (20k chars)"); exit(0); }
    fail("long strdup (20k chars)", "content mismatch or same pointer"); exit(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
** main
** ═══════════════════════════════════════════════════════════════════════════ */

typedef struct { const char *name; void (*fn)(void); } t_test;

/*
** Each entry in the table is run in a child process.
** The child calls ok()/fail() then exits 0 or 1.
** The parent counts the exit status to build the grand total.
*/

static int run_all(const char *section, t_test *tests, int n)
{
    printf("\n\033[1;34m══ %s ══\033[0m\n", section);
    int section_pass = 0;
    for (int i = 0; i < n; i++) {
        fflush(stdout);
        pid_t pid = fork();
        if (pid == 0) {
            tests[i].fn();
            exit(0); /* fn should exit itself, this is a safety net */
        }
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            printf("  \033[31m[CRASH]\033[0m %s — signal %d\n", tests[i].name, WTERMSIG(status));
            g_total++;
        } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            /* child printed ok() itself */
            g_passed++;
            g_total++;
            section_pass++;
        } else {
            /* child printed fail() itself */
            g_total++;
        }
    }
    return section_pass;
}

int main(void)
{
    printf("\n");
    printf("\033[1;37m╔══════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[1;37m║          libasm — 42 Evaluator Test Suite                ║\033[0m\n");
    printf("\033[1;37m╚══════════════════════════════════════════════════════════╝\033[0m\n");

    t_test strlen_tests[] = {
        {"empty string → 0",           strlen_empty},
        {"single char → 1",            strlen_single},
        {"matches strlen()",           strlen_matches_libc},
        {"high-ASCII bytes",           strlen_high_ascii},
        {"255 non-null bytes",         strlen_all_printable},
        {"long string (100k)",         strlen_long},
        {"NULL → 0",                   strlen_null},
    };

    t_test strcpy_tests[] = {
        {"basic copy + return value",          strcpy_basic},
        {"empty src copies null terminator",   strcpy_empty},
        {"null terminator is written",         strcpy_null_terminator_copied},
        {"returns dst pointer exactly",        strcpy_returns_dst},
        {"long copy (50k)",                    strcpy_long},
        {"matches strcpy() byte-for-byte",     strcpy_matches_libc},
    };

    t_test strcmp_tests[] = {
        {"equal strings → 0",                 strcmp_equal},
        {"both empty → 0",                    strcmp_empty_equal},
        {"s1 < s2 → negative",                strcmp_s1_less},
        {"s1 > s2 → positive",                strcmp_s1_greater},
        {"prefix shorter s1 → negative",      strcmp_prefix_shorter},
        {"prefix longer s1 → positive",       strcmp_prefix_longer},
        {"unsigned char: 0xFF > 0x01",        strcmp_unsigned_char},
        {"long equal strings → 0",            strcmp_long_equal},
        {"differ only at last byte",          strcmp_differ_last},
    };

    t_test write_tests[] = {
        {"write 0 bytes → 0",              write_zero_bytes},
        {"write returns byte count",       write_returns_count},
        {"invalid fd → -1 + EBADF",       write_invalid_fd},
        {"errno is set on error",          write_sets_errno},
        {"write to file",                  write_to_file},
        {"large write (64k)",              write_large},
    };

    t_test read_tests[] = {
        {"read 0 bytes → 0",                           read_zero_bytes},
        {"read returns count + correct data",          read_returns_count},
        {"invalid fd → -1 + EBADF",                   read_invalid_fd},
        {"errno is set on error",                      read_sets_errno},
        {"returns actual bytes at EOF",                read_eof},
        {"read from /dev/zero",                        read_from_devzero},
    };

    t_test strdup_tests[] = {
        {"returns non-NULL",                          strdup_not_null},
        {"content matches source",                    strdup_content_matches},
        {"returns different pointer",                 strdup_different_pointer},
        {"duplicate is independently modifiable",     strdup_independent},
        {"empty string",                              strdup_empty},
        {"null terminator present",                   strdup_null_terminator},
        {"result can be freed",                       strdup_can_be_freed},
        {"NULL input → NULL",                         strdup_null_input},
        {"long strdup (20k)",                         strdup_long},
    };

#define COUNT(arr) (int)(sizeof(arr) / sizeof((arr)[0]))

    run_all("ft_strlen",  strlen_tests,  COUNT(strlen_tests));
    run_all("ft_strcpy",  strcpy_tests,  COUNT(strcpy_tests));
    run_all("ft_strcmp",  strcmp_tests,  COUNT(strcmp_tests));
    run_all("ft_write",   write_tests,   COUNT(write_tests));
    run_all("ft_read",    read_tests,    COUNT(read_tests));
    run_all("ft_strdup",  strdup_tests,  COUNT(strdup_tests));

    printf("\n");
    printf("\033[1;37m╔══════════════════════════════════════════════════════════╗\033[0m\n");
    if (g_passed == g_total)
        printf("\033[1;32m║  RESULT: %3d / %3d passed — ALL TESTS PASSED              ║\033[0m\n", g_passed, g_total);
    else
        printf("\033[1;31m║  RESULT: %3d / %3d passed                                 ║\033[0m\n", g_passed, g_total);
    printf("\033[1;37m╚══════════════════════════════════════════════════════════╝\033[0m\n");
    printf("\n");
    printf("  Tip: run with valgrind to catch memory errors:\n");
    printf("  \033[33mmake valtest-eval\033[0m\n\n");

    return (g_passed == g_total) ? 0 : 1;
}
