#pragma once

#include "def.h"
#include "Image.h"
#include "shape.h"
#include "pen.h"
#include "units.h"
#include "unit_menu.h"
#include <functional>
#include <string>


enum tools
{
  TOOL_PEN,
  TOOL_BRUSH,
  TOOL_ERASER,
  TOOL_RECT,
  TOOL_LINE,
  TOOL_CIRCLE
};

class paint_system
{
  int IsInit;
  tools ActiveTool;
  COLOR CurrentColor;
  INT PenSize;

  pen *Pens[3];
  shape *Shapes[3];

protected:
  std::function<VOID(VOID)> OnColors[24];
  std::function<VOID(VOID)> OnTools[6];
  std::function<VOID(VOID)> OnSize[3];

  static VOID OnNew(VOID);
  static VOID OnOpen(VOID);
  static VOID OnSave(VOID);

  static image Image;
  image ImageViewed;
  INT Zoom;
  BOOL NeedCursor;
public:
   paint_system();
   ~paint_system()
   {
      for (int i = 0; i < 3; i++) {
         delete Pens[i];
         delete Shapes[i];
      }
   }

   VOID OnClick(int x, int y, int IsClick);
   VOID OnCursorMove(int x, int y);
};

class UNIT_PAINT : public UNIT_MENU, public paint_system {
public:
   UNIT_PAINT(ANIM * anim) : UNIT_MENU(anim, "a.txt")
   {
      std::string str = "c";

      SetCallbackByName("pen", OnTools[TOOL_PEN]);
      SetCallbackByName("brush", OnTools[TOOL_BRUSH]);
      SetCallbackByName("eraser", OnTools[TOOL_ERASER]);
      SetCallbackByName("rect", OnTools[TOOL_RECT]);
      SetCallbackByName("line", OnTools[TOOL_LINE]);
      SetCallbackByName("circle", OnTools[TOOL_CIRCLE]);

      SetCallbackByName("new", OnNew);
      SetCallbackByName("open", OnOpen);
      SetCallbackByName("save", OnSave);

      for (int i = 0; i < 6; i++)
         for (int j = 0; j < 4; j++) {
            str = "c" + std::to_string(i) + std::to_string(j);
            SetCallbackByName(str, OnColors[i * 4 + j]);
         }

      SetCallbackByName("small", OnSize[0]);
      SetCallbackByName("medium", OnSize[1]);
      SetCallbackByName("big", OnSize[2]);


   }

   virtual VOID Draw(VOID);
   virtual VOID Response(VOID);
};

