#ifndef __H_LINE__
#define __H_LINE__

#include <stdlib.h>
#include <ncurses.h>

typedef struct {
    int lineNo;
    int length, capacity;
    char *value;
} line;

typedef struct line_list {
    line *value;
    struct line_list *prev, *next;
} line_list;

line *create_line(int lineNo, int length, int capacity) {
    line *ret = (line *)malloc(sizeof(line));
    if (ret != NULL) {
        ret->lineNo = lineNo;
        ret->length = length;
        ret->capacity = capacity;
        ret->value = (char *)malloc(capacity);
        if (ret->value == NULL) {
            fprintf(stderr, "Unable to allocate %d bytes of memory\n", capacity);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Unable to allocate %lu bytes of memory\n", sizeof(line));
        exit(EXIT_FAILURE);
    }
    return ret;
}

void resize_line_value(line *line) {
    line->value = (char *)realloc(line->value, line->capacity * 2);
    if (line->value != NULL) {
        line->capacity *= 2;
        memset(line->value + line->length, '\0', line->capacity - line->length);
    } else {
        fprintf(stderr, "Failed to reallocate %d bytes of memory", line->capacity * 2);
        endwin();
        exit(EXIT_FAILURE);
    } 
}

void add_to_line(line *line, char val, int i) {
    if (line->length + 1 >= line->capacity) {
        resize_line_value(line);
    }
    if (i < line->length) {
        char *cpystuff = (char *)malloc(line->length - i);
        strncpy(cpystuff, line->value + i, line->length - i);
        line->value[i] = val;
        strncpy(line->value + i + 1, cpystuff, line->length - i);
    } else {
        line->value[i] = val;
    }
    ++line->length;
}

void free_line(line *val) {
    free(val->value);
    free(val);
    val = NULL;
}

line_list *create_line_list() {
    line_list *list = (line_list *)malloc(sizeof(line_list));
    if (list != NULL) {
        list->value = NULL;
        list->prev = NULL;
        list->next = NULL;
    } else {
        fprintf(stderr, "Unable to allocate %lu bytes of memory.", sizeof(line_list));
        return NULL;
    }
    return list;
}

void add_to_end_of_list(line_list *list, line *val) {
    if (list->value == NULL) {
        list->value = val;
    } else if (list->next == NULL) {
        list->next = create_line_list();
        list->next->value = val;
        list->next->prev = list;
    } else {
        add_to_end_of_list(list->next, val);
    }
}

int add_to_index_of_list(line_list *list, line *value, int idx) {
    line_list *curr = list;
    for (int i = 0; i < idx; ++i) {
        if (curr->next == NULL) {
            fprintf(stderr, "There is not %d lines in this file\n", idx + 1);
            return 0;
        }
        curr = curr->next;
    }
    line_list *prev = curr->prev;
    line_list *new_line = create_line_list();
    new_line->prev = curr->prev;
    new_line->next = curr;
    new_line->value = value;
    prev->next = new_line;
    curr->prev = new_line;
    while (curr->next != NULL) {
        ++curr->value->lineNo;
        curr = curr->next;
    }
    ++curr->value->lineNo;
    return 1;
}

void free_line_list(line_list *val) {
    if (val->next == NULL) {
        if (val->prev == NULL) {
            if (val->value == NULL) {
                free_line(val->value);
            }
            free(val);
        } else {
            free_line(val->value);
            val->prev->next = NULL;
            line_list *prev = val->prev;
            free(val);
            if (prev != NULL) {
                free_line_list(prev);
            }
        }
    } else {
        free_line_list(val->next);
    }
}

#endif /* __H_LINE__ */
