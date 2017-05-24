#ifndef HEADER_MONSTERS
#define HEADER_MONSTERS
void new_monsters( void );
struct object *get_monsters( void );
struct object *get_monster( int, int );
int damage_monster( struct object *, int );
void update_monsters(void);
#endif
