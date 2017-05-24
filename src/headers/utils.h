#ifndef HEADER_UTILS
#define HEADER_UTILS
void init_seed( void );
int get_rand( int );
char *get_char_array( int );
void give_char_array( char * );
struct object *get_object( void );
void give_object( struct object * );
struct entity *get_entity( void );
void give_entity( struct entity * );
int absolute( int );
int proc_unit( int, int );
#endif
