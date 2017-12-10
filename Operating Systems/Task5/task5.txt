
The Dining Philosophers

The Dining Philosophers problem is a classic multi-process synchronization
problem. The problem consists of five philosophers sitting at a table who do
nothing but think and eat. Between each philosopher, there is a single fork
In order to eat, a philosopher must have both forks. A problem can arise if
each philosopher grabs the fork on the right, then waits for the fork on the
left. In this case a deadlock has occurred, and all philosophers will starve.
Also, the philosophers should be fair. Each philosopher should be able to eat
as much as the rest.


Implement in the C language the dining philosophers program. For
synchronization implement and use the two following functions:

void grab_forks( int left_fork_id );

void put_away_forks( int left_fork_id );

where parameters are integers identifying semaphores associated with forks.
grab_forks() and put_away_forks() should use IPC semaphores (man semget, man
semop) to make atomic changes on two semaphores at the same time. Print on the
standard output verbose messages from philosophers.

