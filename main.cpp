#include "anim.h"
#include "paint_system.h"

#include <list>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInctance, CHAR *cmdLine, INT showCmd)
{
   ANIM anim;

   anim.AddUnit(new UNIT_PAINT(&anim));
   anim.Run();

   return 0x1e;
}
