#ifndef HEADER_UTILS
#define HEADER_UTILS
void init_seed(void);
int get_random(int);
char *get_char_array(int);
void give_char_array(char*);
void clean_symbol(struct symbol*);
void clean_object(struct object*);
struct symbol *do_apply(struct object*);
#endif
