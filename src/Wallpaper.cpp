#include "Wallpaper.h"
#include "WallpaperDown.h"
#include "WallpaperInfo.h"
#include <QMenu>
#include <QApplication>
#include <QScreen>
#include <QSettings>
#include <QPainter>
extern QSettings *gSetting;
#define SettingKeyRunOnStartup      "RunOnStartup"

Wallpaper::Wallpaper(/* args */)
{
    mTrayIcon = QIcon(":/icon/tray.ico");
    mSysTray.setIcon(mTrayIcon);
    mSysTray.show();

    if(!gSetting->contains(SettingKeyRunOnStartup)){
        SetRunOnStartup(true);
    }

    mWpInfo = new WallpaperInfo(this);
    connect(mWpInfo, &WallpaperInfo::sigGetFinish, this, &Wallpaper::onWpInfoFinish);
    connect(mWpInfo, &WallpaperInfo::sigGetError, this, &Wallpaper::onError);
    
    mWpDown = new WallpaperDown(this);
    connect(mWpDown, &WallpaperDown::sigDownFinish, this, &Wallpaper::onWpDownFinish);
    connect(mWpDown, &WallpaperDown::sigDownError, this, &Wallpaper::onError);

    mTaryMenu = new QMenu();
    initMenu();
    mSysTray.setContextMenu(mTaryMenu);
    
    stateChange(State::CheckInfo);
}

void Wallpaper::changeTrayIcon(QString addIcon)
{
    if(addIcon.isEmpty()){
        mSysTray.setIcon(mTrayIcon);
        return;
    }
    const float iconSizePrecent = 0.6;
    int iconSize = qMin(this->mSysTray.geometry().width(), this->mSysTray.geometry().height());
    QPixmap newIcon = mTrayIcon.pixmap(iconSize, iconSize);
    QImage addImg(addIcon);
    QPainter p(&newIcon);
    QRect dstRect = QRect(
        iconSize * (1-iconSizePrecent), 
        iconSize * (1-iconSizePrecent), 
        iconSize * iconSizePrecent, 
        iconSize * iconSizePrecent);
    p.drawImage(dstRect, addImg);
    mSysTray.setIcon(newIcon);
}

Wallpaper::~Wallpaper()
{
    delete mTaryMenu;
    if(mQueryTimer > 0){
        killTimer(mQueryTimer);
    }
    qDebug() << "destoryed.";
}

void Wallpaper::setStateInit(bool enable)
{
    if(enable){
        if(mWpInfo->getImageCount() > 0){
            mActs.info->setText(QString("[%1/%2] %3")
                .arg(mWpInfo->getCurrentInx() + 1)
                .arg(mWpInfo->getImageCount())
                .arg(mWpInfo->getKeyword())
                );
            qDebug() << "startTimer:"<<mQueryTimer;
        }else{
            mActs.info->setText(tr("Error"));
        }
        mQueryTimer = startTimer(5*60000, Qt::VeryCoarseTimer); /*5 min*/

        if(mLastError.isEmpty()){
            changeTrayIcon("");
            mSysTray.setToolTip(mWpInfo->getCopyright());
        }else{
            changeTrayIcon(":/icon/error.png");
            mSysTray.setToolTip(mLastError);
        }
        
    }else{
        qDebug() << "killTimer:"<<mQueryTimer;
        killTimer(mQueryTimer);
        mQueryTimer = -1;
        mLastError.clear();
    }
}

void Wallpaper::setStateCheckUpdate(bool enable)
{
    
}

void Wallpaper::setStateCheckInfo(bool enable)
{
    if(enable){
        mWpInfo->getInfo();
        mSysTray.setToolTip("Query...");
        mActs.info->setText(tr("Query..."));
        changeTrayIcon(":/icon/query.png");
    }else{
        mWpInfo->stop();
    }
}

void Wallpaper::setStateDownLoading(bool enable)
{
    if(enable){
        mActs.info->setText(tr("Downloading..."));
        mSysTray.setToolTip("Downloading...");
        changeTrayIcon(":/icon/download.png");
        QScreen *screen=QApplication::primaryScreen();
        QRect screct = screen->availableGeometry();
        mWpDown->download(mWpInfo->getImageUrl(screct.width(), screct.height()), 
                "./" + mWpInfo->getFullstartdate() + ".jpg");
    }else{
        mWpDown->stop();
    }
}

void Wallpaper::onWpInfoFinish(bool isNew)
{
    qInfo() << "Get infomation finish. new:" << isNew;
    if(isNew){
        stateChange(DownLoading);
    }else{
        stateChange(Init);
    }
}

