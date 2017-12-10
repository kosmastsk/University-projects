# Data Structures
## DS - Proximity ##
> the original game is available [here](http://goo.gl/c0QWCa)

The whole project is implemented in Java. The GUI was given, we had to create the necessary functions for playing.

### Part A - Random Movement ###
* Function that gets the coordinates of each neighbour
* Function that implements a random movement on the board

### Part B - Heuristic Algorithm ###
* _getEvaluation_ function that evaluates every possible movement on the board, by taking into consideration some metrics that occur if we choose this movement for the next step. The one with the best metrics is our next move on the game.

### Part C - MinMax AB Pruning Algorithm ###
* _evaluate_ function that evaluates a movement on the board, by going in depth two and considering some metrics that occur for every possible move. The one with the best metrics is our next move on the game.