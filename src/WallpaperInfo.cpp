#include "WallpaperInfo.h"
#include <QDebug>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDesktopServices>

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

int WallpaperInfo::getCurrentInx()
{
    return mCurrentInx;
}

bool WallpaperInfo::setCurrentInx(int inx)
{
    if(inx < 0 || inx >= mImageCount){
        return false;
    }
    mCurrentInx = inx;
    mCurrentJsonObj = mInfoJson["images"].toArray()[mCurrentInx].toObject();
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

QString WallpaperInfo::getFullstartdate()
{
    return mCurrentJsonObj["fullstartdate"].toString();
}

QString WallpaperInfo::getEnddate()
{
    return mCurrentJsonObj["enddate"].toString();
}

QString WallpaperInfo::getKeyword()
{
    return QUrl::fromPercentEncoding(getKeywordUrlEncode().toLocal8Bit());
}

void WallpaperInfo::stop()
{
    mReply->disconnect();
    mReply->close();
    mReply->deleteLater();
    mReply = nullptr;
}

QString WallpaperInfo::getCopyrightLink()
{
    return mCurrentJsonObj["copyrightlink"].toString();
}

QString WallpaperInfo::getGoogleEarthLink()
{
    return "https://earth.google.com/web/search/" + getKeywordUrlEncode();
}

QString WallpaperInfo::getKeywordUrlEncode()
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
    QString lastVer;
    if(!mInfoJson.isEmpty()){
        lastVer = mInfoJson["images"].toArray()[0].toObject()["fullstartdate"].toString();
    }
    QJsonParseError jsonErr;
    QByteArray reply = mReply->readAll();
    QFile QFile("test.json");
    QFile.open(QFile::WriteOnly);
    QFile.write(reply);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply, &jsonErr);
    if(jsonDoc.isEmpty()){
        emit sigGetError(jsonErr.errorString());
        return;
    }
    mInfoJson = jsonDoc.object();
    mImageCount = mInfoJson["images"].toArray().size();
    mCurrentInx = 0;
    mCurrentJsonObj = mInfoJson["images"].toArray()[mCurrentInx].toObject();
    QString nowVer = mInfoJson["images"].toArray()[0].toObject()["fullstartdate"].toString();
    
    emit sigGetFinish(nowVer != lastVer);
}

void WallpaperInfo::onGetError(QNetworkReply::NetworkError err)
{
    qDebug() << err;
    emit sigGetError(QString("get infomation network error, code:%1").arg(err));
}


void WallpaperInfo::openGoogleEarthLink()
{
    QDesktopServices::openUrl(QUrl(getGoogleEarthLink()));
}

void WallpaperInfo::openCopyrightLink()
{
    QDesktopServices::openUrl(QUrl(getCopyrightLink()));
}