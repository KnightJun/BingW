/* download bing wallpaper
    --by KnightJun
 */
#include <QObject>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

class WallpaperDown : public QObject
{
    Q_OBJECT
public:
    WallpaperDown(QObject* parent = nullptr);
    ~WallpaperDown();
    void download(QString imgurl, QString fileName);
    void stop();
signals:
    void sigDownFinish();
    void sigDownError(QString);
    void sigDownProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager nam;
    QNetworkReply* mReply;
    QString mFilename;
    QFile mFile;
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownError(QNetworkReply::NetworkError);
    void onFinish();
    void setWallpaper();
};