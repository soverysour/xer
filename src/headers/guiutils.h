#ifndef HEADER_GUIUTILS
#define HEADER_GUIUTILS
struct symbol *do_apply( struct object * );
void clean_symbol( struct symbol * );
int in_reach(int, int, int, int);
void put_fov( void );
#endif