void Wallpaper::onWpDownFinish()
{
    stateChange(Init);
}

void Wallpaper::onError(QString err)
{
    mLastError = err;
    mSysTray.showMessage(tr("Error occurred"), err, QSystemTrayIcon::Critical);
    stateChange(Init);
}

void Wallpaper::initMenu()
{
    mActs.info        = mTaryMenu->addAction(tr("Info"));
    mActs.info->setEnabled(false);

    mActs.prev        = mTaryMenu->addAction(tr("Prev walllpaper"));
    mActs.next        = mTaryMenu->addAction(tr("Next walllpaper"));
    mActs.today       = mTaryMenu->addAction(tr("Today walllpaper"));
    connect(mActs.prev, &QAction::triggered, this, &Wallpaper::onChangePageAct);
    connect(mActs.next, &QAction::triggered, this, &Wallpaper::onChangePageAct);
    connect(mActs.today, &QAction::triggered, this, &Wallpaper::onChangePageAct);

    mActs.bing        = mTaryMenu->addAction(tr("Bing search"));
    connect(mActs.bing, &QAction::triggered, mWpInfo, &WallpaperInfo::openCopyrightLink);
    mActs.googleEarth = mTaryMenu->addAction(tr("Google earth"));
    connect(mActs.googleEarth, &QAction::triggered, mWpInfo, &WallpaperInfo::openGoogleEarthLink);

    // mActs.saveas      = mTaryMenu->addAction(tr("Save as"));
    // mActs.preference  = mTaryMenu->addAction(tr("Preference"));
    mActs.runOnStart  = mTaryMenu->addAction(tr("Run on system stratup"));
    mActs.runOnStart->setCheckable(true);
    mActs.runOnStart->setChecked(gSetting->value(SettingKeyRunOnStartup).toBool());
    connect(mActs.runOnStart, &QAction::triggered, this, &Wallpaper::SetRunOnStartup);
    
    mActs.exit        = mTaryMenu->addAction(tr("Exit"));
    connect(mActs.exit, &QAction::triggered, this, &QObject::deleteLater);
}

void Wallpaper::onChangePageAct()
{
    if(sender() == mActs.prev){
        mWpInfo->setCurrentInx(mWpInfo->getCurrentInx() + 1);
    }else if(sender() == mActs.next){
        mWpInfo->setCurrentInx(mWpInfo->getCurrentInx() - 1);
    }else if(sender() == mActs.today){
        mWpInfo->setCurrentInx(0);
    }else{
        onError("onChangePageAct:Unexpect call.");
        return;
    }
    stateChange(DownLoading);
}

void Wallpaper::timerEvent(QTimerEvent *event)
{
        qDebug() << "timerEvent";
    if(mState == State::Init){
        qDebug() << "timerEvent";
        stateChange(CheckInfo);
    }
}

void Wallpaper::updateMenu()
{
    bool hasImageInfo = mWpInfo->getImageCount() > 0;
    mActs.prev->setVisible(hasImageInfo && mWpInfo->getCurrentInx()  < mWpInfo->getImageCount() - 1);
    mActs.next->setVisible(hasImageInfo && mWpInfo->getCurrentInx() > 0);
    mActs.today->setVisible(hasImageInfo);
    mActs.bing->setVisible(hasImageInfo);
    mActs.googleEarth->setVisible(hasImageInfo);
}

void Wallpaper::stateChange(State sta)
{
    qDebug() << "chenge state:" << mState << "->" << sta;
#define ExitState(staName) if(mState == State::staName)setState##staName(false);
    ExitState(Init);
    ExitState(CheckUpdate);
    ExitState(CheckInfo);
    ExitState(DownLoading);
#undef ExitState

    mState = sta;
    updateMenu();

#define EnterState(staName) if(mState == State::staName){setState##staName(true);return;}
    EnterState(Init);
    EnterState(CheckUpdate);
    EnterState(CheckInfo);
    EnterState(DownLoading);
#undef EnterState
}

void Wallpaper::SetRunOnStartup(bool isstart)
{
    QString application_name = QApplication::applicationName();
    QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(isstart){
        QString application_path = QApplication::applicationFilePath();
        settings->setValue(application_name, application_path.replace("/", "\\"));
        if(!settings->contains(application_name)){
            onError(tr("set run on system stratup failed."));
        }
    }
    else{
        if(settings->contains(application_name)){
            settings->remove(application_name);
        }
    }
    gSetting->setValue(SettingKeyRunOnStartup, isstart);
}