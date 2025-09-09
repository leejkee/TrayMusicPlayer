#pragma once
#include <QDialog>
#include <memory>

namespace Tray::Ui::Panel {

class ConfigWidgetPrivate;
class ConfigWidget : public QWidget {
public:
    explicit ConfigWidget(QWidget* parent = nullptr);

    void addConfigWidget(const QString& actionName, QWidget* w);
    void addConfigWidget(const QString& buttonId
                         , const QSize& size
                         , const QString& iconPath
                         , const QString& qssPath
                         , QWidget* w);

private:
    std::unique_ptr<ConfigWidgetPrivate> d;


private Q_SLOTS:
    void showActionArea(const QString &actionName);

};
} // namespace Tray::Ui