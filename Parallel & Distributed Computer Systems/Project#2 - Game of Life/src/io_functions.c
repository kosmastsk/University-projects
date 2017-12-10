#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../include/game-of-life.h"

/* print the life board W*/

void print (int *board, int N1, int N2) {
  int i, j;

  /* for each row */
  for (i=0; i<N1; i++) {

    /* print each column position... */
    for (j=0; j<N2; j++) {
      printf ("%c", Board(i,j) ? 'x' : ' ');
    }

    /* followed by a carriage return */
    printf ("\n");
  }
}



/* display the table with delay and clear console */

void display_table(int *board, int N1, int N2) {
  print (board, N1, N2);
  usleep(100000);  
  /* clear the screen using VT100 escape codes */
  puts (" \033 [H \033 [J ");
}
