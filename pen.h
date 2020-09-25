#pragma once

#include "def.h"
#include "Image.h"

class pen {
protected:
   UINT size = 5;
public:
   pen() : size(5) { }
   VOID SetSize(UINT s)
   {
      size = s;
   }
   virtual VOID Apply(image &im, VEC2i center) = 0;
};

class colored_pen : public pen {
   COLOR col;
public:
   colored_pen() : col(0, 0, 0, 255) { }

   VOID SetColor(COLOR c) 
   {
      col = c;
   }
   virtual VOID Apply(image &im, VEC2i center)
   {
      for (int i = 0; i < size; i++)
         for (int j = 0; j < size; j++)
            im.PutPuxel(center.x - size / 2 + j, center.y - size / 2 + i, col);
   }
};

class brush : public pen {
   COLOR col;
public:
   brush() : col(0, 0, 0, 255) { }

   VOID SetColor(COLOR c) 
   {
      col = c;
   }
   virtual VOID Apply(image &im, VEC2i center)
   {
      for (int i = 0; i < size; i++)
         for (int j = 0; j < size; j++) {
            int x = center.x - size / 2 + j, y = center.y - size / 2 + i;
            COLOR c = im.GetPixel(x, y);
            double d = sqrt((size / 2 - i) * (size / 2 - i) + (size / 2 - j) * (size / 2 - j)) / (double)size * 3.0;
            im.PutPuxel(x, y, col * d + c * (1 - d));
         }
   }

};

class eraser : public pen {
public:
   
   virtual VOID Apply(image &im, VEC2i center)
   {
      for (int i = 0; i < size; i++)
         for (int j = 0; j < size; j++)
            im.PutPuxel(center.x - size / 2 + j, center.y - size / 2 + i, COLOR(255, 255, 255, 255));
   }

};
