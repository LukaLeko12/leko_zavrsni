#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

int utils_read_line(const char *prompt, char *buffer, size_t size);
char *utils_trim(char *value);

void utils_print_info(const char *message);
void utils_print_success(const char *message);
void utils_print_error(const char *message);

int utils_confirm(const char *prompt);
void utils_default_backup_name(char *buffer, size_t size);

#endif
