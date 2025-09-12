//
// Created by cww on 25-4-1.
//
#include <stylebutton/stylebutton.h>
#include <log/log.h>
#include <QEvent>
#include <QFile>

namespace Tray::Ui::Panel
{
class StyleButtonPrivate
{
public:
    QString m_id;
    QSize m_size;
    QString m_iconPath;
    QString m_qssPath;
};

StyleButton::StyleButton(const QString& id
                         , const QSize& size
                         , const QString& iconPath
                         , const QString& qssPath
                         , QWidget* parent)
    : QPushButton(parent),
      d(std::make_unique<StyleButtonPrivate>())
{
    setId(id);
    setButtonIcon(iconPath);
    setQss(qssPath);
    if (!size.isEmpty())
    {
        d->m_size = size;
        setFixedSize(d->m_size);
    }
    connect(this, &QPushButton::clicked, this, &StyleButton::onButtonClicked);
    installEventFilter(this);
}

StyleButton::StyleButton(QWidget* parent)
    : StyleButton({}, {}, {}, {}, parent)
{
}

StyleButton::~StyleButton() = default;


bool StyleButton::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this)
    {
        if (event->type() == QEvent::Enter)
        {
            setCursor(Qt::PointingHandCursor);
        }
        else if (event->type() == QEvent::Leave)
        {
            setCursor(Qt::ArrowCursor);
        }
    }
    return QPushButton::eventFilter(watched, event);
}

void StyleButton::setQss(const QString& qssPath)
{
    if (qssPath.isEmpty())
    {
        LOG_WARNING("Empty qssPath");
    }
    d->m_qssPath = qssPath;
    if (QFile file(qssPath); file.open(QFile::ReadOnly))
    {
        const QString qss = QString::fromUtf8(file.readAll()).trimmed();
        file.close();
        setStyleSheet(qss);
        LOG_INFO(QString("QSS [%1] has been loaded.").arg(qssPath));
    }
    else
    {
        LOG_WARNING(QString("Load QSS [%1] failed.").arg(qssPath));
    }
}

void StyleButton::setId(const QString& id)
{
    if (id.isEmpty())
    {
        LOG_WARNING("Empty id");
    }
    d->m_id = id;
    setText(d->m_id);
}

void StyleButton::setButtonIcon(const QString& iconPath)
{
    if (iconPath.isEmpty())
    {
        LOG_WARNING("Empty iconPath");
    }
    d->m_iconPath = iconPath;
    setIcon(QIcon(iconPath));
}

void StyleButton::onButtonClicked()
{
    Q_EMIT signalButtonClicked(d->m_id);
}
}
