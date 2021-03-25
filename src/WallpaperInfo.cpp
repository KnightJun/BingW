#include "WallpaperInfo.h"
#include <QDebug>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
void WallpaperInfo::getInfo()
{
    const QString infoUrl = "HPImageArchive.aspx?format=js&idx=0&n=10&FORM=BEHPTB&uhd=1&uhdwidth=1920&uhdheight=1080";
    QNetworkRequest request(mBaseUrl + infoUrl);
    mReply = nam.get(request);
    connect(mReply, &QNetworkReply::finished, this, &WallpaperInfo::onGetFinish);
    connect(mReply, 
        static_cast<void (QNetworkReply:: *)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
        this, &WallpaperInfo::onGetError);
}

WallpaperInfo::WallpaperInfo(QObject* parent):QObject(parent)
{

}

WallpaperInfo::~WallpaperInfo()
{

}

int WallpaperInfo::getImageCount()
{
    return mImageCount;
}

QString WallpaperInfo::getCopyright()
{
    return mCurrentJsonObj["copyright"].toString();
}

QString WallpaperInfo::getCopyrightLink()
{
    return mCurrentJsonObj["copyrightlink"].toString();
}

QString WallpaperInfo::getGoogleEarthLink()
{
    return "https://earth.google.com/web/search/" + getKeyword();
}

QString WallpaperInfo::getKeyword()
{
    QRegExp rx("q=([%\\w\\d]+)&?");
    int pos = rx.indexIn(getCopyrightLink());
    if(pos < 0){
        emit sigGetError(tr("Get keyword failed!"));
        return "";
    }
    return rx.cap(1);
}

QString WallpaperInfo::getImageUrl(int width, int height)
{
    QString imgUrl = mCurrentJsonObj["url"].toString();
    QString replaceStr = QString("&w=%1&h=%2").arg(width).arg(height);
    return mBaseUrl + imgUrl.replace(QRegExp("&w=\\d+&h=\\d+"), replaceStr);
}

void WallpaperInfo::onGetFinish()
{
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(mReply->readAll(), &jsonErr);
    if(jsonDoc.isEmpty()){
        emit sigGetError(jsonErr.errorString());
        return;
    }
    mInfoJson = jsonDoc.object();
    mImageCount = mInfoJson["images"].toArray().size();
    mCurrentJsonObj = mInfoJson["images"].toArray()[mCurrentInx].toObject();
    qDebug() << getImageUrl(250,320);
    qDebug() << getGoogleEarthLink();
}

void WallpaperInfo::onGetError(QNetworkReply::NetworkError err)
{
    qDebug() << err;
}