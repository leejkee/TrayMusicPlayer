//
// Created by cww on 25-6-12.
//

#pragma once
#include <QSize>

struct BetterButtonMetaData {
    QString name;
    QString iconPath;
    QString qssPath;
    int height;
    int width;

    BetterButtonMetaData(const QString &name, const QString &iconPath, const QString &qss, const int h, const int w)
        : name(name), iconPath(iconPath), qssPath(qss), height(h), width(w) {
        if (height < 0) {
            height = 0;
        }
        if (width < 0) {
            width = 0;
        }
    }

    BetterButtonMetaData() : BetterButtonMetaData("", "", "", 30, 30) {
    }

    BetterButtonMetaData(const QString &name, const QString &iconPath, const QString &qss, const QSize &size) : BetterButtonMetaData(
        name, iconPath, qss, size.width(), size.height()) {
    }

    BetterButtonMetaData(const BetterButtonMetaData &other) : BetterButtonMetaData(
        other.name, other.iconPath, other.qssPath, other.height, other.width) {
    }

    BetterButtonMetaData &operator=(const BetterButtonMetaData &other) {
        if (this != &other) {
            height = other.height;
            width = other.width;
            name = other.name;
            iconPath = other.iconPath;
            qssPath = other.qssPath;
        }
        return *this;
    }
};
