#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- STRING OPERATIONS ---------- */

void cmd_len(const char *s) {
    printf("Length: %zu\n", strlen(s));
}

void cmd_cmp(const char *a, const char *b) {
    int r = strcmp(a, b);
    if (r == 0) printf("Equal\n");
    else if (r < 0) printf("'%s' < '%s'\n", a, b);
    else printf("'%s' > '%s'\n", a, b);
}

void cmd_find(const char *hay, const char *needle) {
    char *p = strstr(hay, needle);
    if (p)
        printf("Found at index %ld\n", p - hay);
    else
        printf("Not found\n");
}

void cmd_copy(const char *s) {
    char *buf = malloc(strlen(s) + 1);
    if (!buf) {
        perror("malloc");
        return;
    }
    strcpy(buf, s);
    printf("Copied string: %s\n", buf);
    free(buf);
}

/* ---------- MEMORY OPERATIONS ---------- */

void cmd_memtest(void) {
    int *a = malloc(5 * sizeof(int));
    int *b = malloc(5 * sizeof(int));

    if (!a || !b) {
        perror("malloc");
        exit(1);
    }

    memset(a, 0, 5 * sizeof(int));

    for (int i = 0; i < 5; i++)
        a[i] = i * 10;

    memcpy(b, a, 5 * sizeof(int));

    printf("Memory copy result:\n");
    for (int i = 0; i < 5; i++)
        printf("%d ", b[i]);
    printf("\n");

    memmove(a + 1, a, 4 * sizeof(int));

    printf("After memmove:\n");
    for (int i = 0; i < 5; i++)
        printf("%d ", a[i]);
    printf("\n");

    printf("memcmp(a, b) = %d\n", memcmp(a, b, 5 * sizeof(int)));

    free(a);
    free(b);
}

/* ---------- MAIN ---------- */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  len <str>\n");
        printf("  cmp <a> <b>\n");
        printf("  find <str> <substr>\n");
        printf("  copy <str>\n");
        printf("  memtest\n");
        return 0;
    }

    if (strcmp(argv[1], "len") == 0 && argc == 3)
        cmd_len(argv[2]);
    else if (strcmp(argv[1], "cmp") == 0 && argc == 4)
        cmd_cmp(argv[2], argv[3]);
    else if (strcmp(argv[1], "find") == 0 && argc == 4)
        cmd_find(argv[2], argv[3]);
    else if (strcmp(argv[1], "copy") == 0 && argc == 3)
        cmd_copy(argv[2]);
    else if (strcmp(argv[1], "memtest") == 0)
        cmd_memtest();
    else
        printf("Invalid command\n");

    return 0;
}
