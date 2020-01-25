#include <ncurses.h>
#include <string.h>

#include "line.h"

void print_border(int row, int col);
void print_screen(int row, int col, int x, int y, line_list *list);

int main(int argc, char **argv) {
    line_list *list = create_line_list();
    add_to_end_of_list(list, create_line(1, 0, 32));
    line_list *curr = list;

    char ch;

	initscr();			// Start curses mode 		
	raw();				// Line buffering disabled	
	keypad(stdscr, TRUE);		// We get F1, F2 etc..		
	noecho();			// Don't echo() while we do getch 

    int row, col;
    getmaxyx(stdscr, row, col);

    print_border(row, col);

    printw("1. ");
    int x, y;
    getyx(stdscr, y, x);
    //int curLine = 1;

    while ((ch = getch()) != 27) {
        if (ch == '\n') {
            ++y;
            move(y, 1);
            printw("%d. ", y);
            add_to_end_of_list(list, create_line(y, 0, 32));
            curr = curr->next;
            print_screen(row, col, x, y, list);
        } else {
            strncat(curr->value->value, &ch, 1);
            printw("%c", ch);
        }
    }
    
	endwin();			// End curses mode		  
    
    free_line_list(list);

/*
    int ch;

	initscr();			// Start curses mode 		
	raw();				// Line buffering disabled	
	keypad(stdscr, TRUE);		// We get F1, F2 etc..		
	noecho();			// Don't echo() while we do getch 

    int row, col;
    getmaxyx(stdscr, row, col);

    print_border(row, col);

    printw("1. ");
    int x, y;
    getyx(stdscr, y, x);
    int curLine = 1;

    while ((ch = getch()) != 27) {
        if (ch == '\n') {
            ++y;
            move(y, 1);
            printw("%d. ", y);
        } else {
            printw("%c", ch);
        }
    }
    
	endwin();			// End curses mode		  
*/
	return 0;
}

void print_border(int row, int col) {
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
    move(1, 1);
}

void print_screen(int row, int col, int x, int y, line_list *list) {
    clear();
    print_border(row, col);
    line_list *curr = list;
    while (curr->next != NULL) {
        printw("%d. %s", curr->value->lineNo, curr->value->value);
        move(curr->value->lineNo + 1, 1);
        curr = curr->next;
    }
    printw("%d. %s", curr->value->lineNo, curr->value->value);
    move(y, x);
}
