//
// Created by cww on 25-4-1.
//
#pragma once
#include <QPushButton>
#include <memory>

namespace Tray::Ui::Panel
{
class StyleButtonPrivate;
class StyleButton final : public QPushButton
{
    Q_OBJECT

public:
    StyleButton(const QString& id
                , const QSize& size
                , const QString& iconPath
                , const QString& qssPath
                , QWidget* parent = nullptr);

    explicit StyleButton(QWidget* parent = nullptr);

    ~StyleButton() override;

    void setQss(const QString& qssPath);

    void setId(const QString& id);

    void setButtonIcon(const QString& iconPath);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

Q_SIGNALS:
    void signalButtonClicked(const QString& listName);

private Q_SLOTS:
    void onButtonClicked();

private:
    std::unique_ptr<StyleButtonPrivate> d;
};
}
