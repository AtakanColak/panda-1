#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct cell {
  struct cell * prevCell;
  struct cell * nextCell;
  char item[];
};
typedef struct cell Cell;

static Cell Sentinel ,*sentinel;

static void initializeSentinel() {
  sentinel = &Sentinel;
  sentinel->nextCell = sentinel;
  sentinel->prevCell = sentinel;
}

struct list {
  int itemsize;
  int length;
  bool empty;
  Cell *first;
  Cell *current;
  Cell *last;
};
typedef struct list list;

static Cell * newCell(int itemsize) {
  Cell * myCell = malloc(sizeof(Cell) + itemsize);
  return myCell;
}

static Cell * firstCell(int itemsize) {
  Cell * myCell = newCell(itemsize);
  myCell->prevCell = sentinel;
  myCell->nextCell = sentinel;
  return myCell;
}

list * newList(int b) {
  initializeSentinel();
  int mySize = sizeof(list);
  list * myList = malloc(mySize);
  myList->first = firstCell(b);
  myList->current = myList->first;
  myList->last = myList->first;
  myList->itemsize = b;
  myList->length = 1;
  myList->empty = true;
  return myList;
}


void start(list *l) {
  l->current = l->first;
}
void end(list *l) {
  l->current = l->last;
}

bool atStart(list *l) {
  return l->current == l->first;
}
bool atEnd(list *l) {
  return l->current == l->last;
}

void forward(list *l) {
  Cell * current = l->current;
  if (!atEnd(l)) { l->current = current->nextCell; }
  else { fprintf(stderr,"Can't go forward; This is the last cell.\n"); exit(1); }
}
void backward(list *l) {
  Cell * current = l->current;
  if (!atStart(l)) { l->current = current->prevCell; }
  else { fprintf(stderr,"Can't go backward; This is the first cell.\n"); exit(1); }
}

void insertBefore(list *l, void *p) {
  Cell * current = l->current;
  if (l->empty) {
    memcpy(current->item,p,l->itemsize);
    l->empty = false;
  }
  else {
    Cell * previous = current->prevCell;

    Cell * newlyCell = newCell(l->itemsize);
    if (previous == sentinel) {
      l->first = newlyCell;
    }

    newlyCell->prevCell = previous;
    newlyCell->nextCell = current;

    memcpy(newlyCell->item,p,l->itemsize);

    previous->nextCell = newlyCell;
    current->prevCell = newlyCell;

    l->length = l->length + 1;
  }
}
void insertAfter(list *l, void *p) {
  Cell * current = l->current;
  if (l->empty) {
    memcpy(current->item,p,l->itemsize);
    l->empty = false;
  }
  else {
    Cell * next = current->nextCell;

    Cell * newlyCell = newCell(l->itemsize);

    if (next == sentinel) {
      l->last = newlyCell;
    }

    newlyCell->prevCell = current;
    newlyCell->nextCell = next;

    memcpy(newlyCell->item,p,l->itemsize);

    current->nextCell = newlyCell;
    next->prevCell = newlyCell;

    l->length = l->length + 1;


  }
}

void getBefore(list *l, void *p) {
  if (l->empty) {
    fprintf(stderr,"Can't get before; The list is empty.\n"); exit(1);
  }
  else if (atStart(l)) {
    fprintf(stderr,"Can't get before; This is the first cell.\n"); exit(1);
  }
  else {
    Cell * current = l->current;
    Cell * previous = current->prevCell;
    memcpy(p,previous->item,l->itemsize);
  }
}
void getAfter(list *l, void *p) {
    if (l->empty) {
      fprintf(stderr,"Can't get after; The list is empty.\n"); exit(1);
    }
    else if (atEnd(l)) {
      fprintf(stderr,"Can't get after; This is the last cell.\n"); exit(1);
    }
    else {
      Cell * current = l->current;
      Cell * next = current->nextCell;
      memcpy(p,next->item,l->itemsize);
    }
}

void setBefore(list *l, void *p) {
  if (l->empty) {
    fprintf(stderr,"Can't set before; The list is empty.\n"); exit(1);
  }
  else if (atStart(l)) {
    fprintf(stderr,"Can't set before; This is the first cell.\n"); exit(1);
  }
  else {
    Cell * current = l->current;
    Cell * previous = current->prevCell;
    memcpy(previous->item,p,l->itemsize);
  }
}
void setAfter(list *l, void *p) {
  if (l->empty) {
    fprintf(stderr,"Can't set after; The list is empty.\n"); exit(1);
  }
  else if (atEnd(l)) {
    fprintf(stderr,"Can't set after; This is the last cell.\n"); exit(1);
  }
  else {
    Cell * current = l->current;
    Cell * next = current->nextCell;
    memcpy(next->item,p,l->itemsize);
  }
}

