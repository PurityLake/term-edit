#include <ncurses.h>
#include <string.h>

#include "line.h"

#define BORDER_PAIR 1
#define LINE_NO_PAIR 2

void print_border(int row, int col);
void print_screen(int row, int col, int x, int y, line_list *list);

inline int min(int a, int b) { 
    return (a < b) ? a : b;
}

int main(int argc, char **argv) {
    line_list *list = create_line_list();
    add_to_end_of_list(list, create_line(1, 0, 32));
    line_list *curr = list;

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
        free_line_list(list);
        return EXIT_FAILURE;
    } 

    int row, col;
    getmaxyx(stdscr, row, col);

    print_border(row, col);

    attron(COLOR_PAIR(LINE_NO_PAIR));
    printw("1. ");
    attroff(COLOR_PAIR(LINE_NO_PAIR));

    int x, y;
    getyx(stdscr, y, x);
    //int curLine = 1;

    while ((ch = getch()) != 27) {
        if (ch == '\n') {
            ++y;
            move(y, 1);
            attron(COLOR_PAIR(LINE_NO_PAIR));
            printw("%d. ", y);
            attroff(COLOR_PAIR(LINE_NO_PAIR));
            add_to_end_of_list(list, create_line(y, 0, 32));
            curr = curr->next;
            print_screen(row, col, x, y, list);
        } else if (ch == KEY_UP) {
            if (y > 1) {
                --y;
                curr = curr->prev;
                x = strlen(curr->value->value) + 4;
                move(y, x);
            }
        } else {
            char c = (char)ch;
            strncat(curr->value->value, &c, 1);
            printw("%c", ch);
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

void print_screen(int row, int col, int x, int y, line_list *list) {
    clear();
    print_border(row, col);
    line_list *curr = list;
    while (curr->next != NULL) {
        attron(COLOR_PAIR(LINE_NO_PAIR));
        printw("%d. ", curr->value->lineNo);
        attroff(COLOR_PAIR(LINE_NO_PAIR));
        printw("%s", curr->value->value);
        move(curr->value->lineNo + 1, 1);
        curr = curr->next;
    }
    attron(COLOR_PAIR(LINE_NO_PAIR));
    printw("%d. ", curr->value->lineNo);
    attroff(COLOR_PAIR(LINE_NO_PAIR));
    printw("%s", curr->value->value);
    move(y, x);
}
