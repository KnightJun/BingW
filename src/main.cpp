#include <QCoreApplication>
#include "WallpaperInfo.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WallpaperInfo wallInfo;
    wallInfo.getInfo();
    printf("hello xmake\n");
    return a.exec();
}
