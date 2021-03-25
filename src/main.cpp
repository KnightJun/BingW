#include <QCoreApplication>
#include "WallpaperInfo.h"
#include "WallpaperDown.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WallpaperInfo wallInfo;
    WallpaperDown wallDown;
    wallInfo.getInfo();
    QObject::connect(&wallInfo, &WallpaperInfo::sigGetFinish, [&](){
        wallDown.download(wallInfo.getImageUrl(1920, 1080), "test.jpg");
    });
    printf("hello xmake\n");
    return a.exec();
}
