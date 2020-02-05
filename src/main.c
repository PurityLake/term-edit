#include <ncurses.h>
#include <string.h>

#include "line.h"

#define BORDER_PAIR 1
#define LINE_NO_PAIR 2

void print_border(int row, int col);
void print_screen(int row, int col, int x, int y, int minx, line_list *list);

int min(int a, int b);

int get_length_of_lineno(int max_line_no);

int main(int argc, char **argv) {
    int ch;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    if (has_colors()) {
        start_color();
        init_pair(BORDER_PAIR, COLOR_CYAN, COLOR_BLACK);
        init_pair(LINE_NO_PAIR, COLOR_YELLOW, COLOR_BLACK);
    } else {
        endwin();
        return EXIT_FAILURE;
    } 

    line_list *list = create_line_list();
    add_to_end_of_list(list, create_line(1, 0, 32));
    line_list *curr = list;

    int row, col;
    getmaxyx(stdscr, row, col);

    print_border(row, col);

    attron(COLOR_PAIR(LINE_NO_PAIR));
    printw("1. ");
    attroff(COLOR_PAIR(LINE_NO_PAIR));

    int x, y;
    int minx = 3;
    getyx(stdscr, y, x);
    int currLines = 1;

    while ((ch = getch()) != 27) {
        if (ch == '\n') {
            ++y;
            ++currLines;
            if (y < currLines) {
                if (!add_to_index_of_list(list, create_line(y - 1, 0, 32), y - 1)) {
                    break;
                }
            } else {
                add_to_end_of_list(list, create_line(y, 0, 32));
            }
            curr = curr->next;
            x = get_length_of_lineno(currLines);
            print_screen(row, col, x, y, minx, list);
        } else if (ch == KEY_UP) {
            if (y > 1) {
                --y;
                curr = curr->prev;
                x = min(x, curr->value->length + minx);
                move(y, x);
            }
        } else if (ch == KEY_DOWN) {
            if (y < row - 2 && y < currLines) {
                ++y;
                curr = curr->next;
                x = min(x, curr->value->length + minx);
                move(y, x);
            }
        } else {
            char c = (char)ch;
            add_to_line(curr->value, c, x - minx - 1);
            ++x;
            print_screen(row, col, x, y, minx, list);
        }
    }
    
    endwin();			// End curses mode		  
    
    free_line_list(list);

    return EXIT_SUCCESS;
}

void print_border(int row, int col) {
    attron(COLOR_PAIR(BORDER_PAIR));
    for (int i = 0; i < col; ++i) {
        mvprintw(0, i, "-");
        mvprintw(row-1, i, "-");
    }
    for (int i = 0; i < row; ++i) {
        mvprintw(i, 0, "|");
        mvprintw(i, col-1, "|");
    }
    mvprintw(0, 0, "+");
    mvprintw(0, col-1, "+");
    mvprintw(row-1, 0, "+");
    mvprintw(row-1, col-1, "+");
    attroff(COLOR_PAIR(BORDER_PAIR));
    move(1, 1);
}

void print_screen(int row, int col, int x, int y, int minx, line_list *list) {
    clear();
    print_border(row, col);
    line_list *curr = list;
    char *buf = (char *)malloc(minx);
    if (buf != NULL) {
        while (curr->next != NULL) {
            attron(COLOR_PAIR(LINE_NO_PAIR));
            printw("%d.", curr->value->lineNo);
            memset(buf, '\0', minx);
            int n = sprintf(buf, "%d.", curr->value->lineNo);
            for (int i = 0; i < minx - n; ++i) {
                printw(" ");
            }
            attroff(COLOR_PAIR(LINE_NO_PAIR));
            printw("%s", curr->value->value);
            move(curr->value->lineNo + 1, 1);
            curr = curr->next;
        }
    } else {
        endwin();
        fprintf(stderr, "Failed to allocate %d bytes of memory", minx);
    }
    free(buf);
    attron(COLOR_PAIR(LINE_NO_PAIR));
    printw("%d. ", curr->value->lineNo);
    attroff(COLOR_PAIR(LINE_NO_PAIR));
    printw("%s", curr->value->value);
    move(y, x);
}

int min(int a, int b) { 
    return (a < b) ? a : b;
}

int get_length_of_lineno(int max_line_no) {
    int minx = 3;
    do {
        max_line_no /= 10;
        ++minx;
    } while(max_line_no > 0);
    return minx;
}
