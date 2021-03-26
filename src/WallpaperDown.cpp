#include "WallpaperDown.h"
#include <QFileInfo>
#include <QSettings>
#ifdef _WIN32
#include <windows.h>
#endif
WallpaperDown::WallpaperDown(QObject* parent):QObject(parent)
{
    
}

WallpaperDown::~WallpaperDown()
{
    
}

void WallpaperDown::stop()
{
    mReply->disconnect();
    mReply->close();
    mReply->deleteLater();
    mReply = nullptr;
}

void WallpaperDown::onFinish()
{
    mFile.close();
	setWallpaper();
	emit sigDownFinish();
}

#ifdef _WIN32
void WallpaperDown::setWallpaper()
{
	QFileInfo fileInfo(mFile);

	QSettings wallPaper("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
	QString path(fileInfo.absoluteFilePath());
	wallPaper.setValue("Wallpaper", path);
	qDebug() << "change wallpaper to " << fileInfo.absoluteFilePath();
	QByteArray byte = path.toLocal8Bit();
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, byte.data(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

}
#endif

void WallpaperDown::download(QString imgurl, QString fileName)
{	
	QNetworkRequest request(imgurl);
    qDebug() << "imgurl:" << imgurl;
	mReply = nam.get(request);
	connect(mReply, &QNetworkReply::finished, this, &WallpaperDown::onFinish);
	connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onDownError(QNetworkReply::NetworkError)));
	connect(mReply, &QNetworkReply::downloadProgress, this, &WallpaperDown::sigDownProgress);
	connect(mReply, &QNetworkReply::downloadProgress, this, &WallpaperDown::onDownloadProgress);
    mFilename = fileName;
    mFile.setFileName(mFilename);
    mFile.open(QFile::WriteOnly);
}

void WallpaperDown::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    mFile.write(mReply->readAll());
}

void WallpaperDown::onDownError(QNetworkReply::NetworkError err)
{
	qDebug() << "Download error:" << err;
	sigDownError("Download error code:"  + QString::number(err));
}