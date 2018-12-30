#define DX 0
#define DY 1
#define DT 2
#define PEN 3
#define CLEAR 4
#define KEY 5
#define COL 6

char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ #";
int lengths[] = {15, 15, 14, 16, 22, 16, 19, 14, 7, 12, 13, 7, 13, 11, 19, 16, 30, 19, 20, 11, 12, 8, 10, 10, 12, 10, 2, 5};

int length(char c) {
  if (c == ' ') { return 2; }
  else if (c == '#') { return 5; }
  else { return lengths[toupper(c)-65]; }
}

int totalLength() {
  int len = 0;
  for (int i = 0; i < strlen(alphabet); i++) {
    len += lengths[i];
    len++;
  }
  return len;
}

int bytify(int opcode, int operand){
    int myByte = opcode << 6;
    myByte = myByte | (operand & 0x3F);
    return myByte;
}

int penify() {
    return bytify(PEN,PEN);
}

bool validString(char *str){
  int lengthOfStr = strlen(str);
  for (int i = 0; i < lengthOfStr; i++) {
    char c = str[i];
    c = toupper(c);
    if (strcspn(alphabet,&c) == strlen(alphabet)) {
      if ((c != ' ') && (c != '#')) {
        return false;
      }
    }
  }
  return true;
}

int * unpackRGBA(int rgba){
  int * unpack = malloc(4 * sizeof(int));
  for (int i = 0; i < 4; i++) {
    *unpack = (rgba >> (8*(3-i))) & 0xFF;
    unpack++;
  }
  return unpack;
}

int * crtA() {
  int * myBytes = (int *)malloc(15 * sizeof(int));
  myBytes[0] = bytify(DY,30);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,10);
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = bytify(DX,10);
  myBytes[5] = bytify(DY,30);
  myBytes[6] = penify();
  myBytes[7] = bytify(DX,-5);
  myBytes[8] = bytify(DY,-15);
  myBytes[9] = penify();
  myBytes[10] = bytify(DX,-10);
  myBytes[11] = bytify(DY,0);
  //Return to square one
  myBytes[12] = penify();
  myBytes[13] = bytify(DX,-5);
  myBytes[14] = bytify(DY,-15);
  return myBytes;
}

int * crtB() {
  int * myBytes = (int *)malloc(15 * sizeof(int));
  myBytes[0] = penify();

  myBytes[1] = bytify(DX,7);
  myBytes[2] = bytify(DY,0);

  myBytes[3] = bytify(DX,5);
  myBytes[4] = bytify(DY,5);

  myBytes[5] = bytify(DX,-5);
  myBytes[6] = bytify(DY,5);

  myBytes[7] = bytify(DX,10);
  myBytes[8] = bytify(DY,10);

  myBytes[9] = bytify(DX,-10);
  myBytes[10] = bytify(DY,10);

  myBytes[11] = bytify(DX,-7);
  myBytes[12] = bytify(DY,0);

  myBytes[13] = bytify(DY,-30);

  //Return to square one
  myBytes[14] = penify();

  return myBytes;
}

int * crtC() {
  int * myBytes = (int *)malloc(14 * sizeof(int));
  myBytes[0] = bytify(DX,20);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,-15);
  myBytes[3] = bytify(DY,0);
  myBytes[4] = bytify(DX,-5);
  myBytes[5] = bytify(DY,5);
  myBytes[6] = bytify(DY,20);
  myBytes[7] = bytify(DX,5);
  myBytes[8] = bytify(DY,5);
  myBytes[9] = bytify(DX,15);
  myBytes[10] = bytify(DY,0);

  //Return to square one
  myBytes[11] = penify();
  myBytes[12] = bytify(DX,-20);
  myBytes[13] = bytify(DY,-30);
  return myBytes;
}

int * crtD() {
  int * myBytes = (int *)malloc(16 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,10);
  myBytes[6] = bytify(DY,0);
  myBytes[7] = bytify(DX,10);
  myBytes[8] = bytify(DY,7);
  myBytes[9] = bytify(DY,16);
  myBytes[10] = bytify(DX,-10);
  myBytes[11] = bytify(DY,7);
  myBytes[12] = bytify(DX,-10);
  myBytes[13] = bytify(DY,0);
  //Return to square one
  myBytes[14] = penify();
  myBytes[15] = bytify(DY,-30);
  return myBytes;
}

