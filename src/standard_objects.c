#include "headers/symbol.h"

struct object Oquit =
{
  .id = ID_QUIT
};

char Ewallfloor[NR_BUFFS] = {};
char Ehud[NR_BUFFS] = { 1 };

struct object Owall =
{
  .id = ID_WALL,
  .effects = Ewallfloor
};

struct object Ofloor =
{
  .id = ID_FLOOR,
  .effects = Ewallfloor
};

struct object Owait =
{
  .id = ID_HUD,
  .effects = Ehud,
  .x = 20,
  .y = POS_WAIT
};

struct object Ohp =
{
  .id = ID_HUD,
  .x = 2,
  .y = POS_HP,
  .effects = Ehud
};
