#ifndef HEADER_MAPGEN
#define HEADER_MAPGEN
int next_level( void );
struct object *get_tile( int, int );
struct room *get_rooms( void );
char get_level(void);
#endif
