
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define INIT_SNAKE_LEN 5
#define MAX_SNAKE_LEN 32
#define INIT_SNAKE_X 8
#define INIT_SNAKE_Y 5

//global array of direction vectors
int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

/*
*   Get Dimensions of current window
*/
int *get_dimension(){
    // array for window dimensions
    static int dimensions[2];
    struct winsize wbuf;
    if (ioctl(0, TIOCGWINSZ, &wbuf) != -1){
        dimensions[0] = wbuf.ws_col;
        dimensions[1] = wbuf.ws_row;
    }
    return dimensions;
}

/*
*   Vector struct that contains x and y coords
*/
typedef struct{
    int x;
    int y;
} Vector;

/*
*   This initializes a Vector struct
*/
Vector* init_vector(int x, int y){
    Vector *vector = malloc(sizeof(*vector));
    vector->x = x;
    vector->y = y;
    return vector;
}

/*
*   Snake struct contains array of vectors to store coords for each cell of the snake.
*   Snake struct stores length of snake.
*   Snake Struct stores current direction of snake.
*/
typedef struct{
    Vector body[MAX_SNAKE_LEN];
    int length;
    Vector direction;
} Snake;

/*
*   Initialize the Snake
*/
Snake* init_snake(){

    // Allocate memory in heap for the snake struct
    Snake *snake = malloc(sizeof(*snake));
    snake->length = INIT_SNAKE_LEN;

    // Snake head coords
    snake->body[0].x = INIT_SNAKE_X;
    snake->body[0].y = INIT_SNAKE_Y;

    // Snake direction
    snake->direction.x = 1;
    snake->direction.y = 0;

    // Construct rest of body in opposite of direction vector
    for(int i = 1; i < snake->length; i++){
        snake->body[i].x = snake->body[i-1].x + (snake->direction.x *-1);
        snake->body[i].y = snake->body[i-1].y + (snake->direction.y *-1);
    }

    return snake;
}

/*
*   Move the snake by building a new vector at the position the snake will be in the next frame.
*   Shift the vector objects that make up the body of the snake by one and drop the last
*/
void move_snake(Snake *snake){
    Vector *head = init_vector(snake->body[0].x + snake->direction.x, snake->body[0].y + snake->direction.y);

    // Make room in the snake body array for the new head vector
    for(int i = snake->length; i > 0; i--){
        snake->body[i] = snake->body[i-1];
    }
    snake->body[0] = *head;
}

/*
*   Change the direction of the snake 
*/
void change_direction(Snake *snake, int x, int y){
    snake->direction.x = x;
    snake->direction.y = y;
}

/*
*   Draw a char for each vector of the snake body @ current position
*/
void draw_snake(Snake *snake){
    for(int i = 0; i < snake->length; i++){
        mvaddch(snake->body[i].y, snake->body[i].x, ACS_BLOCK);
    }
}

/*
*   Draw the border for the snake pit
*/
void draw_pit(int width, int height){
    int i,j;
    for (i = 0; i < height-1; i++){
        if (i == 0 || i == height-2){
            for (j = 0; j < width; j++){
                mvaddch(i, j, '-');
            }
        }
        else {
            mvaddch(i, 0, '|');
            mvaddch(i, width-1, '|');
        }
    }
}

void draw_score(Snake *snake, int x, int y){
    mvprintw(y, x,"Length: %d", snake->length);
}

void init_game(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(25);
}

int main(){
    int input = 0;              // User input
    int x = 1, y = 0;           // Direction x and y
    int *dim = get_dimension(); // Get the dimensions of the window
    

    Snake *snake = init_snake();    // Initialize the snake
    printf("snake initialized... game start in 1 sec...\n");  
    sleep(1);
    init_game();                    // Initialize game window

    while(input != 'q'){

        // Update the game screen
        erase();
        draw_pit(dim[0], dim[1]);
        draw_snake(snake);
        draw_score(snake, 5, dim[1]-1);
        refresh();
        

        input = getch();        // Get user input and modify snake direction
        switch (input) {
            case KEY_UP:
                y = -1;
                x = 0;
                break;
            case KEY_DOWN:
                y = 1;
                x = 0;
                break;
            case KEY_LEFT:
                x = -1;
                y = 0;
                break;
            case KEY_RIGHT:
                x = 1;
                y = 0;
                break;
        }

        flushinp();                     // Flush out the user input buffer in case key held

        change_direction(snake, x, y);  // Update the snake direction
        move_snake(snake);              // Update the snake state

        usleep(95000);                  // Delay
    }
    
    endwin();

    return 0;
}