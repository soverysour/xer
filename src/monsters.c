#include "headers/symbol.h"
#include "headers/mapgen.h"
#include "headers/utils.h"

#define MONSTER_COUNT 8

struct object *head_monster;
struct object *tail_monster;

char effects[NR_BUFFS] = {};

void kill_monsters(void){
  if( !head_monster )
    return;

  struct object *current = head_monster, *previous = head_monster;

  while ( current->next ){
    give_entity(current->entity);
    current = current->next;
    give_object(previous);
    previous = current;
  }

  give_entity(current->entity);
  give_object(current);
}

void new_monsters(void){
  kill_monsters();

  head_monster = get_object();

  struct object *current_monster = head_monster;
  current_monster->entity = get_entity();

  struct room *x = get_rooms();
  for ( int i = 0; i < MONSTER_COUNT; i++ ){
    current_monster->x = x[i].x;
    current_monster->y = x[i].y;
    current_monster->id = ID_MONSTER;
    current_monster->visibility = V_SEEN;
    current_monster->effects = effects;
    
    current_monster->entity = get_entity();
    current_monster->entity->hp = 5;

    current_monster->next = get_object();
    current_monster = current_monster->next; 
  }

  current_monster->x = x[MONSTER_COUNT].x;
  current_monster->y = x[MONSTER_COUNT].y;
  current_monster->id = ID_MONSTER;
  current_monster->visibility = V_SEEN;
  current_monster->effects = effects;
    
  current_monster->entity = get_entity();
  current_monster->entity->hp = 5;

  struct object *a = head_monster, *b;
}

struct object *get_monsters(void){
  return head_monster;
}
