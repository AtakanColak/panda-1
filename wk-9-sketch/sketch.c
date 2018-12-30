#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "display.h"
#include "alphabet.h"

#define DX 0
#define DY 1
#define DT 2
#define PEN 3
#define CLEAR 4
#define KEY 5
#define COL 6

struct state {
  bool onExtension;
  bool penDown;
  int currentX,currentY,previousX,previousY, rgba_;
  int rgba[4];
  int countdown, currentByte, opcode, operand;
};
typedef struct state State;

//void line(display *d, int x0, int y0, int x1, int y1);
//void pause(display *d, int ms);
//void cline(display *d, int x0, int y0, int x1, int y1, int rgba);

//Empty state constructor
State *newState(){
  State *s = malloc(sizeof(State));
  s->onExtension = false;
  s->penDown = false;
  s->currentX = 0;
  s->currentY = 0;
  s->previousX = 0;
  s->previousY = 0;
  s->countdown = 0;
  s->opcode = 0;
  s->operand = 0;
  s->currentByte = 0;
  s->rgba_ = 0x000000FF;
  s->rgba[0] = 0;
  s->rgba[1] = 0;
  s->rgba[2] = 0;
  s->rgba[3] = 255;
  return s;
}

//Updates the state according to its row and col numbers
void refleshState(State * s, int row, int col) {
  s->onExtension = false;
  s->penDown = false;
  s->currentX = (col * 30) + 20;
  s->previousX = s->currentX;
  s->currentY = (row * 45) + 20;
  s->previousY = s->currentY;
  s->opcode = 0;
  s->operand = 0;
  s->currentByte = 0;
  s->rgba_ = 0x000000FF;
  s->rgba[0] = 0;
  s->rgba[1] = 0;
  s->rgba[2] = 0;
  s->rgba[3] = 255;
}

int packRGBA(int * rgba){
  int pack = 0;
  for (int i = 0; i < 4; i++) {
    pack = (pack << 8) | rgba[i];
  }
  return pack;
}

//Returns hex of a 8-bit int as a char[2]
char *toHex(int n){
  char *space = malloc(sizeof(char[2]));
  char *alphabet = "0123456789ABCDEF";
  int a;
  a = ((n >> (4)) & 0xF);
  space[0] = alphabet[a];
  a = (n & 0xF);
  space[1] = alphabet[a];
  return space;
}

//Returns the opCode of a byte
void opCode(int a, State *s) {
  if (s->onExtension == false) {
    int n = ((a >> 6) & 0x03);
    if (n == 3) {
      n = a & 0x07;
      int e = ((a >> 4) & 0x03);
      if (e == 3) { e = 4; }
      s->onExtension = true;
      s->operand = 0;
      s->currentByte = 0;
      s->countdown = e;
      printf(" { \n ");
    }
    s->opcode = n;
    printf("%d ", s->opcode);
  }
  else {
    printf("   ");
  }
}

//Returns the operand of a byte
void opErand(int a,State *s) {
    if (s->opcode == DT) {
      if (a < 0 ) { a = 128 - a; }
    }
    if ((s->onExtension == false)) {
      a = a & 0x3F;
      if (((a >> 5) & 0x01) && s->opcode != DT) { a = -(64-a);}
      s->operand = a;
    }
    else if((s->currentByte == 0)) {
      s->operand = 0;
    }
    else {
      if (s->opcode != COL && s->opcode != DT) {
        if (((a >> 6) & 0x01) == 1 ) { a = a - 255;}
      }
      s->operand = (s->operand << 8) | a;
    }
    printf("%d\n", s->operand);
}

void dx(int a, State *s, display *d) {
  s->previousX = s->currentX;
  s->currentX = s->currentX + a;
}

