/* machine-utils.h */

/* Convenient definitions for manipulating the motors 
*/
#define M1_STEP		0x08
#define M1_HFM		0x04
#define M1_DIR		0x02
#define M1_INHIB	0x01
#define M2_STEP		0x10
#define M2_HFM		0x20
#define M2_DIR		0x40
#define M2_INHIB	0x80

/* Reads the keyboard and returns -1 if no key pressed, or
   0-15 if key 0-9 or A-F was pressed.
*/
extern int readKeyboard();
