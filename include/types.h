#ifndef TYPES_H
#define TYPES_H

#define DB_DIR "/tmp/db/"

typedef enum Operations {
  PUT,
  GET,
  CLEAR,
  DELETE,
  ALL,
  NONE,
} Operations;

typedef struct Op_Map {
  char *symbol;
  Operations oper;
} Op_Map;

typedef struct Key_Value {
  int key;
  char *value;
  Operations oper;
} Key_Value;

typedef struct DB {
  int total_object;
  int cap;
  Key_Value **objects;
} DB;

#endif // !TYPES_H
