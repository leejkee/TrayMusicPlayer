//
// Created by cww on 25-2-19.
//
#pragma once
#include <QString>
#include <QFile>

namespace SvgRes {
inline const auto TrayIconSVG = QStringLiteral(":/ui/images/icon.svg");
inline const auto LogoSVG = QStringLiteral(":/images/logo.svg");
inline const auto PlayIconSVG = QStringLiteral(":/images/play.svg");
inline const auto PauseIconSVG = QStringLiteral(":/images/pause.svg");
inline const auto PreIconSVG = QStringLiteral(":/images/pre.svg");
inline const auto NextIconSVG = QStringLiteral(":/images/next.svg");
inline const auto VolumeSVG = QStringLiteral(":/images/high-volume.svg");
inline const auto VolumeMuteSVG = QStringLiteral(":/images/no-volume.svg");
inline const auto UpSVG = QStringLiteral(":/images/up.svg");
inline const auto DownSVG = QStringLiteral(":/images/down.svg");
inline const auto AddSVG = QStringLiteral(":/images/add.svg");
inline const auto AdddSVG = QStringLiteral(":/images/addd.svg");
inline const auto NextPageSVG = QStringLiteral(":/images/nextpage.svg");
inline const auto BackPageSVG = QStringLiteral(":/images/backpage.svg");
inline const auto RemoveSVG = QStringLiteral(":/images/remove.svg");
inline const auto LocalSVG = QStringLiteral(":/images/local.svg");
inline const auto MusicListSVG = QStringLiteral(":/images/list.svg");
inline const auto SettingSVG = QStringLiteral(":/images/setting.svg");
inline const auto ViewPlaySVG = QStringLiteral(":/images/btnplay.svg");
inline const auto ViewPauseSVG = QStringLiteral(":/images/btnpause.svg");
}

namespace User {
    inline const auto PLAY_ALL_KEY = QStringLiteral("Play All");
    inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
    inline const auto EXPAND_BTN_TEXT = QStringLiteral("List");



}

namespace QssRes {
    inline const auto BUTTON_ADD_QSS = QStringLiteral(":/qss/MusicListButton/ButtonAddList.qss");
    inline const auto BUTTON_EXPAND_QSS = QStringLiteral(":/qss/MusicListButton/ButtonExpand.qss");
    inline const auto BUTTON_LIST_QSS = QStringLiteral(":/qss/MusicListButton/ButtonNormal.qss");
    inline const auto BUTTON_SQUARE_QSS = QStringLiteral(":/qss/MusicListButton/ButtonSquare.qss");
    inline const auto LIST_VIEW_QSS = QStringLiteral(":/qss/ListView/PlayListView.qss");
}


namespace ViewConfig {

    inline const auto FONT_MIRC_HEI = QStringLiteral("Microsoft YaHei");

    constexpr int VIEW_BUTTON_SIZE = 20;

    constexpr int VIEW_LOGO_PADDING = 5;

}
namespace Tools {

    inline QString readQSS(const QString &qssPath) {
        if (QFile file(qssPath); file.open(QFile::ReadOnly)) {
            QString qss = QString::fromUtf8(file.readAll()).trimmed();
            file.close();
            return std::move(qss);
        }
        qWarning() << "Failed to load QSS file:" << qssPath;
        return {};
    }

}