void dy(int a, State *s, display *d) {
  s->previousY = s->currentY;
  s->currentY = s->currentY + a;
  if (s->penDown) {
    if (s->rgba_ == 0x000000FF) {
      line(d, s->previousX,s->previousY,s->currentX,s->currentY);
    }
    else {
      cline(d, s->previousX,s->previousY,s->currentX,s->currentY,s->rgba_);
    }
  }
  s->previousX = s->currentX;
}

void dt(int a, display *d) {
  pause(d,a);
}

void pen(State *s) {
  s->penDown = !s->penDown;
}

void color(int a, State *s, display *d) {
  //rgba(102, 255, 204, 255)
  s->rgba[s->currentByte-1] = a;
  if (s->countdown == 0) {
    if ((packRGBA(s->rgba) != 1728040191)) {
      s->rgba_ = packRGBA(s->rgba);
    }
    else {
      printf("Yep nothing wring until here.\n");
      dx(-(s->currentX + 5),s,d);
      dy(35,s,d);
    }
  }
}

void operate(int a, int opcode, int operand, State *s, display *d) {
  if((operand < 0) && s->onExtension) { operand = operand - 1;}
  if (opcode == DX) { dx(operand,s,d); }
  else if (opcode == DY) { dy(operand,s,d); }
  else if (opcode == DT) { dt(operand,d); }
  else if (opcode == PEN) { pen(s); }
  else if (opcode == CLEAR) { clear(d); }
  else if (opcode == KEY) { key(d); }
  else if (opcode == COL) { color(a,s,d);}
  else { printf("opcode %d is not recognized.\n",opcode); }
  s->countdown = 0;
  s->currentByte = 0;
  s->opcode = 0;
  s->operand = 0;
  if (s->onExtension) {
    s->onExtension = false;
    printf(" }\n");
  }
  printf("--------\n");
}

void update(int a, State *s, display *d) {

  opCode(a,s);
  opErand(a,s);

  if ((s->onExtension == false) || (s->countdown == 0)) {
    operate(a,s->opcode,s->operand,s,d);
  }
  else {
    if ((s->opcode == COL) && (s->currentByte != 0)) { color(a,s,d);}
    s->countdown = s->countdown - 1;
    s->currentByte = s->currentByte + 1;
  }
}

void createSketch(int length, int * bytes, char * filename) {
  FILE *out = fopen(filename,"wb");
  for (int i = 0; i < length; i++) {
    fputc(*bytes,out);
    bytes++;
  }
  fclose(out);
}

void interpret(FILE *in, display *d, State * s) {
    unsigned char b = fgetc(in);
    while (! feof(in)) {
      update(b, s, d);
      b = fgetc(in);
    }
    fclose(in);
    printf("\n");
}

// Read sketch instructions from the given file.  If test is NULL, display the
// result in a graphics window, else check the graphics calls made.
void run(char *filename, char *test[]) {
    FILE *in = fopen(filename, "rb");
    if (in == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        exit(1);
    }
    display *d = newDisplay(filename, 500, 500, test);
    State * s = newState();
    interpret(in,d,s);
    end(d);
}

void write(int len, char string[len], char * filename) {

    int byteLength = 0;
    byteLength += 2; // Lenght increase of initial dx 20 dy 20
    //Step 1: Iterate through all of the chars and get the byteLength
    for (int a = 0; a < len; a++) {
      char c = string[a];
      byteLength += (length(c) + 2);
    }
    byteLength += 12; //Length increase of pauses

    int byteArray[byteLength];
    int index = 0;
    //Step 2: Get the bytes to the constructed byte array
    byteArray[index] = bytify(DX,20);
    index++;
    byteArray[index] = bytify(DY,20);
    index++;

    for (int b = 0; b < len; b++) {
      char c = string[b];
      int cLen = length(c);
      int * charsBytes = crt(c);
      for (int e = 0; e < cLen; e++) {
        byteArray[index] = *charsBytes;
        charsBytes++;
        index++;
      }
      byteArray[index] = bytify(DX,25);
      index++;
      byteArray[index] = bytify(DY,0);
      index++;
    }

    //Step 2.2: Add the pauses at the end
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;
    byteArray[index] = bytify(DT,-1);
    index++;

    //Step 3: Save it to the given filename
    createSketch(byteLength, byteArray,filename);

    //Step 4: Read it
    run(filename,NULL);
}
// ----------------------------------------------------------------------------
// Nothing below this point needs to be changed.
// ----------------------------------------------------------------------------

