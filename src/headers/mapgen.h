#ifndef HEADER_MAPGEN
#define HEADER_MAPGEN
void load_map(void);
struct object *get_map(void);
struct object get_tile(int, int);
#endif
