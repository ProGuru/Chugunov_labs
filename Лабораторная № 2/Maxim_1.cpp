#include <conio.h>
#include <time.h>
#include <string>
//#include <dos.h>
using namespace std;

string colortotext(int color) {
        string result;
        switch (color) {
                case 0:
                        result = "чёрный";
                        break;
                case 1:
                        result = "синий";
                        break;
                case 2:
                        result = "зелёный";
                        break;
                case 3:
                        result = "сине-зелёный";
                        break;
                case 4:
                        result = "красный";
                        break;
                case 5:
                        result = "красно-синий";
                        break;
                case 6:
                        result = "коричневый";
                        break;
                case 7:
                        result = "светло-серый";
                        break;
                case 8:
                        result = "тёмно-серый";
                        break;
                case 9:
                        result = "ярко-синий";
                        break;
                case 10:
                        result = "ярко-зелёный";
                        break;
                case 11:
                        result = "яркий сине-зелёный";
                        break;
                case 12:
                        result = "ярко-красный";
                        break;
                case 13:
                        result = "яркий красно-синий";
                        break;
                case 14:
                        result = "жёлтый";
                        break;
                case 15:
                        result = "белый";
                        break;
        }
        return result;
}

extern "C" void scroll (int direction, char l_row, char l_col, char r_row, char r_col, char attr) { 
        union REGS r;
        if (direction) {
                r.h.al = 2;
                r.h.ah = direction;
        } else {
                r.h.al=0;
                r.h.ah=6;
        }
        r.h.ch = l_row;
        r.h.cl = l_col;
        r.h.dh = r_row;
        r.h.dl = r_col;
        r.h.bh=attr; 
        int86(0x10,&r,&r);
}

void printcolor(int textcolor, int backgroundcolor) {
        string color = "Текст: " + colortotext(textcolor) + ", фон: " + colortotext(backgroundcolor);
        cprintf(color.c_str());
}

int main(int argc, char *argv[])
{
        setlocale(0, "");
        SetConsoleOutputCP(1251);
        SetConsoleCP(1251);
        const int x1 = 10, y1 = 15, x2 = 70, y2 = 20, t = 700, s = 2;

        clrscr();
        window(x1, y1, x2, y2);

        for (int i = 0; i < 16; ++i) {
                textcolor(i);
                for (int j = 0; j < 8; ++j) {
                        gotoxy(1, 1);
                        textbackground(j);
                        printcolor(i, j);
			scroll(UP, 0, 0, 60, 5, 0x07)
                        delay(t);
                }
        }

        getch();
        return 0;
}