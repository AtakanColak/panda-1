/* Calculate a grade from a mark. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Check whether a mark is properly formatted.
bool valid(const char *mark) {
  int length = strlen (mark);

  //Special case for numeric input starting with 0
  if ((mark[0]=='0') && (length > 1)) {
    return false;
  }

  //Iterate through all characters to check if all of them are digits
  for (int i = 0; i < length ; i++) {
    int result = isdigit (mark[i]);
    if (result == 0) {
      return false;
    }
  }
  return true;
}

// Convert the mark into a grade
char *grade(const char *mark) {
  if (!valid(mark)) {
    return "Error: invalid mark";
  }
  int n = atoi(mark);
  if(n>100){
    return "Error: invalid mark";
  }
  else if (n >= 70){
    return "1";
  }
  else if (n >= 60){
    return "2.1";
  }
  else if (n >= 50){
    return "2.2";
  }
  else if (n >= 40){
    return "3";
  }
  else {
    return "Fail";
  }
}

// Increment and return the test number and check that two strings are equal.
int eq(int n, const char *grade, const char *expected) {
    n++;
    if (strcmp(grade, expected) != 0) {
        fprintf(stderr, "Test %d fails:\n", n);
        fprintf(stderr, "result %s instead of %s\n", grade, expected);
        exit(1);
    }
    return n;
}

// Run the tests.
void test() {
    int n = 0;
    n = eq(n, grade("0"), "Fail");
    n = eq(n, grade("39"), "Fail");
    n = eq(n, grade("40"), "3");
    n = eq(n, grade("49"), "3");
    n = eq(n, grade("50"), "2.2");
    n = eq(n, grade("59"), "2.2");
    n = eq(n, grade("60"), "2.1");
    n = eq(n, grade("69"), "2.1");
    n = eq(n, grade("70"), "1");
    n = eq(n, grade("100"), "1");
    n = eq(n, grade("40.5"), "Error: invalid mark");
    n = eq(n, grade("x"), "Error: invalid mark");
    n = eq(n, grade("-1"), "Error: invalid mark");
    n = eq(n, grade("101"), "Error: invalid mark");
    n = eq(n, grade("01"), "Error: invalid mark");
    printf("Tests passed: %d\n", n);
}

// Deal with input and output, and call either test or grade.
int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    if (n == 1) test();
    else if (n == 2) printf("%s\n", grade(args[1]));
    else {
        fprintf(stderr, "Use:   ./grade   or   ./grade m\n");
        fprintf(stderr, "where m is an integer mark from 0 to 100\n");
        exit(1);
    }
    return 0;
}
