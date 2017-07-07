#ifndef HEADER_GUIUTILS
#define HEADER_GUIUTILS
struct symbol *do_apply( struct object * );
void clean_symbol( struct symbol * );
int in_fov( int, int, int, int );
void put_fov( void );
int calculate_path( int, int, int, int, int );
char *get_directions( void );
#endif
