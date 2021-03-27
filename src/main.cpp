#include <QApplication>
#include <QSettings>
#include "Wallpaper.h"
#include "Version.h"
QSettings *gSetting;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName(STR_AppName);
    QApplication::setApplicationVersion(STR_Version);
    QApplication::setOrganizationName(STR_Corporation);
    QApplication::setOrganizationDomain(STR_WebUrl);
    QString configName = "config.ini";
    gSetting = new QSettings(configName, QSettings::IniFormat);
    Wallpaper wallpaper;
    wallpaper.connect(&wallpaper, &QObject::destroyed, &a, &QApplication::quit);
    return a.exec();
}
