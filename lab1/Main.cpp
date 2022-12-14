#include "MainWindow.h"
#include <locale>

int main(VOID)
{
    SetConsoleCP(1152);
    SetConsoleOutputCP(1152);
    setlocale(LC_ALL, "ru");
    MainWindow mainWin;
    mainWin.Show();
    return 0;
}
