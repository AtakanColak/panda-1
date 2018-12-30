#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *input;
char *keyword;

int lengthOfInput(){
  return strlen(input);
}

int lengthOfAlphabet(){
  return strlen(alphabet);
}

int lengthOfKeyword(){
  return strlen(keyword);
}

char *convertToUpper(char *str){
  for (int i = 0; i < strlen(str); i++) {
    str[i] = toupper(str[i]);
  }
  return str;
}

char *convertToString(char c){
  static char s[2];
  s[0] = c;
  s[1] = '\0';
  return s;
}

int indexOf(char *list, char item) {
  return strcspn(list, convertToString(item));
}

void encrypt(){
    int oldIndex,addIndex,newIndex;
    for (int i = 0; i < lengthOfInput(); i++) {
      oldIndex = indexOf(alphabet,input[i]);
      addIndex = indexOf(alphabet,keyword[i % lengthOfKeyword()]);
      newIndex = (oldIndex + addIndex) % lengthOfAlphabet();
      input[i] = alphabet[newIndex];
    }
}

void decrypt(){
  int oldIndex,addIndex,newIndex;
  for (int i = 0; i < lengthOfInput(); i++) {
    newIndex = indexOf(alphabet,input[i]);
    addIndex = indexOf(alphabet,keyword[i % lengthOfKeyword()]);
    oldIndex = newIndex - addIndex;
    if (oldIndex < 0) {
      input[i] = alphabet[lengthOfAlphabet() + oldIndex];
    }
    else {
      input[i] = alphabet[oldIndex];
    }

  }
}

bool valid(char *str){
  int lengthOfStr = strlen(str);
  for (int i = 0; i < lengthOfStr; i++) {
    if (indexOf(alphabet,str[i]) == lengthOfAlphabet()) {
      return false;
    }
  }
  return true;
}

int main(int n, char **argv) {
    setbuf(stdout, NULL);
    if (n == 4) {
        input = argv[1];
        keyword = argv[2];
        char c = convertToUpper(argv[3])[0];
        input = convertToUpper(argv[1]);
        keyword = convertToUpper(argv[2]);
        printf("\n");

        printf("Input is:%s\n", input);
        printf("Keyword is:%s\n", keyword);

        if (valid(input) && valid(keyword)) {
          printf("Words are within the alphabet.\n");
          printf("\n");
          if (strcmp(convertToString(c),"E") == 0) {
            encrypt();
            printf("Encrypted form is :%s\n", input);
          }
          else if (strcmp(convertToString(c),"D") == 0){
            decrypt();
            printf("Decrypted form is :%s\n", input);
          }
          else {
            printf("Please enter as follows ./viginere <input> <keyword> <E(for encrypt) or D(for decrypt)>");
          }
          printf("\n");
        }
        else {
          printf("Words are not in the alphabet, please only enter english letters.\n");
        }

     }
    else {
        printf("Please enter as follows ./viginere <input> <keyword> <E(for encrypt) or D(for decrypt)>");
        printf("\n");
        exit(1);
    }
    printf("\n");
    return 0;
}
