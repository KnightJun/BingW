#include "WallpaperDown.h"


WallpaperDown::WallpaperDown(QObject* parent):QObject(parent)
{
    
}

WallpaperDown::~WallpaperDown()
{
    
}
void WallpaperDown::onFinish()
{
    mFile.close();
}

void WallpaperDown::download(QString imgurl, QString fileName)
{	
	QNetworkRequest request(imgurl);
    qDebug() << "imgurl:" << imgurl;
	mReply = nam.get(request);
	connect(mReply, &QNetworkReply::finished, this, &WallpaperDown::sigDownFinish);
	connect(mReply, &QNetworkReply::finished, this, &WallpaperDown::onFinish);
	connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(sigDownError()));
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
