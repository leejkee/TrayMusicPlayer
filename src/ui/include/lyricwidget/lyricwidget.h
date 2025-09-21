//
// Created by 31305 on 2025/7/16.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui
{

class LyricWidgetPrivate;
class LyricWidget final : public QWidget
{
    Q_OBJECT
    public:
    explicit LyricWidget(QWidget *parent = nullptr);
    ~LyricWidget() override;

    [[nodiscard]] int currentIndex() const;

public Q_SLOTS:
    void updateCurrentTiming(int index);

    void updateLyric(const QStringList& lyricText, const QList<int64_t>& lyricsTiming);

private:
    std::unique_ptr<LyricWidgetPrivate> d;
};
}
