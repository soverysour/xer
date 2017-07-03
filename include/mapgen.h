#ifndef HEADER_MAPGEN
#define HEADER_MAPGEN
int next_level( void );
struct object *get_tile( int, int );
struct room *get_rooms( void );
struct room *find_inside( int, int );
char get_level( void );
#endif
