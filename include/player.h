#ifndef HEADER_PLAYER
#define HEADER_PLAYER
int update_player( char );
struct object *get_player( void );
void teleport_player( int, int );
void damage_player( int );
#endif
