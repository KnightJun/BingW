#include "Wallpaper.h"
#include "WallpaperDown.h"
#include "WallpaperInfo.h"
#include <QMenu>
#include <QApplication>
#include <QScreen>
Wallpaper::Wallpaper(/* args */)
{
    mSysTray.setIcon(QIcon(":/icon/tray.ico"));
    mSysTray.show();
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
            mQueryTimer = startTimer(5*60000, Qt::VeryCoarseTimer); /*5 min*/
            qDebug() << "startTimer:"<<mQueryTimer;
        }else{
            mActs.info->setText(tr("Error"));
            mQueryTimer = startTimer(3000);  /* when error happen, retry after 3 second */
        }
    }else{
        qDebug() << "killTimer:"<<mQueryTimer;
        killTimer(mQueryTimer);
        mQueryTimer = -1;
    }
}

void Wallpaper::setStateCheckUpdate(bool enable)
{
    
}

void Wallpaper::setStateCheckInfo(bool enable)
{
    if(enable){
        mWpInfo->getInfo();
        mActs.info->setText(tr("Query..."));
    }else{
        mWpInfo->stop();
    }
}

void Wallpaper::setStateDownLoading(bool enable)
{
    if(enable){
        mActs.info->setText(tr("Downloading..."));
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
    mSysTray.showMessage(tr("Error occurred"), err, QSystemTrayIcon::Critical);
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