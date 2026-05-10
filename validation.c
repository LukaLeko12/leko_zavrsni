#include "validation.h"

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validation_validate_age(int age) {
    return age >= 0 && age <= 150;
}

int validation_validate_risk(double risk) {
    return risk >= 0.0 && risk <= 100.0;
}

int validation_validate_non_empty(const char *value) {
    size_t i;
    if (value == NULL) {
        return 0;
    }
    for (i = 0; value[i] != '\0'; i++) {
        if (!isspace((unsigned char)value[i])) {
            return 1;
        }
    }
    return 0;
}

int validation_validate_email(const char *email) {
    const char *at;
    const char *dot;

    if (email == NULL || *email == '\0') {
        return 1;
    }

    at = strchr(email, '@');
    if (at == NULL || at == email || *(at + 1) == '\0') {
        return 0;
    }

    dot = strrchr(at + 1, '.');
    return dot != NULL && dot != at + 1 && *(dot + 1) != '\0';
}

int validation_validate_phone(const char *phone) {
    size_t i;
    int digit_found = 0;

    if (phone == NULL || *phone == '\0') {
        return 1;
    }

    for (i = 0; phone[i] != '\0'; i++) {
        char ch = phone[i];
        if (isdigit((unsigned char)ch)) {
            digit_found = 1;
            continue;
        }
        if (ch == '+' || ch == '-' || ch == ' ' || ch == '(' || ch == ')' || ch == '/') {
            continue;
        }
        return 0;
    }

    return digit_found;
}

int validation_read_non_empty(const char *prompt, char *buffer, size_t size) {
    while (utils_read_line(prompt, buffer, size)) {
        utils_trim(buffer);
        if (validation_validate_non_empty(buffer)) {
            return 1;
        }
        utils_print_error("Polje ne smije biti prazno.");
    }
    return 0;
}

int validation_read_int_range(const char *prompt, int min, int max, int *out_value) {
    char input[64];
    char *end_ptr;
    long value;

    while (utils_read_line(prompt, input, sizeof(input))) {
        utils_trim(input);
        if (!validation_validate_non_empty(input)) {
            utils_print_error("Polje ne smije biti prazno.");
            continue;
        }

        value = strtol(input, &end_ptr, 10);
        if (*end_ptr != '\0') {
            utils_print_error("Unos mora biti cijeli broj.");
            continue;
        }
        if (value < min || value > max) {
            char message[128];
            snprintf(message, sizeof(message), "Vrijednost mora biti u rasponu %d-%d.", min, max);
            utils_print_error(message);
            continue;
        }
        *out_value = (int)value;
        return 1;
    }
    return 0;
}

int validation_read_double_range(const char *prompt, double min, double max, double *out_value) {
    char input[64];
    char *end_ptr;
    double value;

    while (utils_read_line(prompt, input, sizeof(input))) {
        utils_trim(input);
        if (!validation_validate_non_empty(input)) {
            utils_print_error("Polje ne smije biti prazno.");
            continue;
        }

        value = strtod(input, &end_ptr);
        if (*end_ptr != '\0') {
            utils_print_error("Unos mora biti broj.");
            continue;
        }
        if (value < min || value > max) {
            char message[128];
            snprintf(message, sizeof(message), "Vrijednost mora biti u rasponu %.2f-%.2f.", min, max);
            utils_print_error(message);
            continue;
        }
        *out_value = value;
        return 1;
    }
    return 0;
}

int validation_read_optional_email(const char *prompt, char *buffer, size_t size) {
    while (utils_read_line(prompt, buffer, size)) {
        utils_trim(buffer);
        if (validation_validate_email(buffer)) {
            return 1;
        }
        utils_print_error("Neispravan email format.");
    }
    return 0;
}

int validation_read_optional_phone(const char *prompt, char *buffer, size_t size) {
    while (utils_read_line(prompt, buffer, size)) {
        utils_trim(buffer);
        if (validation_validate_phone(buffer)) {
            return 1;
        }
        utils_print_error("Neispravan broj telefona.");
    }
    return 0;
}
