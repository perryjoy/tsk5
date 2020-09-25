#include "Image.h"

BOOL image::LoadBMP(std::string FileName, bool invGB)
{
   HBITMAP hBm;

   if ((hBm = ( HBITMAP )LoadImage(NULL, FileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL) {
      INT i;
      DWORD *Bits;
      BITMAP bm;
      BITMAPINFOHEADER bih = { 0 };
      HBITMAP hDib;
      HDC hDC, hMemDC1, hMemDC2;

      hDC = GetDC(NULL);
      hMemDC1 = CreateCompatibleDC(hDC);
      hMemDC2 = CreateCompatibleDC(hDC);
      ReleaseDC(NULL, hDC);

      GetObject(hBm, sizeof(bm), &bm);

      /* Make DIB in memory */
      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biBitCount = 32;
      bih.biWidth = bm.bmWidth;
      bih.biHeight = bm.bmHeight;
      bih.biPlanes = 1;

      hDib = CreateDIBSection(NULL, ( BITMAPINFO * )& bih, DIB_RGB_COLORS, ( VOID * * )& Bits, NULL, 0);

      SelectObject(hMemDC1, hBm);
      SelectObject(hMemDC2, hDib);
      BitBlt(hMemDC2, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCCOPY);

      W = bm.bmWidth;
      H = bm.bmHeight;
      Pixels.resize(H * W);

      /* Correct alpha-channel */
      for (i = 0; i < bm.bmWidth * bm.bmHeight; i++) {
         //*(ULONG*)(Pixels + i) = Bits[i] | 0xFF000000;
         auto tmp = Bits[i] | 0xFF000000;
         color4 c = *( color4 * )(&tmp);
         Pixels[i].a = c.A;
         Pixels[i].g = c.G;
         if (invGB) {
            Pixels[i].b = c.R;
            Pixels[i].r = c.B;
         }
         else {
            Pixels[i].r = c.R;
            Pixels[i].b = c.B;
         }
         if (Pixels[i].r == 250 && Pixels[i].g == 250 && Pixels[i].b == 250)
            Pixels[i].a = 0;
      }
      //FlipVertical();

      DeleteObject(hDib);
      DeleteObject(hBm);
      DeleteDC(hMemDC1);
      DeleteDC(hMemDC2);
      return TRUE;
   }
   return FALSE;
}

VOID image::SaveBMP(std::string FileName)
{
  FILE *F;
  unsigned char *img = NULL;
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  BYTE Palette[1024];
  unsigned char bmppad[3] = { 0,0,0 };
  int filesize = 54 + 3 * W * H;

  if (!(F = fopen(FileName.c_str(), "wb")))
    return;


  memset(Palette, 0, 1024);
  memset(&bfh, 0, sizeof(bfh));

  bfh.bfType = 0x4D42;                           // 'BM'
  bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;    // Палитра занимает 1Kb, но мы его использовать не будем
  bfh.bfSize = bfh.bfOffBits +
    sizeof(BYTE) * 3 * W * H;
  memset(&bih, 0, sizeof(bih));
  bih.biSize = sizeof(bih);                      // Так положено
  bih.biBitCount = 24;                           // 16 бит на пиксель
  bih.biClrUsed = 0;                         // Мы используем 5-5-5
  bih.biCompression = BI_RGB;                    // Без сжатия
  bih.biHeight = H;
  bih.biWidth = W;
  bih.biPlanes = 1;

  fwrite(&bfh, sizeof(bfh), 1, F);
  fwrite(&bih, sizeof(bih), 1, F);
  fwrite(Palette, sizeof(BYTE), 1024, F);

  for (int i = 0; i < H; i++)
    for (int j = 0; j < W; j++)
    {
      fwrite(&Pixels[i * W + j].b, sizeof(BYTE), 1, F);
      fwrite(&Pixels[i * W + j].g, sizeof(BYTE), 1, F);
      fwrite(&Pixels[i * W + j].r, sizeof(BYTE), 1, F);
    }

  fclose(F);
}