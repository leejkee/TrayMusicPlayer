//
// Created by 31305 on 25-6-18.
//
#pragma once
#include <QObject>
namespace Tray::Core
{
    class LyricParserPrivate;
    class LyricParser final : public QObject
    {
        Q_OBJECT
    public:
        explicit LyricParser(QObject *parent = nullptr);

    public Q_SLOTS:
        void loadLyricFile(const QString &lyricPath);
    private:
        std::unique_ptr<LyricParserPrivate> d;
    };

}


