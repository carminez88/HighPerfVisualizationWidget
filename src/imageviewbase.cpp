#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QMatrix>
#include <QRect>
#include <cmath>
#include <iostream>
#include <cstdint>
#include <cstring>

#include "imageviewbase.h"

ImageViewBase::ImageViewBase(QWidget *a_parent)
    : QGraphicsView ( a_parent                      )
    , m_scene       ( new QGraphicsScene( nullptr ) )
    , m_pixmapItem  ( new QGraphicsPixmapItem       )
{
    setScene( m_scene );

    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(true);
    setMouseTracking(true);

    m_scene->addItem( m_pixmapItem );

    m_pixmapItem->setPos( 0, 0 );

    show();
}

void ImageViewBase::updateImage(QImage a_img, bool a_zoomFit)
{
    if ( not imageIsValid( a_img ) ) {

        a_img.fill( Qt::black );

        a_img = a_img.copy();

    }

    if ( a_zoomFit )
        zoomFit();

    std::swap( m_img, a_img );

    m_pixmapItem->setPixmap( QPixmap::fromImage( m_img ) );
}

void ImageViewBase::resizeEvent(QResizeEvent *a_evnt)
{
    QGraphicsView::resizeEvent( a_evnt );

    if ( not m_scene->items().isEmpty() )
        zoomFit();
}

bool ImageViewBase::imageIsValid(const QImage &a_img)
{
    return not a_img.isNull();
}

void ImageViewBase::wheelEvent(QWheelEvent *a_evnt)
{
    if ( a_evnt->modifiers() == Qt::NoModifier ) {

        if ( a_evnt->delta() > 0 )
            zoomIn( 3 );
        else if ( a_evnt->delta() < 0 )
            zoomOut( 3 );

        a_evnt->accept();
    }
    else
        QGraphicsView::wheelEvent( a_evnt );
}

void ImageViewBase::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent( event );

    viewport()->setCursor( Qt::CrossCursor );
}

void ImageViewBase::zoomIn(int32_t a_zoomLevel)
{
    m_zoom += a_zoomLevel;

    m_fit = false;

    setTransMatrix();
}

void ImageViewBase::zoomOut(int32_t a_zoomLevel)
{
    m_zoom -= a_zoomLevel;

    m_fit = false;

    setTransMatrix();
}

void ImageViewBase::zoomFit()
{
    fitInView( m_pixmapItem, Qt::KeepAspectRatio );

    m_zoom = int32_t( 10.0 * std::log2( matrix().m11() ) );

    m_fit = true;
}

void ImageViewBase::setTransMatrix()
{
    const double_t l_scale = std::pow( 2.0, m_zoom / 10.0 );

    QMatrix l_matrix;
    l_matrix.scale( l_scale, l_scale );

    setMatrix( l_matrix );
}
