/* Get bing wallpaper infomation
    --by KnightJun
 */
#include <QObject>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class WallpaperInfo : public QObject
{
    Q_OBJECT
public:
    WallpaperInfo(QObject* parent = nullptr);
    ~WallpaperInfo();
    void getInfo();
    int getImageCount();
    QString getImageUrl(int width, int height);
    QString getCopyright();
    QString getCopyrightLink();
    QString getKeyword();
    QString getGoogleEarthLink();

signals:
    void sigGetFinish();
    void sigGetError(QString);

private:
    /* data */
    void onGetFinish();
    void onGetError(QNetworkReply::NetworkError err);
    QNetworkAccessManager nam;
    QNetworkReply* mReply;
    QJsonObject mInfoJson;
    QJsonObject mCurrentJsonObj;
    QString mBaseUrl = "https://cn.bing.com/";
    int mCurrentInx = 0;
    int mImageCount = 0;
};
