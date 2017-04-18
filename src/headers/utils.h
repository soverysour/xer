#ifndef HEADER_UTILS
#define HEADER_UTILS
void init_seed(void);
int get_rand(int);
char *get_char_array(int);
void give_char_array(char*);
struct object *get_object(void);
void give_object(struct object*);
struct symbol *do_apply(struct object*);
void clean_symbol(struct symbol*);
int absolute(int);
#endif
