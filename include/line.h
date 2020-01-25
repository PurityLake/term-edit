#ifndef __H_LINE__
#define __H_LINE__

#include <stdlib.h>

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
        fprintf(stderr, "Unable to allocate %d bytes of memory\n", sizeof(line));
        exit(EXIT_FAILURE);
    }
    return ret;
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
        fprintf(stderr, "Unable to allocate %d bytes of memory.", sizeof(line_list));
        exit(EXIT_FAILURE);
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

void add_to_index_of_list(line_list *list, line *value, int idx) {
    if (idx == 0) {
        /*
        line_list *l = create_line_list();
        l->next = list;
        l->value = value;
        list->prev = l;
        list = l;
        */
    } else {
        line_list *curr = list;
        for (int i = 0; i < idx; ++i) {
            if (curr->next == NULL) {
                fprintf(stderr, "There is not %d lines in this file\n", idx + 1);
                exit(EXIT_FAILURE);
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
    }
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