// Forward declaration of test data.
char **lineTest, **squareTest, **boxTest, **oxoTest, **diagTest, **crossTest,
     **clearTest, **keyTest, **pausesTest, **fieldTest, **lawnTest;

void testSketches() {
    // Stage 1
    run("line.sketch", lineTest);
    run("square.sketch", squareTest);
    run("box.sketch", boxTest);
    run("oxo.sketch", oxoTest);

    // Stage 2
    run("diag.sketch", diagTest);
    run("cross.sketch", crossTest);

    // Stage 3
    run("clear.sketch", clearTest);
    run("key.sketch", keyTest);

    // Stage 4
    run("pauses.sketch", pausesTest);
    run("field.sketch", fieldTest);
    run("lawn.sketch", lawnTest);
}

int main(int n, char *args[n]) {
    if (n == 1) testSketches();
    else if (n == 2) {
      char *input = args[1];
      if (strcmp(input,"testExtension") == 0) {
        //write(5,"hello","teachersday.sketch");
        char * test = "happy teachers day#ian#it was teachers#day in turkey";
        write(strlen(test),test,"test.sketch");
      }
      else {
        run(args[1], NULL);
      }

    }
    else if (n == 3) {
        char *input = args[1];
        int length = strlen(args[1]);
        char * filename = args[2];
        if (validString(input)) {
          write(length, input, filename);
        }
        else {
          fprintf(stderr, "Please enter only the characters in the alphabet.\n");
        }

    }
    else {
        fprintf(stderr, "Usage: ./sketch [file.sketch]\n");
        fprintf(stderr, "or  ./sketch <input> <filename>\n");
        fprintf(stderr, "or  ./sketch testExtension\n");
        exit(1);
    }
}

// Each test is a series of calls, stored in a variable-length array of strings,
// with a NULL terminator.

// The calls that should be made for line.sketch.
char **lineTest = (char *[]) {
    "line(d,30,30,60,30)", NULL
};

// The calls that should be made for square.sketch.
char **squareTest = (char *[]) {
    "line(d,30,30,60,30)", "line(d,60,30,60,60)",
    "line(d,60,60,30,60)","line(d,30,60,30,30)", NULL
};

