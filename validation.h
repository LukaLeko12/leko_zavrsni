#ifndef VALIDATION_H
#define VALIDATION_H

#include <stddef.h>

int validation_validate_age(int age);
int validation_validate_risk(double risk);
int validation_validate_non_empty(const char *value);
int validation_validate_email(const char *email);
int validation_validate_phone(const char *phone);

int validation_read_non_empty(const char *prompt, char *buffer, size_t size);
int validation_read_int_range(const char *prompt, int min, int max, int *out_value);
int validation_read_double_range(const char *prompt, double min, double max, double *out_value);
int validation_read_optional_email(const char *prompt, char *buffer, size_t size);
int validation_read_optional_phone(const char *prompt, char *buffer, size_t size);

#endif
