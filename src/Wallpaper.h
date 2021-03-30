#include <QObject>
#include <QSystemTrayIcon>
#include "AboutDialog.h"
class WallpaperInfo;
class WallpaperDown;
class QAction;
class Wallpaper : public QObject
{
    Q_OBJECT
private:
    enum State
    {
        Init,
        CheckUpdate,
        CheckInfo,
        DownLoading
    };
    
public:
    Wallpaper(/* args */);
    ~Wallpaper();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void stateChange(State sta);
    void setStateInit(bool enable);
    void setStateCheckUpdate(bool enable);
    void setStateCheckInfo(bool enable);
    void setStateDownLoading(bool enable);

    void onWpInfoFinish(bool isNew);
    void onWpDownFinish();
    void onError(QString err);
    void onKeepWallpaper(bool enable);

    void initMenu();
    void updateMenu();
    void onChangePageAct();
    void SetRunOnStartup(bool isstart);
    void changeTrayIcon(QString addIcon);
    QIcon mTrayIcon;
    QSystemTrayIcon mSysTray;
    QString mLastError;
    QMenu *mTaryMenu = nullptr;
    WallpaperInfo* mWpInfo  = nullptr;
    WallpaperDown* mWpDown  = nullptr;
    int mState = -1;
    int mQueryTimer = -1;
    bool mForcedRefresh = false;

    struct actions
    {
        QAction *info;
        QAction *prev;
        QAction *today;
        QAction *next;
        QAction *bing;
        QAction *googleEarth;
        QAction *keepWallpaper;
        QAction *saveas;
        QAction *preference;
        QAction *about;
        QAction *runOnStart; 
        QAction *exit; 
    } mActs;
};
