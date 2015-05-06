/* machine-utils.c
   Contains usefull functions for lab4
*/

#include "delayLib.h"
#include "machine-utils.h"

char keyMap[4][4] = {{1,2,3,10},{4,5,6,11},{7,8,9,12},{0,15,14,13}};

/* Polls the keyboard and returns currently pressed key (value 0-15)
  or -1 if no key is pressed */
int readKeyboard() {
  int row=-1, column=-1, val,i;

  /* read row from keyboard */
  sysOutByte(0x180,0x0F);
  delayUsec(100);
  val=sysInByte(0x180);
  if(!(val & 1)) row=0;
  if(!(val & 2)) row=1;
  if(!(val & 4)) row=2;
  if(!(val & 8)) row=3;

  /* read column from keyboard */
  sysOutByte(0x180,0xF0);
  delayUsec(100);
  val=sysInByte(0x180);
  if(!(val & 16)) column=0;
  if(!(val & 32)) column=1;
  if(!(val & 64)) column=2;
  if(!(val & 128)) column=3;
     
  if(row != -1 && column != -1) return keyMap[row][column];
  else return -1;
}
