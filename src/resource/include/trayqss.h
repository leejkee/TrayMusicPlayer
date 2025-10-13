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

inline const auto LIST_VIEW_QSS = QStringLiteral(R"(
    QListView {
        font-size: 14px;
        font-family: "Microsoft YaHei";
        font-weight: 300;
    }
    QListView::item {
        height: 40px;
    }
)");

inline const auto VOLUME_SLIDER_QSS = QStringLiteral(R"(
    QSlider::groove:vertical {
        border: none;
        background: #ccc;
        width: 5px;
    }
    QSlider::handle:vertical {
        background: gray;
        border-radius: 5px;
        height: 10px;
        width: 10px;
        margin: 0 -3px;
    }
    QSlider::sub-page:vertical {
        background: #ccc;
        border-radius: 2px;
    }
    QSlider::add-page:vertical {
        background: #666;
        border-radius: 2px;
    }
)");

inline const auto VOLUME_MENU_QSS = QStringLiteral(R"(
    QMenu {
        background: #F5F5F5;
        border: 1px solid #DDD;
    }
    QMenu::item {
        padding: 0;
        margin: 0;
    }
)");

inline const auto PROGRESS_BAR_QSS = QStringLiteral(R"(
    QSlider::groove:horizontal {
        border: none;
        height: 4px;
        background: #ccc;
    }
    QSlider::sub-page:horizontal {
        background: #666;
        border-radius: 2px;
    }
    QSlider::add-page:horizontal {
        background: #ccc;
        border-radius: 2px;
    }
    QSlider::handle:horizontal {
        background: gray;
        border-radius: 5px;
        width: 10px;
        height: 10px;
        margin: -3px 0;
    }
    QSlider::handle:horizontal:pressed {
        background: #222;
    }
)");

inline const auto VOLUME_CONTROLLER_QSS = QStringLiteral(R"(
    QSlider::groove:vertical {
        border: none;
        width: 4px;
        background: #ccc;
    }
    QSlider::sub-page:vertical {
        background: #fff;
        border-radius: 2px;
    }
    QSlider::add-page:vertical {
        background: #666;
        border-radius: 2px;
    }
    QSlider::handle:vertical {
        background: gray;
        border-radius: 5px;
        width: 10px;
        height: 10px;
        margin: 0px -3px;
    }
    QSlider::handle:vertical:pressed {
        background: #222;
    }
)");

inline const auto VIEW_SPLITTER_QSS = QStringLiteral(R"(
    QSplitter::handle {
        background-color: gray;
    }
    QSplitter::handle:vertical {
        height: 1px;
    }
)");

inline const auto HORIZONTAL_LINE_QSS = QStringLiteral(R"(
    QFrame {
        border: none;
        background-color: #999999;
        max-height: 1px;
    }
)");
}
