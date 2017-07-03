#ifndef HEADER_GUIUTILS
#define HEADER_GUIUTILS
struct symbol *do_apply( struct object * );
void clean_symbol( struct symbol * );
int in_path( int, int, int, int );
int in_fov( int, int);
void put_fov( void );
char *get_directions( void );
#endif