int * crtE() {
  int * myBytes = (int *)malloc(22 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,20);
  myBytes[6] = bytify(DY,0);
  myBytes[7] = penify();
  myBytes[8] = bytify(DX,-20);
  myBytes[9] = bytify(DY,15);
  myBytes[10] = penify();
  myBytes[11] = bytify(DX,10);
  myBytes[12] = bytify(DY,0);
  myBytes[13] = penify();
  myBytes[14] = bytify(DX,-10);
  myBytes[15] = bytify(DY,15);
  myBytes[16] = penify();
  myBytes[17] = bytify(DX,20);
  myBytes[18] = bytify(DY,0);
  //Return to square one
  myBytes[19] = penify();
  myBytes[20] = bytify(DX,-20);
  myBytes[21] = bytify(DY,-30);
  return myBytes;
}

int * crtF() {
  int * myBytes = (int *)malloc(16 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,20);
  myBytes[6] = bytify(DY,0);
  myBytes[7] = penify();
  myBytes[8] = bytify(DX,-20);
  myBytes[9] = bytify(DY,15);
  myBytes[10] = penify();
  myBytes[11] = bytify(DX,10);
  myBytes[12] = bytify(DY,0);
  //Return to square one
  myBytes[13] = penify();
  myBytes[14] = bytify(DX,-10);
  myBytes[15] = bytify(DY,-15);
  return myBytes;
}

int * crtG() {
  int * myBytes = (int *)malloc(19 * sizeof(int));
  myBytes[0] = bytify(DX,20);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,-15);
  myBytes[3] = bytify(DY,0);
  myBytes[4] = bytify(DX,-5);
  myBytes[5] = bytify(DY,5);
  myBytes[6] = bytify(DY,20);
  myBytes[7] = bytify(DX,5);
  myBytes[8] = bytify(DY,5);
  myBytes[9] = bytify(DX,10);
  myBytes[10] = bytify(DY,0);

  myBytes[11] = bytify(DX,5);
  myBytes[12] = bytify(DY,-5);

  myBytes[13] = bytify(DY,-10);

  myBytes[14] = bytify(DX,-10);
  myBytes[15] = bytify(DY,0);
  //Return to square one
  myBytes[16] = penify();
  myBytes[17] = bytify(DX,-10);
  myBytes[18] = bytify(DY,-15);

  return myBytes;
}

int * crtH() {
  int * myBytes = (int *)malloc(14 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-15);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,20);
  myBytes[6] = bytify(DY,0);
  myBytes[7] = penify();
  myBytes[8] = bytify(DY,-15);
  myBytes[9] = penify();
  myBytes[10] = bytify(DY,30);
  //Return to square one
  myBytes[11] = penify();
  myBytes[12] = bytify(DX,-20);
  myBytes[13] = bytify(DY,-30);
  return myBytes;
}

int * crtI() {
  int * myBytes = (int *)malloc(7 * sizeof(int));
  myBytes[0] = bytify(DX,10);
  myBytes[1] = bytify(DY,0);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,30);
  //Return to square one
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,-10);
  myBytes[6] = bytify(DY,-30);
  return myBytes;
}

int * crtJ() {
  int * myBytes = (int *)malloc(12 * sizeof(int));
  myBytes[0] = bytify(DY,25);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,5);
  myBytes[3] = bytify(DY,5);
  myBytes[4] = bytify(DX,10);
  myBytes[5] = bytify(DY,0);
  myBytes[6] = bytify(DX,5);
  myBytes[7] = bytify(DY,-5);
  myBytes[8] = bytify(DY,-25);
  //Return to square one
  myBytes[9] = penify();
  myBytes[10] = bytify(DX,-20);
  myBytes[11] = bytify(DY,0);
  return myBytes;
}

int * crtK() {
  int * myBytes = (int *)malloc(13 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DX,20);
  myBytes[4] = bytify(DY,0);
  myBytes[5] = penify();
  myBytes[6] = bytify(DX,-20);
  myBytes[7] = bytify(DY,-15);
  myBytes[8] = bytify(DX,20);
  myBytes[9] = bytify(DY,-15);
  //Return to square one
  myBytes[10] = penify();
  myBytes[11] = bytify(DX,-20);
  myBytes[12] = bytify(DY,0);
  return myBytes;
}

