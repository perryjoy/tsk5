#include "paint_system.h"
#include "anim.h"
#include <array>

const int TOOLBAR_HEIGHT = 205;

image paint_system::Image;

const COLOR TRUE_COLORS[][4] = {
   {{0, 0, 0, 255}, {255, 255, 255, 255}, {195, 195, 195, 255}, {127, 127, 127, 255}},
   {{136, 0, 21, 255}, {185, 122, 87, 255}, {255, 174, 201, 255}, {237, 28, 36, 255}},
   {{255, 127, 39, 255}, {255, 201, 14, 255}, {240, 228, 176, 255}, {255, 242, 0, 255}},
   {{34, 177, 76, 255}, {181, 230, 29, 255}, {153, 217, 234, 255}, {0, 162, 232, 255}},
   {{63, 72, 204, 255}, {112, 146, 190, 255}, {163, 73, 164, 255}, {200, 191, 231, 255}},
   {{100, 180, 110, 255}, {64, 64, 64, 255}, {0, 100, 150, 255}, {147, 147, 6, 255}}
};

__inline unsigned long long Coord(int x, int y, int w, int h)
{
   return ((unsigned long long)(h - y) * w + x);
}


#define Init(i) OnColors[i] = [&]() { CurrentColor = TRUE_COLORS[(i) / 4][(i) % 4]; };
#define ColorInit4(j) \
Init(j * 4)   \
Init(j * 4 + 1)   \
Init(j * 4 + 2)   \
Init(j * 4 + 3)  \


paint_system::paint_system()
{
   NeedCursor = false;
   ActiveTool = TOOL_PEN;
   CurrentColor = COLOR(0, 0, 0, 255);
   PenSize = 5;
   Image.Create(1200, 500);
   IsInit = true;
   
   ColorInit4(0);
   ColorInit4(1);
   ColorInit4(2);
   ColorInit4(3);
   ColorInit4(4);
   ColorInit4(5);

   OnTools[0] = [&]() { ActiveTool = ( tools )0; };
   OnTools[1] = [&]() { ActiveTool = ( tools )1; };
   OnTools[2] = [&]() { ActiveTool = ( tools )2; };
   OnTools[3] = [&]() { ActiveTool = ( tools )3; };
   OnTools[4] = [&]() { ActiveTool = ( tools )4; };
   OnTools[5] = [&]() { ActiveTool = ( tools )5; };
   
   OnSize[0] = [&]() { PenSize = (0 + 1) * 5; };
   OnSize[1] = [&]() { PenSize = (1 + 1) * 5; };
   OnSize[2] = [&]() { PenSize = (2 + 1) * 5; };

   Pens[0] = new colored_pen;
   Pens[1] = new brush;
   Pens[2] = new eraser;

   Shapes[0] = new rect;
   Shapes[1] = new line;
   Shapes[2] = new circle;
}

VOID paint_system::OnNew(VOID)
{
   Image.Free();
   Image.Create(1200, 500);
}
VOID paint_system::OnOpen(VOID)
{
   CHAR FileName[300] = "\0";
   OPENFILENAME opfn = { 0 };
   INT e;

   memset(&opfn, 0, sizeof(opfn));
   opfn.lStructSize = sizeof(opfn);
   opfn.hwndOwner = NULL;
   opfn.hInstance = NULL;
   opfn.lpstrFilter = "Bitmap files(*.bmp)\0*.bmp\0";
   opfn.lpstrFile = FileName;
   opfn.nMaxFile = 300;
   opfn.nFilterIndex = 1;
   opfn.lpstrFileTitle = NULL;
   opfn.nMaxFileTitle = 0;
   opfn.lpstrInitialDir = NULL;
   opfn.Flags = OFN_PATHMUSTEXIST | OFN_CREATEPROMPT;
   if (GetOpenFileName(&opfn)) {
      Image.Free();
      if (!Image.LoadBMP(FileName, false))
         MessageBox(NULL, "Can not open file", "ERROR", 0);
   }
   else
      e = CommDlgExtendedError();
}
VOID paint_system::OnSave(VOID)
{
   CHAR FileName[300] = "\0";
   OPENFILENAME opfn = { 0 };
   INT e;
   memset(&opfn, 0, sizeof(opfn));
   opfn.lStructSize = sizeof(opfn);
   opfn.hwndOwner = NULL;
   opfn.hInstance = NULL;
   opfn.lpstrFilter = "Bitmap files(*.bmp)\0*.bmp\0";
   opfn.lpstrFile = FileName;
   opfn.nMaxFile = 300;
   opfn.nFilterIndex = 1;
   opfn.lpstrFileTitle = NULL;
   opfn.nMaxFileTitle = 0;
   opfn.lpstrInitialDir = NULL;
   opfn.Flags = OFN_PATHMUSTEXIST | OFN_CREATEPROMPT;
   if (GetSaveFileName(&opfn))
      Image.SaveBMP(FileName);
}

VOID paint_system::OnClick(int x, int y, int IsClick)
{
   static BOOL f = true;
   y -= TOOLBAR_HEIGHT + 1;
   y = Image.GetH() - y;
   switch (ActiveTool) {
   case TOOL_PEN:
      if (dynamic_cast< colored_pen * >(Pens[ActiveTool]))
         dynamic_cast< colored_pen * >(Pens[ActiveTool])->SetColor(CurrentColor);
   case TOOL_BRUSH:
      if (dynamic_cast< brush * >(Pens[ActiveTool]))
         dynamic_cast< brush * >(Pens[ActiveTool])->SetColor(CurrentColor);
   case TOOL_ERASER:
      Pens[ActiveTool]->SetSize(PenSize);
      Pens[ActiveTool]->Apply(Image, VEC2i(x, y));
      break;
   case TOOL_RECT:
   case TOOL_LINE:
   case TOOL_CIRCLE:
      if (IsClick) {
         if (f)
            Shapes[ActiveTool - 3]->SetP1(VEC2i(x, y));
         else {
            Shapes[ActiveTool - 3]->SetP2(VEC2i(x, y));
            Shapes[ActiveTool - 3]->Apply(Image, CurrentColor);
         }
         f = !f;
      }
      break;
   default:
      break;
   }
}

VOID paint_system::OnCursorMove(int x, int y)
{
}


VOID UNIT_PAINT::Draw(VOID)
{
   ImageViewed = Image;

   if (ImageViewed.GetPtr()) {
     glRasterPos2f(-1, 1 - (( FLT )TOOLBAR_HEIGHT + ImageViewed.GetH()) / ani->GetH() * 2.0);
     glDrawPixels(ImageViewed.GetW(), ImageViewed.GetH(), GL_RGBA, GL_UNSIGNED_BYTE, ImageViewed.GetPtr());
  }

   UNIT_MENU::Draw();
}

VOID UNIT_PAINT::Response(VOID)
{
   if (ani->GetMouseData().buttonsClick[LEFT] || ani->GetMouseData().buttons[LEFT])
      OnClick(ani->GetMouseData().pos.x, ani->GetMouseData().pos.y, ani->GetMouseData().buttonsClick[LEFT]);

   UNIT_MENU::Response();
}

