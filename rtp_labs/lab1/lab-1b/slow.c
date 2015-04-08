#include <unistd.h>
#include <sys/io.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
  int i,j,cycles;

  cycles=0;
  while(1) {
    for(i=0,j=0;i<150000;i++) { j += i; }
    if((++cycles) % 100 == 0) printf("Tick\n");
    usleep(10000);
  }
}
