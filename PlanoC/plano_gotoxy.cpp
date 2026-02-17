#include <iostream>
#include <windows.h>
using namespace std;

void gotoxy(int x, int y) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

void punto(int x, int y, int origenX, int origenY) {
    gotoxy(origenX + x, origenY - y);
    cout << "X";
}

int main() {

    int maxX = 20;
    int maxY = 10;

    int origenX = 5;
    int origenY = 15;

    // Dibujar eje X
    for(int x = 0; x <= maxX; x++) {
        gotoxy(origenX + x, origenY);
        cout << "-";
    }

    // Dibujar eje Y
    for(int y = 0; y <= maxY; y++) {
        gotoxy(origenX, origenY - y);
        cout << "|";
    }

    gotoxy(origenX, origenY);
    cout << "+";

    // Dibujar puntos
    punto(5, 3, origenX, origenY);
    punto(10, 7, origenX, origenY);
    punto(15, 2, origenX, origenY);

    gotoxy(0, origenY + 3);
    return 0;
}
