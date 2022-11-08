#include "MainWindow.h"
#include <locale>

int main(VOID)
{
    SetConsoleCP(866);
    SetConsoleOutputCP(866);
    setlocale(LC_ALL, "ru");
    MainWindow mainWin;
    mainWin.Show();
    return 0;
}
