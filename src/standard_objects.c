#include "symbol.h"

struct object Oquit =
{
  .id = ID_QUIT
};

struct object Owin =
{
  .id = ID_WIN
};

struct object Odead =
{
  .id = ID_DEAD
};

char Ewallfloordoor[NR_BUFFS] = {};
char Ehud[NR_BUFFS] = { 1 };

struct object Owall =
{
  .id = ID_WALL,
  .effects = Ewallfloordoor
};

struct object Ofloor =
{
  .id = ID_FLOOR,
  .effects = Ewallfloordoor
};

struct object Odoor =
{
  .id = ID_DOOR,
  .effects = Ewallfloordoor
};

struct object Owait =
{
  .id = ID_HUD,
  .y = POS_WAIT,
  .effects = Ehud
};

struct object Olevel =
{
  .id = ID_HUD,
  .y = POS_LEVEL,
  .effects = Ehud
};

struct object Ohp =
{
  .id = ID_HUD,
  .y = POS_HP,
  .effects = Ehud,
  .next = &Olevel
};
