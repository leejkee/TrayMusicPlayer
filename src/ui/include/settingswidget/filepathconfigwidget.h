#pragma once

#include <QWidget>
#include <memory>

class QPushButton;
class QListWidget;
class QLabel;

namespace Tray::Ui {
class FilepathConfigWidgetPrivate;
class FilePathConfigWidget final : public QWidget {
public:
    Q_OBJECT

public:
    explicit FilePathConfigWidget(QWidget* parent = nullptr);

    ~FilePathConfigWidget() override;

private:
    std::unique_ptr<FilepathConfigWidgetPrivate> d;


Q_SIGNALS:
    void signalLocalDirAdded(const QString&);
    void signalLocalDirRemoved(const QString&);

private Q_SLOTS:
    void addMusicPath();
    void removeMusicPath();

public Q_SLOTS:
    void updateLocalPaths(const QStringList& paths);

};

} // namespace Tray::Ui