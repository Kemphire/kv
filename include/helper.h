#ifndef HELPER_H
#define HELPER_H

#include "types.h"
#include <stdbool.h>

Key_Value *parse_token(char **, const char *);
Key_Value *parse_partial_or_stored_token(char **, const char *);

int has_char(const char *);

Key_Value *get_data_from_file(char *file);

DB *all_objects(char *dir);

Key_Value *get_a_object(int key);

bool db_dir_exist();

bool write_a_object_to_file(int key, char *value);

int count_commas(const char *arg, char del);

void handle_the_situation(int count, char *arg);

void distinguish_bw_clear_and_all(char *arg);

void distinguish_bw_get_and_delete(char *arg);

void handle_delete(Key_Value *obj);

void handle_clear();

void handle_all();
#endif // !HELPER_H
