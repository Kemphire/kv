#include "../include/helper.h"
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int has_char(const char *str) {
  while (*str) {
    if (!isdigit(*str))
      return 1;
    str++;
  }
  return 0;
}

Key_Value *parse_token(char **str, const char *del) {
  Key_Value *object = malloc(sizeof(Key_Value));

  if (object == NULL) {
    return NULL;
  }
  static const Op_Map op_map[] = {
      {"p", PUT}, {"a", ALL}, {"d", DELETE}, {"c", CLEAR}, {"g", GET}};

  char *oper = strsep(str, del);

  if (!oper) {
    return NULL;
  }

  int found = 0;

  for (int i = 0; i < 5; i++) {
    if (!strcmp(op_map[i].symbol, oper)) {
      object->oper = op_map[i].oper;
      found = 1;
      break;
    }
  }

  if (!found) {
    return NULL;
  }

  char *key = strsep(str, del);

  if (!key || has_char(key)) {
    return NULL;
  }

  char *endptr;
  errno = 0;
  long a = strtol(key, &endptr, 10);

  if (errno != 0) {
    return NULL;
  } else {
    object->key = (int)a;
  }

  char *value = strsep(str, del);

  if (!value) {
    return NULL;
  }

  object->value = value;

  return object;
}

Key_Value *parse_partial_token(char **str, const char *del) {
  Key_Value *object = malloc(sizeof(Key_Value));

  if (object == NULL) {
    return NULL;
  }
  static const Op_Map op_map[] = {
      {"p", PUT}, {"a", ALL}, {"d", DELETE}, {"c", CLEAR}, {"g", GET}};

  char *oper = strsep(str, del);

  if (!oper) {
    return NULL;
  }

  int found = 0;

  for (int i = 0; i < 5; i++) {
    if (!strcmp(op_map[i].symbol, oper)) {
      object->oper = op_map[i].oper;
      found = 1;
      break;
    }
  }

  if (!found) {
    return NULL;
  }

  char *key = strsep(str, del);

  if (!key || has_char(key)) {
    return NULL;
  }

  char *endptr;
  errno = 0;
  long a = strtol(key, &endptr, 10);

  if (errno != 0) {
    return NULL;
  } else {
    object->key = (int)a;
  }

  object->value = NULL;

  return object;
}

Key_Value *parse_stored_token(char **str, const char *del) {
  Key_Value *object = malloc(sizeof(Key_Value));

  if (object == NULL) {
    return NULL;
  }

  object->oper = NONE;

  char *key = strsep(str, del);

  if (!key || has_char(key)) {
    return NULL;
  }

  char *endptr;
  errno = 0;
  long a = strtol(key, &endptr, 10);

  if (errno != 0) {
    return NULL;
  } else {
    object->key = (int)a;
  }

  char *value = strsep(str, del);

  if (!value) {
    return NULL;
  }

  object->value = value;

  return object;
}

DB *all_objects(char *directory) {
  DIR *d;
  d = opendir(directory);

  if (d == NULL) {
    perror("Error opening directory\n");
    return NULL;
  }

  DB *db = malloc(sizeof(DB));

  db->cap = 20;

  db->objects = malloc(sizeof(Key_Value *) * db->cap);

  db->total_object = 0;

  struct dirent *f_dir;
  while ((f_dir = readdir(d)) != NULL) {
    if (f_dir->d_type == DT_REG) {
      if (db->total_object >= db->cap) {
        db->cap *= 2;
        Key_Value **new_objs =
            realloc(db->objects, sizeof(Key_Value *) * db->cap);
        if (!new_objs) {
          perror("realloc failled");
          closedir(d);
          return NULL;
        }
        db->objects = new_objs;
      }

      char *key = strdup(f_dir->d_name);

      int key_i = atoi(key);

      Key_Value *object = get_a_object(key_i);

      if (object) {
        db->objects[db->total_object++] = object;
      }
    }
  }
  closedir(d);
  return db;
}

