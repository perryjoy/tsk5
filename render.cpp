#include "win.h"
#include "render.h"

VOID RENDER::StartFrame(VOID)
{
  glClearColor((FLT)0.0, (FLT)0.0, (FLT)0.0, (FLT)1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID RENDER::EndFrame(VOID)
{
  SwapBuffers(hDC);
  glFinish();
}

VOID RENDER::Resize(INT w, INT h)
{
  glViewport(0, 0, w, h);
}

RENDER::RENDER(WIN * window) : hWin(window->hWnd)
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = { 0 };
  w = window->w;
  h = window->h;
  isSized = FALSE;
  hDC = GetDC(window->hWnd);

  ///* OpenGL initialization: setup pixel format */
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(hDC, &pfd);
  DescribePixelFormat(hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(hDC, i, &pfd);

  ///* OpenGL initialization: setup rendering context */
  hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hGLRC);

  Resize(w, h);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
} 
VOID RENDER::Rect(VEC2i p1, VEC2i p2, VEC2f t1, VEC2f t2)
{
  VEC2f pos1 = VEC2f(p1.x / ( double )w * 2.0 - 1, -p1.y / ( double )h * 2.0 + 1);
  VEC2f pos2 = VEC2f(p2.x / ( double )w * 2.0 - 1, -p2.y / ( double )h * 2.0 + 1);
  
  glBegin(GL_QUADS);
  glTexCoord2f(t1.x, 1 - t1.y);
  glVertex2f(pos1.x, pos1.y);
  glTexCoord2f(t2.x, 1 - t1.y);
  glVertex2f(pos2.x, pos1.y);
  glTexCoord2f(t2.x, 1 - t2.y);
  glVertex2f(pos2.x, pos2.y);
  glTexCoord2f(t1.x, 1 - t2.y);
  glVertex2f(pos1.x, pos2.y);

   glEnd();
}
