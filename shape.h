#pragma once

#include "def.h"
#include "Image.h"

class shape {
protected:
   VEC2i p1, p2;
public:
   shape() : p1(0, 0), p2(10, 10) { }

   VOID SetP1(VEC2i p)
   {
      p1 = p;
   }
   VOID SetP2(VEC2i p)
   {
      p2 = p;
   }
   virtual VOID Apply(image &im, COLOR c) = 0;
};

class line : public shape {
   virtual VOID Apply(image &im, COLOR c)
   {
      INT signY = (p2.y - p1.y > 0) ? 1 : -1, signX = (p2.x - p1.x > 0) ? 1 : -1;
      FLT d = (fabs(p1.y - p2.y) + 1) / (fabs(p1.x - p2.x) + 1), err = 0;
      if (d < 1) {
         INT y = p1.y;
         for (INT x = p1.x; x != p2.x; x += signX) {
            im.PutPuxel(x, y, c);
            err += d;
            if (err > 1) {
               y += signY;
               err--;
            }
         }
      }
      else {
         d = 1.0 / d;
         INT x = p1.x;
         for (INT y = p1.y; y != p2.y; y += signY) {
            im.PutPuxel(x, y, c);
            err += d;
            if (err > 1) {
               x += signX;
               err--;
            }
         }
      }
   }
};

class rect : public shape {
   virtual VOID Apply(image &im, COLOR c)
   {
      INT signY = (p2.y - p1.y > 0) ? 1 : -1, signX = (p2.x - p1.x > 0) ? 1 : -1;
      for (INT y = p1.y; y != p2.y; y += signY) {
         im.PutPuxel(p1.x, y, c);
         im.PutPuxel(p2.x, y, c);
      }

      for (INT x = p1.x; x != p2.x; x += signX) {
         im.PutPuxel(x, p1.y, c);
         im.PutPuxel(x, p2.y, c);
      }
   }
};

class circle : public shape {
   virtual VOID Apply(image &im, COLOR c)
   {
      int dx = p2.x - p1.x, dy = p2.y - p1.y;
      int R = sqrt(dx * dx + dy * dy);
      int x = 0;
      int y = R;
      int delta = 1 - 2 * R;
      int error = 0;
      while (y >= 0) {
         im.PutPuxel(p1.x + x, p1.y + y, c);
         im.PutPuxel(p1.x + x, p1.y - y, c);
         im.PutPuxel(p1.x - x, p1.y + y, c);
         im.PutPuxel(p1.x - x, p1.y - y, c);
         error = 2 * (delta + y) - 1;
         if ((delta < 0) && (error <= 0)) {
            delta += 2 * ++x + 1;
            continue;
         }
         if ((delta > 0) && (error > 0)) {
            delta -= 2 * --y + 1;
            continue;
         }
         delta += 2 * (++x - --y);
      }
   }
};
