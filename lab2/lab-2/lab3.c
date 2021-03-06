/* Intensity values in range 0 - 255 for each component of the 16x8 pixels */
extern int pixels[16][8][3];

void drawIt(int offset,int flip) {
  int x,y,r,g,b;
  char *data;
  /* This is a secret image, can you figure out what it is? */
  static char *icon =
    "``````````_(``]!``]<``_(``````````````]@``]!``]!``]!QM)!J+1!````"
    "``_(``]!!!!!``]!``]!``]!QM)!W>F_``]<``]!``]!``]!``]!``]!QM)!J+1!"
    "``]<``]!``]!``]!!!!!``]!QM)!J+1!``_(!!!!!!!!!!!!``]!``]!QM)!W>F_"
    "``````]@``]!``]!``]!QM)!J+1!``````````````_(QM)!R=5\"W>F_````````"
    "";
  
  /* Normally there would be comments here, but that would spoil the surprise */
  for(x=0;x<16;x++) 
    for(y=0;y<8;y++)
      if(y > 5) {
	pixels[x][y][0] = 64;
	pixels[x][y][1] = 64;
	pixels[x][y][2] = 255;
      } else {
	pixels[x][y][0] = 0;
	pixels[x][y][1] = 255;
	pixels[x][y][2] = 0;	   
      }
  
  for(x=0;x<8;x++)
    for(y=0;y<8;y++) {
      data = icon + ((flip?(7-x):x) + (7-y)*8)*4;
      r = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4));
      g = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2));
      b = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33)));
      if(r<200 || g<200 || b<200) {
	pixels[x+offset][y][0] = r;
	pixels[x+offset][y][1] = g;
	pixels[x+offset][y][2] = b;
      }
    }
}

void drawThread() {
  int pos=0,dir=1;
  for(;;) {
    /* Add a delay here so that this function is only run twice per second */
    taskDelay(500);

    pos = pos + dir;
    drawIt(pos,dir==1?1:0);
    if(pos>=7) dir=-1;
    else if(pos<=0) dir=1;
  }
}
