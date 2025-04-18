//
// Created by cww on 25-2-19.
//
#pragma once
#include <QString>
#include <QFile>
#include <QDebug>


namespace SvgRes {
    inline const auto TrayIconSVG = QStringLiteral(":/ui/images/icon.svg");
    inline const auto LogoSVG = QStringLiteral(":/ui/images/logo.svg");
    inline const auto PlayIconSVG = QStringLiteral(":/ui/images/play.svg");
    inline const auto PauseIconSVG = QStringLiteral(":/ui/images/pause.svg");
    inline const auto PreIconSVG = QStringLiteral(":/ui/images/pre.svg");
    inline const auto NextIconSVG = QStringLiteral(":/ui/images/next.svg");
    inline const auto VolumeSVG = QStringLiteral(":/ui/images/high-volume.svg");
    inline const auto UpSVG = QStringLiteral(":/ui/images/up.svg");
    inline const auto DownSVG = QStringLiteral(":/ui/images/down.svg");
    inline const auto AddSVG = QStringLiteral(":/ui/images/add.svg");
    inline const auto AdddSVG = QStringLiteral(":/ui/images/addd.svg");
    inline const auto NextPageSVG = QStringLiteral(":/ui/images/nextpage.svg");
    inline const auto BackPageSVG = QStringLiteral(":/ui/images/backpage.svg");
    inline const auto RemoveSVG = QStringLiteral(":/ui/images/remove.svg");
    inline const auto LocalSVG = QStringLiteral(":/ui/images/local.svg");
    inline const auto MusicListSVG = QStringLiteral(":/ui/images/list.svg");
    inline const auto SettingSVG = QStringLiteral(":/ui/images/setting.svg");
    inline const auto VolumeBtnSVG = QStringLiteral(":/ui/images/volumeBtn.svg");
    inline const auto VolumeMuteSVG = QStringLiteral(":/ui/images/volumeMute.svg");
    inline const auto ViewPlaySVG = QStringLiteral(":/ui/images/btnplay.svg");
    inline const auto ViewPauseSVG = QStringLiteral(":/ui/images/btnpause.svg");
    inline const auto ShuffleModeSVG = QStringLiteral(":/ui/images/shuffleList.svg");
    inline const auto LoopAllModeSVG = QStringLiteral(":/ui/images/loopList.svg");
    inline const auto LoopOneModeSVG = QStringLiteral(":/ui/images/loopOne.svg");
    inline const auto SequentialModeSVG = QStringLiteral(":/ui/images/sequentialList.svg");
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
    inline const auto PLAY_MODE_QSS = QStringLiteral(":/qss/MusicListButton/ButtonPlayMode.qss");
}


namespace UI::Panel {
    inline const auto PLAY_ALL_KEY = QStringLiteral("Play All");
    inline const auto EXPAND_BTN_TEXT = QStringLiteral("List");
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