void deleteBefore(list *l) {
  if (l->empty) {
    fprintf(stderr,"Can't delete before; The list is empty.\n"); exit(1);
  }
  else if (l->length == 1) {
    l->empty = true;
  }
  else if (atStart(l)) {
    fprintf(stderr,"Can't delete before; This is the first cell.\n"); exit(1);
  }
  else
  {
    Cell * current = l->current;
    Cell * previous = current->prevCell;
    Cell * preprevious = previous->prevCell;

    preprevious->nextCell = current;
    current->prevCell = preprevious;

    free(previous);

    l->length = l->length - 1;
  }

}
void deleteAfter(list *l) {
  if (l->empty) {
    fprintf(stderr,"Can't delete after; The list is empty.\n"); exit(1);
  }
  else if (l->length == 1) {
    l->empty = true;
  }
  else if (atEnd(l)) {
    fprintf(stderr,"Can't delete after; This is the last cell.\n"); exit(1);
  }
  else {
    Cell * current = l->current;
    Cell * next = current->nextCell;
    Cell * nexter = next->nextCell;

    current->nextCell = nexter;
    nexter->prevCell = current;

    free(next);

    l->length = l->length - 1;
  }


}

//Unit testing that tests insert and at...functions

static void test(bool result,int *n) {
  if (!result) { printf("Test %d failed. \n", *n);}
  else {
    *n = *n + 1;
    printf("Test %d succeeded. \n", *n);
  }
}

static void assertTrue(bool expected, char* message){
  if(!expected) {
    printf("Assertion failed:%s\n", message);
    exit(1);
  }
}

//TEST 1: Tests insertAfter, atStart, atEnd, start and ends with a char list
static bool test1() {
  list *l = newList(sizeof(char));
  assertTrue(atStart(l) && atEnd(l), "Should be at start and end...");
  char i = '1';
  insertAfter(l,&i);
  assertTrue(atStart(l) && atEnd(l), "One insert should be at start and end...");
  i = '2';
  insertAfter(l,&i);
  i = '5';
  insertAfter(l,&i);
  i = '4';
  insertAfter(l,&i);
  i = '3';
  insertAfter(l,&i);
  end(l);
  assertTrue(!atStart(l) && atEnd(l), "Should be at end...");
  start(l);
  assertTrue(atStart(l) && !atEnd(l), "Should be at start...");
  return true;
}

//TEST 2: Tests inserts, gets, backward and forward with an int list
static bool test2() {
  list *l = newList(sizeof(int));
  int i = 1;
  insertAfter(l,&i);
  i = 5;
  insertAfter(l,&i);
  getAfter(l,&i);
  assertTrue(i == 5, "Should be 2...");
  forward(l);
  backward(l);
  forward(l);
  getBefore(l,&i);
  assertTrue(i == 1, "Should be 2...");
  return true;
}

//TEST 3: Tests inserts, sets, gets, backward, forward with a string list
static bool test3() {
  char * lel = "kikikiki";
  list *l = newList(sizeof(lel) + 1);
  insertAfter(l,lel);
  lel = "hahahaha";
  insertAfter(l,lel);
  lel = "hehehehe";
  insertBefore(l,lel);
  assertTrue(!atEnd(l) && !atStart(l), "Should have been in the middle...");
  char myList[sizeof(lel)];
  getAfter(l,&myList);
  assertTrue(strcmp(myList,"hahahaha") == 0, "getAfter does not work");
  getBefore(l,&myList);
  assertTrue(strcmp(myList,"hehehehe") == 0, "getBefore does not work");
  forward(l);
  setBefore(l,"kekekeke");
  getBefore(l,&myList);
  assertTrue(strcmp(myList,"kekekeke") == 0, "getBefore does not work");
  backward(l);
  backward(l);
  setAfter(l,"hihihihi");
  getAfter(l,&myList);
  assertTrue(strcmp(myList,"hihihihi") == 0, "getAfter does not work");
  return true;
}

//TEST 4: Tests insert and deletes with a bool list
static bool test4() {
  bool bee = false;
  list *l = newList(sizeof(bee));
  insertAfter(l,&bee);
  insertAfter(l,&bee);
  insertAfter(l,&bee);
  deleteAfter(l);
  deleteAfter(l);
  deleteAfter(l);
  assertTrue(l->empty,"Afters don't work.");
  insertBefore(l,&bee);
  insertBefore(l,&bee);
  insertBefore(l,&bee);
  deleteBefore(l);
  deleteBefore(l);
  deleteBefore(l);
  assertTrue(l->empty,"Befores don't work.");
  return true;
}

int testLists() {
  int n = 0;
  test(test1(),&n);
  test(test2(),&n);
  test(test3(),&n);
  test(test4(),&n);
  fprintf(stdout, "All tests passed successfully!\n");
  return n;
}
