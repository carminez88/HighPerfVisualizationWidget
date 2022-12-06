#ifndef IMAGEVIEWBASE_H
#define IMAGEVIEWBASE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QGraphicsRectItem>
#include <QWheelEvent>
#include <QRubberBand>

#include <memory>
#include <optional>

class ImageViewBase : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewBase(QWidget* a_parent = nullptr);

    ~ImageViewBase() override = default;

    void updateImage(QImage a_img, bool a_zoomFit = true);

protected:
    static bool imageIsValid(const QImage& a_img);

    void resizeEvent(QResizeEvent* a_evnt) override;

    void wheelEvent(QWheelEvent * a_evnt) override;

    void enterEvent(QEvent *event) override;

    void zoomIn(int32_t a_zoomLevel);

    void zoomOut(int32_t a_zoomLevel);

    void zoomFit();

    void setTransMatrix();

protected:
    QGraphicsScene* m_scene { nullptr };
    QGraphicsPixmapItem* m_pixmapItem { nullptr };
    int32_t m_zoom { 0 };
    bool m_fit { true };

private:
    QImage m_img;
    QGraphicsPixmapItem m_pixmap;
};

#endif // IMAGEVIEW_H
