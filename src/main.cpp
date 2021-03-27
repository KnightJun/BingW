#include <QApplication>
#include <QSettings>
#include <QMutex>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include "Wallpaper.h"
#include "Version.h"
QSettings *gSetting;

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3").arg(current_date_time).arg(text).arg(msg);
    QFile file("log.txt");
    QFileInfo fileInfo(file);
    if(fileInfo.size() < 4 * 1024 * 1024){
        file.open(QIODevice::WriteOnly | QIODevice::Append);
    }else{
        file.open(QIODevice::WriteOnly);
    }
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    QApplication::setApplicationName(STR_AppName);
    QApplication::setApplicationVersion(STR_Version);
    QApplication::setOrganizationName(STR_Corporation);
    QApplication::setOrganizationDomain(STR_WebUrl);
    QString configName = "config.ini";
    qInfo() << STR_AppName << " Start";
    gSetting = new QSettings(configName, QSettings::IniFormat);
    Wallpaper *wallpaper = new Wallpaper();
    wallpaper->connect(wallpaper, &QObject::destroyed, &a, &QApplication::quit);
    return a.exec();
}
