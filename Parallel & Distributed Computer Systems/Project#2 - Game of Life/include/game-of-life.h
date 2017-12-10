/* #ifndef UTILS_H_   /\* Include guard *\/ */
/* #define UTILS_H_ */

#define Board(x,y) board[(x)*N2 + (y)]
#define NewBoard(x,y) newboard[(x)*N2 + (y)]

/* set everything to zero */

void initialize_board (int *board, int N1, int N2);

/* add to a width index, wrapping around like a cylinder */

int xadd (int i, int a, int N1);

/* add to a height index, wrapping around */

int yadd (int i, int a, int N);

/* return the number of on cells adjacent to the i,j cell */

int adjacent_to (int *board, int i, int j, int N1, int N2);

/* play the game through one generation */

void play (int *board, int *newboard, int N1, int N2, int rank, int numtasks);

/* print the life board */

void print (int *board, int N1, int N2);

/* generate random table */

void generate_table (int *board, int N1, int N2, double threshold, int numtasks);

/* display the table with delay and clear console */

void display_table(int *board, int N1, int N2);

/* #endif // FOO_H_ */
