#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<time.h>

struct board {
    char cells[10][25];
    int orders[10][25];
    int playerX;
    int playerY;
    int length;
};
typedef struct board Board;


struct position { int row, col; };
typedef struct position Position;


//----MANUALLY ADDED FUNCTIONSSS
char *alphabet = "wasd";

char *cToStr(char c){ // converts char to string
  static char s[2];
  s[0] = c;
  s[1] = '\0';
  return s;
}
int indexOf(char *list, char item) { //returns index of given char in string
  return strcspn(list, cToStr(item));
}
bool valid(char *str){ //determines  if string is in the alphabet
  for (int i = 0; i < strlen(str); i++) {
    if (indexOf(alphabet,str[i]) == strlen(alphabet)) {
      return false;
    }
  }
  return true;
}
int cToInt(char c){
  return c - '0';
}
//----END OF MANUALLY ADDED FUNCTIONS

Position newPosition(int row, int col){
  Position p;
  p.row = row;
  p.col = col;
  return p;
}

// Display the board.
void display(Board *b) {
  fprintf(stderr, "\n");
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 25; j++) {
      fprintf(stderr, "%c",b->cells[i][j]);
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

//Places new food assuming there is still remaining spaces
void placeNewFood(Board *b){
  srand(time(NULL));
  int row = rand() % 10;
  int col = rand() % 25;
  if (b->cells[row][col] == '.') {
    b->cells[row][col] = 'f';
  }
  else{
    placeNewFood(b);
  }
}

//Initializes a new game
void newGame(Board *b) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 25; j++) {
      b->cells[i][j] = '.';
      b->orders[i][j] = 0;
    }
  }
  b->cells[5][10] = 'o';
  b->orders[5][10] = 1;
  b->playerX = 5;
  b->playerY = 10;
  b->length = 1;
  placeNewFood(b);
}

void sweep(Board *b){
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 25; j++) {
      if (b->orders[i][j]>0) {
        b->orders[i][j]--;
      }
      if ((b->cells[i][j]=='o') && (b->orders[i][j] == 0)) {
        b->cells[i][j] = '.';
      }
    }
  }
}

void play(Board *b);
//Move
void move(Board *b, int row, int col){
  int i = b->playerX + row;
  int j = b->playerY + col;
  if (i < 10 && i > -1 && j > -1 && j < 25) {

  char next = b->cells[i][j];
  if (next == '.') {
    sweep(b);
    b->cells[i][j] = 'o';
    b->orders[i][j] = b->length;
    b->playerX = i;
    b->playerY = j;
    display(b);
    play(b);
  }
  else if (next == 'f') {
    b->cells[i][j] = 'o';
    b->length = b->length + 1;
    b->orders[i][j] = b->length;
    b->playerX = i;
    b->playerY = j;
    if (b->length == (10*25)) {
      fprintf(stderr, "You win. \n");

    }
    else{
      placeNewFood(b);
      display(b);
      play(b);
    }
  }
  else {
    fprintf(stderr, "\n");
    fprintf(stderr, "You hit yourself. You lost.\n");
    fprintf(stderr, "\n");

  }
}
  else {
    fprintf(stderr, "\n");
    fprintf(stderr, "You hit the wall. You lost.\n");
    fprintf(stderr, "\n");
  }
}

void play(Board *b){
  char line[10];
  if (fgets(line,sizeof(line),stdin)){
      switch (line[0]) {
        case 'w':
          move(b,-1,0);
          break;
        case 'a':
          move(b,0,-1);
          break;
        case 's':
          move(b,1,0);
          break;
        case 'd':
          move(b,0,1);
          break;
        default:
          display(b);
          break;
      }
    }

}

int main()
{
  setbuf(stdout, NULL);
  Board bdata;
  Board *board = &bdata;
  newGame(board);
  fprintf(stderr, "\n");
  fprintf(stderr, "Use wasd and press enter to move the snake.\n");
  fprintf(stderr, "\n");
  display(board);
  play(board);
  fprintf(stderr, "\n");
  return 0;
}
