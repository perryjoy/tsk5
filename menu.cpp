#include "anim.h"
#include "unit_menu.h"

VOID default_func(VOID)
{
}


menu_system::menu_system(std::string Filename)
{
   FILE *F;
   char Buf[128];
   int tmp;

   if (!(F = fopen(Filename.c_str(), "r"))) {
      MessageBox(NULL, "Invalid file name", "Error", NULL);
      throw "Error";
   }

   UINT CurWindow = 0;
   while (!feof(F)) {
      fscanf(F, "%s", Buf);
      if (!strcmp("NumOfWindows", Buf)) {
         fscanf(F, "%i", &tmp);
         MenuWindows.resize(tmp);
      }
      else if (!strcmp("WindowBegin", Buf)) {
         fscanf(F, "%s", Buf);
         if (!strcmp("Position", Buf))
            fscanf(F, "%i %i", &(MenuWindows[CurWindow].Pos.x), &(MenuWindows[CurWindow].Pos.y));

         fscanf(F, "%s", Buf);
         if (!strcmp("Size", Buf))
            fscanf(F, "%i %i", &(MenuWindows[CurWindow].Size.x), &(MenuWindows[CurWindow].Size.y));

         MenuWindows[CurWindow].Tex1 = VEC2f(MenuWindows[CurWindow].Pos.x / 392.0, MenuWindows[CurWindow].Pos.y / 102.0);
         MenuWindows[CurWindow].Tex2 = VEC2f((MenuWindows[CurWindow].Pos.x + MenuWindows[CurWindow].Size.x) / 392.0, (MenuWindows[CurWindow].Pos.y + MenuWindows[CurWindow].Size.y) / 102.0);


         while (strcmp("WindowEnd", Buf) && !feof(F)) {
            fscanf(F, "%s", Buf);
            if (!strcmp("NumOfButtons", Buf)) {
               fscanf(F, "%i", &tmp);
               if (tmp == 0)
                  MessageBox(NULL, "Cant create window without buttons", "Error", NULL);

            }
            else if (!strcmp("ButtonBegin", Buf)) {
               button CurrentButton;
               fscanf(F, "%s", Buf);
               if (!strcmp("Position", Buf))
                  fscanf(F, "%i %i", &CurrentButton.Pos.x, &CurrentButton.Pos.y);

               fscanf(F, "%s", Buf);
               if (!strcmp("Size", Buf))
                  fscanf(F, "%i %i", &CurrentButton.Size.x, &CurrentButton.Size.y);

               CurrentButton.Tex1 = VEC2f(CurrentButton.Pos.x / 392.0, CurrentButton.Pos.y / 102.0);
               CurrentButton.Tex2 = VEC2f((CurrentButton.Pos.x + CurrentButton.Size.x) / 392.0, (CurrentButton.Pos.y + CurrentButton.Size.y) / 102.0);

               fscanf(F, "%s", Buf);
               if (!strcmp("Name", Buf))
                  fscanf(F, "%s", CurrentButton.Name);

               fscanf(F, "%s", Buf);
               MenuWindows[CurWindow].Buttons.push_back(CurrentButton);
            }
         }
         CurWindow++;
      }
   }

   for (auto &win : MenuWindows) {
      win.Pos *= 2;
      win.Size *= 2;
      for (auto &b : win.Buttons) {
         b.Pos *= 2;
         b.Size *= 2;
      }
   }

   SelectedWindow = 0;
   SelectedButton = 0;

   IsInit = TRUE;
}

VOID UNIT_MENU::Draw(VOID)
{
   int q = 0;
   for (auto &win : MenuWindows) {
      ani->Rect(win.Pos, win.Pos + win.Size, win.Tex1, win.Tex2);

      for (int i = 0; i < win.Buttons.size(); i++) {
         if (i == SelectedButton && q == SelectedWindow)
            Select.Apply();
         else
            Common.Apply();
         ani->Rect(win.Buttons[i].Pos,
            win.Buttons[i].Pos + win.Buttons[i].Size,
            win.Buttons[i].Tex1, win.Buttons[i].Tex2);
      }
      q++;
   }
}

VOID UNIT_MENU::Response(VOID)
{
   if (ani->GetKeyboardData().keysClick[VK_RETURN])    //VK_RETURN
      MenuWindows[SelectedWindow].Buttons[SelectedButton].OnClick();
   else if (ani->GetKeyboardData().keysClick[VK_DOWN] && SelectedButton < MenuWindows[SelectedWindow].Buttons.size() - 1)
      SelectedButton++;
   else if (ani->GetKeyboardData().keysClick[VK_UP] && SelectedButton > 0)
      SelectedButton--;
   else if (ani->GetKeyboardData().keysClick[VK_RIGHT] && SelectedWindow < MenuWindows.size() - 1) {
      SelectedWindow++;
      SelectedButton = 0;
   }
   else if (ani->GetKeyboardData().keysClick[VK_LEFT] && SelectedWindow > 0) {
      SelectedWindow--;
      SelectedButton = 0;
   }
}
