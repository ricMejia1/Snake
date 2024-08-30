#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>  // For usleep()

#define WIDTH 20
#define HEIGHT 20

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;

    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);  // Non-blocking getch() with 100ms delay
}

void Draw() {
    clear();

    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");
    printw("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printw("#");

            if (i == y && j == x)
                printw("O");
            else if (i == fruitY && j == fruitX)
                printw("F");
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printw("o");
                        print = 1;
                    }
                }
                if (!print)
                    printw(" ");
            }

            if (j == WIDTH - 1)
                printw("#");
        }
        printw("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");
    printw("\n");

    printw("Score: %d\n", score);
    refresh();
}

void Input() {
    int ch = getch();
    switch (ch) {
    case 'a':
        dir = LEFT;
        break;
    case 'd':
        dir = RIGHT;
        break;
    case 'w':
        dir = UP;
        break;
    case 's':
        dir = DOWN;
        break;
    case 'x':
        gameOver = 1;
        break;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(100000);  // sleep for 100ms
    }
    endwin();  // End ncurses mode
    return 0;
}
