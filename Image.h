#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

struct color4
{
  BYTE B, G, R, A;
};

class image {
   friend class texture;

   std::vector<COLOR> Pixels;
   UINT W, H;
public:
   image() : W(0), H(0)
   {
   }
   BOOL LoadBMP(std::string FileName, bool invGB);

   VOID SaveBMP(std::string FileName);

   VOID PutPuxel(UINT x, UINT y, COLOR c)
   {
      if (x < W && x >= 0 && y < H && y >= 0)
         Pixels[y * W + x] = c;
   }

   COLOR GetPixel(UINT x, UINT y)
   {
      if (x < W && x >= 0 && y < H && y >= 0)
         return Pixels[y * W + x];
      return COLOR(0, 0, 0, 0);
   }

   VOID Free(VOID)
   {
      Pixels.clear();
   }

   VOID Create(int w, int h)
   {
      W = w;
      H = h;
      Pixels.resize(w * h);
      for (auto &p : Pixels)
         p = COLOR(255, 255, 255, 255);
   }
   int GetW(VOID)
   {
      return W;
   }
   int GetH(VOID)
   {
      return H;
   }
   COLOR * GetPtr(VOID)
   {
      return &Pixels[0];
   }

   ~image()
   {
   };
};


#endif // __IMAGE_H_
