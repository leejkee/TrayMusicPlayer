#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPropertyAnimation>


namespace Tray::Ui::Panel {
    class RotatingLabel final : public QWidget {
        Q_OBJECT
        Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

    public:
        enum RotatingLabelMode {
            Rotating,
            NoRotating,
        };

        /// Constructs an empty RotatingLabel. Image and mode can be set later.
        /// @param parent The parent widget.
        explicit RotatingLabel(QWidget *parent = nullptr);

        /// Constructs a RotatingLabel with the given image, size, and display mode.
        /// @param logo The image to be displayed and rotated.
        /// @param targetSize The desired display size of the image.
        /// @param mode The display mode (e.g., static or rotating).
        /// @param parent The parent widget.
        explicit RotatingLabel(const QPixmap &logo, const QSize &targetSize, RotatingLabelMode mode = Rotating,
                               QWidget *parent = nullptr);

        void setLabelMode(RotatingLabelMode mode);

        void setPixmap(const QPixmap &pixmap);

        void setPixmapWithSmoothScale(const QPixmap &pixmap, const QSize &targetSize);

        [[nodiscard]] qreal rotation() const;

        void setRotation(qreal angle);

        /// Initializes the rotation animation parameters.
        /// @param startV The starting value of the rotation (in degrees).
        /// @param endV The ending value of the rotation (in degrees).
        /// @param msecs Duration of one rotation cycle in milliseconds.
        /// @param loopCount Number of times the animation should repeat (-1 for infinite).
        void initRotation(const QVariant &startV, const QVariant &endV, int msecs, int loopCount);

    signals:
        void rotationChanged(qreal angle);

    protected:
        void paintEvent(QPaintEvent *event) override;

        [[nodiscard]] QSize sizeHint() const override;

    private:
        QPixmap m_pixmap;
        qreal m_rotation = 0.0;
        QPropertyAnimation *m_rotationAnimation;
    };
}