int * crtL() {
  int * myBytes = (int *)malloc(7 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = bytify(DX,20);
  myBytes[3] = bytify(DY,0);
  //Return to square one
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,-20);
  myBytes[6] = bytify(DY,-30);
  return myBytes;
}

int * crtM() {
  int * myBytes = (int *)malloc(13 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,10);
  myBytes[6] = bytify(DY,30);
  myBytes[7] = bytify(DX,10);
  myBytes[8] = bytify(DY,-30);
  myBytes[9] = bytify(DY,30);
  //Return to square one
  myBytes[10] = penify();
  myBytes[11] = bytify(DX,-20);
  myBytes[12] = bytify(DY,-30);
  return myBytes;
}

int * crtN() {
  int * myBytes = (int *)malloc(11 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,20);
  myBytes[6] = bytify(DY,30);
  myBytes[7] = bytify(DY,-30);
  //Return to square one
  myBytes[8] = penify();
  myBytes[9] = bytify(DX,-20);
  myBytes[10] = bytify(DY,0);
  return myBytes;
}

int * crtO() {
  int * myBytes = (int *)malloc(19 * sizeof(int));
  myBytes[0] = bytify(DX,15);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,-10);
  myBytes[3] = bytify(DY,0);
  myBytes[4] = bytify(DX,-5);
  myBytes[5] = bytify(DY,5);
  myBytes[6] = bytify(DY,20);
  myBytes[7] = bytify(DX,5);
  myBytes[8] = bytify(DY,5);
  myBytes[9] = bytify(DX,10);
  myBytes[10] = bytify(DY,0);
  myBytes[11] = bytify(DX,5);
  myBytes[12] = bytify(DY,-5);
  myBytes[13] = bytify(DY,-20);
  myBytes[14] = bytify(DX,-5);
  myBytes[15] = bytify(DY,-5);
  //Return to square one
  myBytes[16] = penify();
  myBytes[17] = bytify(DX,-15);
  myBytes[18] = bytify(DY,0);
  return myBytes;
}

int * crtP() {
  int * myBytes = (int *)malloc(16 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,10);
  myBytes[6] = bytify(DY,0);
  myBytes[7] = bytify(DX,10);
  myBytes[8] = bytify(DY,4);
  myBytes[9] = bytify(DY,8);
  myBytes[10] = bytify(DX,-10);
  myBytes[11] = bytify(DY,4);
  myBytes[12] = bytify(DX,-10);
  myBytes[13] = bytify(DY,0);
  //Return to square one
  myBytes[14] = penify();
  myBytes[15] = bytify(DY,-16);
  return myBytes;
}

int * crtQ() {
  int * myBytes = (int *)malloc(30 * sizeof(int));
  myBytes[0] = bytify(DX,15);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,-10);
  myBytes[3] = bytify(DY,0);
  myBytes[4] = bytify(DX,-5);
  myBytes[5] = bytify(DY,5);
  myBytes[6] = bytify(DY,20);
  myBytes[7] = bytify(DX,5);
  myBytes[8] = bytify(DY,5);
  myBytes[9] = bytify(DX,10);
  myBytes[10] = bytify(DY,0);
  myBytes[11] = bytify(DX,5);
  myBytes[12] = bytify(DY,-5);
  myBytes[13] = bytify(DY,-20);
  myBytes[14] = bytify(DX,-5);
  myBytes[15] = bytify(DY,-5);
  myBytes[16] = penify();

  myBytes[17] = bytify(DX,5);
  myBytes[18] = bytify(DY,5);
  myBytes[19] = bytify(DY,20);
  myBytes[20] = bytify(DX,-2);
  myBytes[21] = bytify(DY,2);
  myBytes[22] = penify();

  myBytes[23] = bytify(DX,3);
  myBytes[24] = bytify(DY,3);

  myBytes[25] = bytify(DX,-5);
  myBytes[26] = bytify(DY,-5);

  //Return to square one
  myBytes[27] = penify();
  myBytes[28] = bytify(DX,-16);
  myBytes[29] = bytify(DY,-25);
  return myBytes;
}

