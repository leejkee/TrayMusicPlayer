//
// Created by cww on 25-4-2.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui::Panel
{
class VolumeControllerPrivate;
class VolumeController final : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeController(QWidget* parent = nullptr);

    ~VolumeController() override;

    void setSliderVolumeValue(int v);

protected:
    QSize sizeHint() const override;


Q_SIGNALS:
    void signalSetValue(int);

    void signalSetMute();

public Q_SLOTS:
    void setVolumeButtonIcon(bool);

private:
    std::unique_ptr<VolumeControllerPrivate> d;

    void setLabelVolume(unsigned v);
};
}
