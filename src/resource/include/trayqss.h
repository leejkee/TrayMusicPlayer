//
// Created by cww on 25-5-11.
//
#pragma once
#include <QString>
#include <QFile>

namespace Tray::Res
{
inline const auto BUTTON_ADD_QSS = QStringLiteral(R"(
    QPushButton {
        qproperty-iconSize: 12px;
        border: 2px solid transparent;
        border-radius: 8px;
        padding: 3px;
        background-color: transparent;
    }
    QPushButton:hover {
        border: 2px solid transparent;
        border-radius: 8px;
        background-color: lightgray;
    }
)");

inline const auto BUTTON_EXPAND_QSS = QStringLiteral(R"(
    QPushButton {
        max-width: 60px;
        max-height: 30px;
        font-size: 12px;
        font-weight: bold;
        color: grey;
        qproperty-iconSize: 12px;
        border: 2px solid transparent;
        border-radius: 5px;
        padding: 3px;
    }
    QPushButton:hover {
        color: black;
        border: 2px solid transparent;
        background-color: transparent;
    }
)");

inline const auto BUTTON_NORMAL_QSS = QStringLiteral(R"(
    QPushButton {
        font-size: 14px;
        qproperty-iconSize: 18px;
        border: 2px solid transparent;
        border-radius: 5px;
        font-weight: bold;
        padding: 5px;
        text-align: left;
    }
    QPushButton:hover {
        border: 2px solid transparent;
        background-color: lightgray;
    }
)");

inline const auto BUTTON_VOLUME_QSS = QStringLiteral(R"(
    QPushButton {
        font-size: 14px;
        qproperty-iconSize: 20px;
        border: 2px solid transparent;
        border-radius: 5px;
        padding: 5px;
        text-align: left;
    }
    QPushButton:hover {
        border: 2px solid transparent;
        background-color: lightgray;
    }
)");

inline const auto BUTTON_SQUARE_QSS = QStringLiteral(R"(
    QPushButton {
        qproperty-iconSize: 18px;
        border: 1px solid transparent;
        border-radius: 10px;
        padding: 2px;
    }
    QPushButton:hover {
        border: 1px solid transparent;
        background-color: lightgray;
    }
)");

inline const auto BUTTON_COLLAPSE_QSS = QStringLiteral(R"(
    QPushButton {
        qproperty-iconSize: 20px;
        border: 2px solid transparent;
        border-radius: 5px;
        font-weight: bold;
        padding: 5px;
    }
    QPushButton:hover {
        border: 2px solid transparent;
        background-color: lightgray;
    }
)");

inline const auto BUTTON_WINDOW_QSS = QStringLiteral(R"(
    QPushButton {
        qproperty-iconSize: 18px;
        border: 2px solid transparent;
        padding: 0px;
    }
    QPushButton:hover {
        border: 0px solid transparent;
        background-color: lightgray;
    }
)");

inline const auto BUTTON_RADIUS_TEMPLATE_QSS = QStringLiteral(R"(
    QPushButton {
        qproperty-iconSize: 18px;
        border: 2px solid transparent;
        padding: 0px;
        border-top-right-radius: %1px;
    }
    QPushButton:hover {
        border: 0px solid transparent;
        background-color: lightgray;
    }
)");

inline const auto CONFIG_ACTION_QSS = QStringLiteral(R"(
    QPushButton {
        background-color: transparent;
        border: 2px solid transparent;
        border-radius: 5px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: rgb(200, 200, 200);
    }
)");

inline const auto PLAY_MODE_QSS = QStringLiteral(R"(
    QPushButton {
        font-size: 14px;
        qproperty-iconSize: 18px;
        border: 2px solid transparent;
        border-radius: 5px;
        padding: 5;
        text-align: left;
    }
    QPushButton:hover {
        border: 2px solid transparent;
        background-color: lightgray;
    }
)");

// inline const auto WINDOW_QSS = QStringLiteral(R"(
//     QWidget#StyleWindow {
//         background-color: white;
//         border-radius: 10px;
//         border: 10px solid #CCCCCC;
//     }
// )");

inline const auto LIST_VIEW_QSS =
        QStringLiteral(":/qss/ListView/PlayListView.qss");
inline const auto VOLUME_SLIDER_QSS =
        QStringLiteral(":/qss/Slider/QSlider.qss");
inline const auto VOLUME_MENU_QSS =
        QStringLiteral(":/qss/QMenu/VolumeMenu.qss");
inline const auto PROGRESS_BAR_QSS =
        QStringLiteral(":/qss/Slider/ProgressBar.qss");
inline const auto VOLUME_CONTROLLER_QSS =
        QStringLiteral(":/qss/Slider/VolumeController.qss");

inline const auto VIEW_SPLITTER_QSS =
        QStringLiteral(":/qss/Splitter/Splitter.qss");
inline const auto HORIZONTAL_LINE_QSS = QStringLiteral(":/qss/QFrame/Line.qss");


inline QString readQss(const QString& qssPath)
{
    if (QFile file(qssPath); file.open(QFile::ReadOnly))
    {
        const QString qss = QString::fromUtf8(file.readAll()).trimmed();
        file.close();
        return qss;
    }
    return {};
}
}
