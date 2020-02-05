#ifndef __H_LINE__
#define __H_LINE__

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int lineNo;
    int length, capacity;
    char *value;
} line;

typedef struct line_list {
    line *value;
    struct line_list *prev, *next;
} line_list;

line *create_line(int lineNo, int length, int capacity);
void resize_line_value(line *line);
void add_to_line(line *line, char val, int i);
void free_line(line *val);

line_list *create_line_list();
void add_to_end_of_list(line_list *list, line *val);
int add_to_index_of_list(line_list *list, line *value, int idx);
void free_line_list(line_list *val);

#endif /* __H_LINE__ */
