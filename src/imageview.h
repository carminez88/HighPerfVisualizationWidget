#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "imageviewbase.h"

class ImageView : public ImageViewBase
{
    Q_OBJECT
public:
    explicit ImageView(QWidget* a_parent = nullptr);

    ~ImageView() override;

    void init(int32_t a_width, int32_t a_height, QImage::Format a_format);

    void updateImageData(const std::vector<uint8_t> &a_line);

Q_SIGNALS:
    void cropSelected(QImage);

private:
    std::optional<QRectF> isValidRectangle();

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage* m_image { nullptr };
    QPixmap* m_pixmap { nullptr };
    QPoint m_tl { 0, 0 };
    QPoint m_br { 0, 0 };
    QRubberBand* m_rb { nullptr };
    bool m_selectingROI { false };
};

#endif // IMAGEVIEW_H