int * crtR() {
  int * myBytes = (int *)malloc(19 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = penify();
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = penify();
  myBytes[5] = bytify(DX,10);
  myBytes[6] = bytify(DY,0);
  myBytes[7] = bytify(DX,10);
  myBytes[8] = bytify(DY,4);
  myBytes[9] = bytify(DY,8);
  myBytes[10] = bytify(DX,-10);
  myBytes[11] = bytify(DY,4);
  myBytes[12] = bytify(DX,-10);
  myBytes[13] = bytify(DY,0);
  myBytes[14] = bytify(DX,20);
  myBytes[15] = bytify(DY,14);
  //Return to square one
  myBytes[16] = penify();
  myBytes[17] = bytify(DX,-20);
  myBytes[18] = bytify(DY,-30);
  return myBytes;
}

int * crtS() {
  int * myBytes = (int *)malloc(20 * sizeof(int));
  myBytes[0] = bytify(DX,20);
  myBytes[1] = penify();
  myBytes[2] = bytify(DX,-15);
  myBytes[3] = bytify(DY,0);
  myBytes[4] = bytify(DX,-5);
  myBytes[5] = bytify(DY,5);

  myBytes[6] = bytify(DY,5);

  myBytes[7] = bytify(DX,5);
  myBytes[8] = bytify(DY,5);

  myBytes[9] = bytify(DX,10);
  myBytes[10] = bytify(DY,0);

  myBytes[11] = bytify(DX,5);
  myBytes[12] = bytify(DY,5);

  myBytes[13] = bytify(DY,5);

  myBytes[14] = bytify(DX,-5);
  myBytes[15] = bytify(DY,5);

  myBytes[16] = bytify(DX,-15);
  myBytes[17] = bytify(DY,0);

  //Return to square one
  myBytes[18] = penify();
  myBytes[19] = bytify(DY,-30);
  return myBytes;
}

int * crtT() {
  int * myBytes = (int *)malloc(11 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DX,20);
  myBytes[2] = bytify(DY,0);
  myBytes[3] = penify();
  myBytes[4] = bytify(DX,-10);
  myBytes[5] = bytify(DY,0);
  myBytes[6] = penify();
  myBytes[7] = bytify(DY,30);
  //Return to square one
  myBytes[8] = penify();
  myBytes[9] = bytify(DX,-10);
  myBytes[10] = bytify(DY,-30);
  return myBytes;
}

int * crtU() {
  int * myBytes = (int *)malloc(12 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,25);
  myBytes[2] = bytify(DX,5);
  myBytes[3] = bytify(DY,5);
  myBytes[4] = bytify(DX,10);
  myBytes[5] = bytify(DY,0);
  myBytes[6] = bytify(DX,5);
  myBytes[7] = bytify(DY,-5);
  myBytes[8] = bytify(DY,-25);
  //Return to square one
  myBytes[9] = penify();
  myBytes[10] = bytify(DX,-20);
  myBytes[11] = bytify(DY,0);
  return myBytes;
}

int * crtV() {
  int * myBytes = (int *)malloc(8 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DX,10);
  myBytes[2] = bytify(DY,30);
  myBytes[3] = bytify(DX,10);
  myBytes[4] = bytify(DY,-30);
  myBytes[5] = penify();
  myBytes[6] = bytify(DX,-20);
  myBytes[7] = bytify(DY,0);
  return myBytes;
}

int * crtW() {
  int * myBytes = (int *)malloc(10 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DY,30);
  myBytes[2] = bytify(DX,10);
  myBytes[3] = bytify(DY,-30);
  myBytes[4] = bytify(DX,10);
  myBytes[5] = bytify(DY,30);
  myBytes[6] = bytify(DY,-30);
  //Return to square one
  myBytes[7] = penify();
  myBytes[8] = bytify(DX,-20);
  myBytes[9] = bytify(DY,0);
  return myBytes;
}

int * crtX() {
  int * myBytes = (int *)malloc(10 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DX,20);
  myBytes[2] = bytify(DY,30);
  myBytes[3] = penify();
  myBytes[4] = bytify(DY,-30);
  myBytes[5] = penify();
  myBytes[6] = bytify(DX,-20);
  myBytes[7] = bytify(DY,30);
  //Return to square one
  myBytes[8] = penify();
  myBytes[9] = bytify(DY,-30);
  return myBytes;
}

int * crtY() {
  int * myBytes = (int *)malloc(12 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DX,10);
  myBytes[2] = bytify(DY,15);
  myBytes[3] = bytify(DY,15);
  myBytes[4] = penify();
  myBytes[5] = bytify(DY,-15);
  myBytes[6] = penify();
  myBytes[7] = bytify(DX,10);
  myBytes[8] = bytify(DY,-15);
  //Return to square one
  myBytes[9] = penify();
  myBytes[10] = bytify(DX,-20);
  myBytes[11] = bytify(DY,0);
  return myBytes;
}

int * crtZ() {
  int * myBytes = (int *)malloc(10 * sizeof(int));
  myBytes[0] = penify();
  myBytes[1] = bytify(DX,20);
  myBytes[2] = bytify(DY,0);
  myBytes[3] = bytify(DX,-20);
  myBytes[4] = bytify(DY,30);
  myBytes[5] = bytify(DX,20);
  myBytes[6] = bytify(DY,0);
  //Return to square one
  myBytes[7] = penify();
  myBytes[8] = bytify(DX,-20);
  myBytes[9] = bytify(DY,-30);
  return myBytes;
}

int * crtSPC() {
  int * myBytes = (int *)malloc(2 * sizeof(int));
  myBytes[0] = bytify(DX,15);
  myBytes[1] = bytify(DY,0);
  return myBytes;
}

int * crtROWDOWN() {
  //Creates special color code for row down
  //rgba(102, 255, 204, 255)
  int * myBytes = (int *)malloc(5 * sizeof(int));
  myBytes[0] = 0xF6;
  //int specialColor = 0x66FFCCFF;
  myBytes[1] = 102;
  myBytes[2] = 255;
  myBytes[3] = 204;
  myBytes[4] = 255;
  return myBytes;
}

int * crt(char c) {
  switch (tolower(c)) {
    case '#':
      return crtROWDOWN();
      break;
    case ' ':
      return crtSPC();
      break;
    case 'a':
      return crtA();
      break;
    case 'b':
      return crtB();
      break;
    case 'c':
      return crtC();
      break;
    case 'd':
      return crtD();
      break;
    case 'e':
      return crtE();
      break;
    case 'f':
      return crtF();
      break;
    case 'g':
      return crtG();
      break;
    case 'h':
      return crtH();
      break;
    case 'i':
      return crtI();
      break;
    case 'j':
      return crtJ();
      break;
    case 'k':
      return crtK();
      break;
    case 'l':
      return crtL();
      break;
    case 'm':
      return crtM();
      break;
    case 'n':
      return crtN();
      break;
    case 'o':
      return crtO();
      break;
    case 'p':
      return crtP();
      break;
    case 'q':
      return crtQ();
      break;
    case 'r':
      return crtR();
      break;
    case 's':
      return crtS();
      break;
    case 't':
      return crtT();
      break;
    case 'u':
      return crtU();
      break;
    case 'v':
      return crtV();
      break;
    case 'w':
      return crtW();
      break;
    case 'x':
      return crtX();
      break;
    case 'y':
      return crtY();
      break;
    case 'z':
      return crtZ();
      break;
    default:
      return 0;
  }
}

int * crtAlphabet() {
  int size = 0;
  for (int i = 0; i < strlen(alphabet); i++) {
    size += lengths[i];
    size++;
  }

  int * myBytes = (int *)malloc(size * sizeof(int));
  int index = 0;
  for (int j = 0; j < strlen(alphabet); j++) {
    int * letter = crt(alphabet[j]);
    for (int x = 0; x < lengths[j]; x++) {
      myBytes[index] = letter[x];
      index++;
    }
    myBytes[index] = penify();
    index++;
  }

  return myBytes;
}

void printIntArray(int *array, int length) {
  for (int i = 0; i < length; i++) {
    printf("%d ", (*array));
    array++;
  }
  printf("\n");
}
