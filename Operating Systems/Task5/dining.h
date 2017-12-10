#ifndef DINING_H_   /* Include guard */
#define DINING_H_

void grab_forks( int left_fork_id );

void put_away_forks( int left_fork_id );

int philosopher( int num );

void think( int num );

void eat( int num );

#endif // DINING_H_