// The calls that should be made for box.sketch.
char **boxTest = (char *[]) {
    "line(d,30,30,32,30)", "pause(d,10)", "line(d,32,30,34,30)", "pause(d,10)",
    "line(d,34,30,36,30)", "pause(d,10)", "line(d,36,30,38,30)", "pause(d,10)",
    "line(d,38,30,40,30)", "pause(d,10)", "line(d,40,30,42,30)", "pause(d,10)",
    "line(d,42,30,44,30)", "pause(d,10)", "line(d,44,30,46,30)", "pause(d,10)",
    "line(d,46,30,48,30)", "pause(d,10)", "line(d,48,30,50,30)", "pause(d,10)",
    "line(d,50,30,52,30)", "pause(d,10)", "line(d,52,30,54,30)", "pause(d,10)",
    "line(d,54,30,56,30)", "pause(d,10)", "line(d,56,30,58,30)", "pause(d,10)",
    "line(d,58,30,60,30)", "pause(d,10)", "line(d,60,30,60,32)", "pause(d,10)",
    "line(d,60,32,60,34)", "pause(d,10)", "line(d,60,34,60,36)", "pause(d,10)",
    "line(d,60,36,60,38)", "pause(d,10)", "line(d,60,38,60,40)", "pause(d,10)",
    "line(d,60,40,60,42)", "pause(d,10)", "line(d,60,42,60,44)", "pause(d,10)",
    "line(d,60,44,60,46)", "pause(d,10)", "line(d,60,46,60,48)", "pause(d,10)",
    "line(d,60,48,60,50)", "pause(d,10)", "line(d,60,50,60,52)", "pause(d,10)",
    "line(d,60,52,60,54)", "pause(d,10)", "line(d,60,54,60,56)", "pause(d,10)",
    "line(d,60,56,60,58)", "pause(d,10)", "line(d,60,58,60,60)", "pause(d,10)",
    "line(d,60,60,58,60)", "pause(d,10)", "line(d,58,60,56,60)", "pause(d,10)",
    "line(d,56,60,54,60)", "pause(d,10)", "line(d,54,60,52,60)", "pause(d,10)",
    "line(d,52,60,50,60)", "pause(d,10)", "line(d,50,60,48,60)", "pause(d,10)",
    "line(d,48,60,46,60)", "pause(d,10)", "line(d,46,60,44,60)", "pause(d,10)",
    "line(d,44,60,42,60)", "pause(d,10)", "line(d,42,60,40,60)", "pause(d,10)",
    "line(d,40,60,38,60)", "pause(d,10)", "line(d,38,60,36,60)", "pause(d,10)",
    "line(d,36,60,34,60)", "pause(d,10)", "line(d,34,60,32,60)", "pause(d,10)",
    "line(d,32,60,30,60)", "pause(d,10)", "line(d,30,60,30,58)", "pause(d,10)",
    "line(d,30,58,30,56)", "pause(d,10)", "line(d,30,56,30,54)", "pause(d,10)",
    "line(d,30,54,30,52)", "pause(d,10)", "line(d,30,52,30,50)", "pause(d,10)",
    "line(d,30,50,30,48)", "pause(d,10)", "line(d,30,48,30,46)", "pause(d,10)",
    "line(d,30,46,30,44)", "pause(d,10)", "line(d,30,44,30,42)", "pause(d,10)",
    "line(d,30,42,30,40)", "pause(d,10)", "line(d,30,40,30,38)", "pause(d,10)",
    "line(d,30,38,30,36)", "pause(d,10)", "line(d,30,36,30,34)", "pause(d,10)",
    "line(d,30,34,30,32)", "pause(d,10)", "line(d,30,32,30,30)", "pause(d,10)",
    NULL
};

// The calls that should be made for box.sketch.
char **oxoTest = (char *[]) {
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,40,60,40)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,50,60,50)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,40,30,40,60)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,50,30,50,60)", NULL
};

// The calls that should be made for diag.sketch.
char **diagTest = (char *[]) {
    "line(d,30,30,60,60)", NULL
};

// The calls that should be made for cross.sketch.
char **crossTest = (char *[]) {
    "line(d,30,30,60,60)", "line(d,60,30,30,60)", NULL
};

// The calls that should be made for clear.sketch.
char **clearTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for key.sketch.
char **keyTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "key(d)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for diag.sketch.
char **pausesTest = (char *[]) {
    "pause(d,0)", "pause(d,0)", "pause(d,127)", "pause(d,128)", "pause(d,300)",
    "pause(d,0)", "pause(d,71469)", NULL
};

// The calls that should be made for field.sketch.
char **fieldTest = (char *[]) {
    "line(d,30,30,170,30)", "line(d,170,30,170,170)",
    "line(d,170,170,30,170)", "line(d,30,170,30,30)", NULL
};

// The calls that should be made for field.sketch.
char **lawnTest = (char *[]) {
    "cline(d,30,30,170,30,16711935)", "cline(d,170,30,170,170,16711935)",
    "cline(d,170,170,30,170,16711935)", "cline(d,30,170,30,30,16711935)",
    NULL
};
