//
// Created by cww on 25-2-19.
//
#pragma once
#include <QString>
#include <QFile>
#include <QDebug>


namespace SvgRes {
    inline const auto TrayIconSVG = QStringLiteral(":/ui/../data/images/icon.svg");
    inline const auto LogoSVG = QStringLiteral(":/ui/../data/images/logo.svg");
    inline const auto PlayIconSVG = QStringLiteral(":/ui/../data/images/play.svg");
    inline const auto PauseIconSVG = QStringLiteral(":/ui/../data/images/pause.svg");
    inline const auto PreIconSVG = QStringLiteral(":/ui/../data/images/pre.svg");
    inline const auto NextIconSVG = QStringLiteral(":/ui/../data/images/next.svg");
    inline const auto VolumeSVG = QStringLiteral(":/ui/../data/images/high-volume.svg");
    inline const auto UpSVG = QStringLiteral(":/ui/../data/images/up.svg");
    inline const auto DownSVG = QStringLiteral(":/ui/../data/images/down.svg");
    inline const auto AddSVG = QStringLiteral(":/ui/../data/images/add.svg");
    inline const auto AdddSVG = QStringLiteral(":/ui/../data/images/addd.svg");
    inline const auto NextPageSVG = QStringLiteral(":/ui/../data/images/nextpage.svg");
    inline const auto BackPageSVG = QStringLiteral(":/ui/../data/images/backpage.svg");
    inline const auto RemoveSVG = QStringLiteral(":/ui/../data/images/remove.svg");
    inline const auto LocalSVG = QStringLiteral(":/ui/../data/images/local.svg");
    inline const auto MusicListSVG = QStringLiteral(":/ui/../data/images/list.svg");
    inline const auto SettingSVG = QStringLiteral(":/ui/../data/images/setting.svg");
    inline const auto VolumeBtnSVG = QStringLiteral(":/ui/../data/images/volumeBtn.svg");
    inline const auto VolumeMuteSVG = QStringLiteral(":/ui/../data/images/volumeMute.svg");
    inline const auto ViewPlaySVG = QStringLiteral(":/ui/../data/images/btnplay.svg");
    inline const auto ViewPauseSVG = QStringLiteral(":/ui/../data/images/btnpause.svg");
    inline const auto ShuffleModeSVG = QStringLiteral(":/ui/../data/images/shuffleList.svg");
    inline const auto LoopAllModeSVG = QStringLiteral(":/ui/../data/images/loopList.svg");
    inline const auto LoopOneModeSVG = QStringLiteral(":/ui/../data/images/loopOne.svg");
    inline const auto SequentialModeSVG = QStringLiteral(":/ui/../data/images/sequentialList.svg");
}


namespace User {
    inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
}


namespace QssRes {
    inline const auto BUTTON_ADD_QSS = QStringLiteral(":/qss/MusicListButton/ButtonAddList.qss");
    inline const auto BUTTON_EXPAND_QSS = QStringLiteral(":/qss/MusicListButton/ButtonExpand.qss");
    inline const auto BUTTON_LIST_QSS = QStringLiteral(":/qss/MusicListButton/ButtonNormal.qss");
    inline const auto BUTTON_VOLUME_QSS = QStringLiteral(":/qss/MusicListButton/ButtonVolume.qss");
    inline const auto BUTTON_SQUARE_QSS = QStringLiteral(":/qss/MusicListButton/ButtonSquare.qss");
    inline const auto LIST_VIEW_QSS = QStringLiteral(":/qss/ListView/PlayListView.qss");
    inline const auto VOLUME_SLIDER_QSS = QStringLiteral(":/qss/Slider/QSlider.qss");
    inline const auto VOLUME_MENU_QSS = QStringLiteral(":/qss/QMenu/VolumeMenu.qss");
    inline const auto PLAY_MODE_QSS = QStringLiteral(":/qss/MusicListButton/ButtonPlayMode.qss");
}


namespace Tray::Ui::Panel {
    inline const auto PLAY_ALL_KEY = QStringLiteral("Play All");
    inline const auto EXPAND_BTN_TEXT = QStringLiteral("User's");
    constexpr int UNINITIALIZED_VALUE = -1;
}


namespace Tools {
    inline QString readQSS(const QString &qssPath) {
        if (QFile file(qssPath); file.open(QFile::ReadOnly)) {
            QString qss = QString::fromUtf8(file.readAll()).trimmed();
            file.close();
            return qss;
        }
        qWarning() << "Failed to load QSS file:" << qssPath;
        return {};
    }
}
