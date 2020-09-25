#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")


class RENDER {
   HDC hDC;
   HGLRC hGLRC;
   HWND &hWin;
   BOOL isSized;


public:
   INT w, h;            // window size
   RENDER(WIN *window);
   VOID Rect(VEC2i p1, VEC2i p2, VEC2f t1, VEC2f t2);
   VOID StartFrame(VOID);
   VOID EndFrame(VOID);
   VOID Resize(INT w, INT h);
   int GetW(VOID)
   {
      return w;
   }
   int GetH(VOID)
   {
      return h;
   }

};

#endif // __RENDER_H_
