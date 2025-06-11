#include <QWidget>

class QPushButton;
class QListWidget;
class QLabel;

namespace Tray::Ui {
class FilePathConfigWidget : public QWidget {
public:
    Q_OBJECT

public:
    explicit FilePathConfigWidget(QWidget* parent = nullptr);

private:
    QPushButton* m_addButton;
    QPushButton* m_removeButton;
    QListWidget* m_listWidget;
    QLabel* m_messageLabel;

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