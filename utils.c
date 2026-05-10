#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    if (local == NULL) {
        snprintf(buffer, size, "1970-01-01 00:00:00");
        return;
    }
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local);
}

int utils_read_line(const char *prompt, char *buffer, size_t size) {
    int ch;

    if (prompt != NULL) {
        printf("%s", prompt);
    }

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    if (strchr(buffer, '\n') == NULL) {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    } else {
        buffer[strcspn(buffer, "\n")] = '\0';
    }

    return 1;
}

char *utils_trim(char *value) {
    char *start = value;
    char *end;

    while (*start != '\0' && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        *value = '\0';
        return value;
    }

    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    if (start != value) {
        memmove(value, start, strlen(start) + 1);
    }

    return value;
}

static void print_with_level(const char *level, const char *message) {
    char timestamp[32];
    get_timestamp(timestamp, sizeof(timestamp));
    printf("[%s] [%s] %s\n", timestamp, level, message);
}

void utils_print_info(const char *message) {
    print_with_level("INFO", message);
}

void utils_print_success(const char *message) {
    print_with_level("SUCCESS", message);
}

void utils_print_error(const char *message) {
    print_with_level("ERROR", message);
}

int utils_confirm(const char *prompt) {
    char input[8];
    char full_prompt[256];

    snprintf(full_prompt, sizeof(full_prompt), "%s (da/ne): ", prompt);
    while (utils_read_line(full_prompt, input, sizeof(input))) {
        utils_trim(input);
        if (strcmp(input, "da") == 0 || strcmp(input, "DA") == 0 || strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) {
            return 1;
        }
        if (strcmp(input, "ne") == 0 || strcmp(input, "NE") == 0 || strcmp(input, "n") == 0 || strcmp(input, "N") == 0) {
            return 0;
        }
        utils_print_error("Neispravan unos. Upisite 'da' ili 'ne'.");
    }
    return 0;
}

void utils_default_backup_name(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (local == NULL) {
        snprintf(buffer, size, "backup_onkoc.db");
        return;
    }

    strftime(buffer, size, "backup_onkoc_%Y%m%d_%H%M%S.db", local);
}
