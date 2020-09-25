#ifndef __MENU_SYSTEM_H_
#define __MENU_SYSTEM_H_

#include "def.h"
#include "mth.h"
#include "units.h"
#include "texture.h"
#include <stdio.h>
#include <vector>
#include <functional>
#pragma warning(disable:4996)

VOID default_func(VOID);

struct button {
   VEC2i Pos, Size;
   VEC2f Tex1, Tex2;
   char Name[128];
   std::function<VOID(VOID)> OnClick = default_func;
};

struct menu_window
{
  std::vector<button> Buttons;
  VEC2i Pos;
  VEC2i Size;
  VEC2f Tex1, Tex2;
};

class menu_system {
private:
   BOOL IsInit = FALSE;
protected:
   std::vector<menu_window> MenuWindows;
   UINT SelectedButton;
   UINT SelectedWindow;

public:
   VOID SetCallbackByName(std::string Str, std::function<VOID(VOID)> f)
   {
      for (auto &w : MenuWindows)
         for (auto &b : w.Buttons)
            if (Str == std::string(b.Name)) {
               b.OnClick = f;
               return;
            }
      throw "ERROR";
   }

   menu_system(std::string Filename);
};

class UNIT_MENU : public UNIT, public menu_system {
   texture Select, Common;
public:
   UNIT_MENU(ANIM * anim, std::string filename) : UNIT(anim), menu_system(filename)
   {
      image Im;
      Im.LoadBMP("BIN/buttons.bmp", true);
      Common = texture((char *)"common", 4, Im);
      Im.Free();
      Im.LoadBMP("BIN/selected_buttons.bmp", true);
      Select = texture((char *)"selected", 4, Im);
      Im.Free();
   }

   virtual VOID Draw(VOID);

   virtual VOID Response(VOID);
};

#endif // __MENU_SYSTEM_H_