Key_Value *get_data_from_file(char *file) {

  if (access(file, F_OK) != 0) {
    errno = EINVAL;
    return NULL;
  }

  FILE *fp = fopen(file, "r");

  if (!fp) {
    errno = ENOMEM;
    return NULL;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t nread = 0;

  if ((nread = getline(&line, &len, fp)) != -1) {
    Key_Value *obj = parse_stored_token(&line, ",");
    if (!obj) {
      fprintf(stdout, "fuck while reading obj from file\n");
      exit(EXIT_FAILURE);
    }
    return obj;
  }
  return NULL;
}

Key_Value *get_a_object(int key) {
  int size = snprintf(NULL, 0, "%s/%d", DB_DIR, key);
  char *file_path = malloc(size + 1);
  if (!file_path) {
    perror("malloc failled");
    return NULL;
  }
  sprintf(file_path, "%s/%d", DB_DIR, key);

  Key_Value *obj = get_data_from_file(file_path);

  if (!obj) {
    free(file_path);
    return NULL;
  }

  free(file_path);
  return obj;
}

bool write_a_object_to_file(int key, char *value) {
  int __size = snprintf(NULL, 0, "%s%d", DB_DIR, key);

  char *file_path = malloc(__size + 1);
  sprintf(file_path, "%s%d", DB_DIR, key);

  if (access(file_path, F_OK) == 0) {
    errno = EINVAL;
    return false;
  }

  FILE *fp = fopen(file_path, "w+");

  if (!fp)
    return false;

  int nwrite = fprintf(fp, "%d,%s", key, value);

  if (nwrite < 0)
    return false;

  return true;
}

bool db_dir_exist() { return access(DB_DIR, F_OK) == 0; }

void handle_put(char *arg) {

  Key_Value *tok = parse_token(&arg, ",");

  if (tok) {
    if (!write_a_object_to_file(tok->key, tok->value)) {
      printf("%d already in db\n", tok->key);
    }
    free(tok);
  } else if (!tok || errno == EINVAL) {
    printf("bad operation\n");
  }
}

void handle_get(Key_Value *obj) {
  int key = obj->key;
  Key_Value *data = get_a_object(key);

  if (!data) {
    fprintf(stderr, "%d Not avialable\n", key);
    return;
  }

  printf("%d,%s\n", data->key, data->value);
  free(data);
}

void distinguish_bw_get_and_delete(char *arg) {

  Key_Value *obj = parse_partial_token(&arg, ",");

  if (obj) {
    if (obj->oper == GET) {
      handle_get(obj);
    } else if (obj->oper == DELETE) {
      handle_delete(obj);
    } else {
      printf("bad operation\n");
      return;
    }
  } else {
    fprintf(stderr, "bad operation\n");
  }
}

void distinguish_bw_clear_and_all(char *arg) {
  if (!strcmp(arg, "a")) {
    handle_all();
  } else if (!strcmp(arg, "c")) {
    handle_clear();
  } else {
    printf("bad operation\n");
    return;
  }
}

int count_commas(const char *arg, char del) {
  int counter = 0;

  for (int i = 0; arg[i] != '\0'; i++) {

    if (arg[i] == del)
      counter++;
  }

  return counter;
}

void handle_the_situation(int count, char *arg) {
  switch (count) {
  case 2:
    handle_put(arg);
    break;
  case 1:
    distinguish_bw_get_and_delete(arg);
    break;
  case 0:
    distinguish_bw_clear_and_all(arg);
    break;
  }
}

void handle_clear() {
  DB *all_objs = all_objects(DB_DIR);
  if (all_objs) {
    for (int i = 0; i < all_objs->total_object; i++) {
      Key_Value *obj = all_objs->objects[i];
      handle_delete(obj);
      free(obj);
    }
    free(all_objs);
  } else {
    fprintf(stderr, "bad operation\n");
  }
}

void handle_all() {
  DB *all_objs = all_objects(DB_DIR);
  if (all_objs) {
    for (int i = 0; i < all_objs->total_object; i++) {
      Key_Value *obj = all_objs->objects[i];
      printf("%d,%s\n", obj->key, obj->value);
      free(obj);
    }
    free(all_objs);
  } else {
    fprintf(stderr, "bad operation\n");
  }
}

void handle_delete(Key_Value *obj) {
  int __size_fp = snprintf(NULL, 0, "%s%d", DB_DIR, obj->key);

  char *file_path = malloc(__size_fp + 1);

  sprintf(file_path, "%s%d", DB_DIR, obj->key);

  if (access(file_path, F_OK) == 0) {
    if (remove(file_path) != 0) {
      fprintf(stderr, "%d not found", obj->key);
    }
  }
  free(file_path);
}
