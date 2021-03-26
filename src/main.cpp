#include <QApplication>
#include "Wallpaper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Wallpaper wallpaper;
    wallpaper.connect(&wallpaper, &QObject::destroyed, &a, &QApplication::quit);
    return a.exec();
}
