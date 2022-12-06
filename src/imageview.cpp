#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QMatrix>
#include <QRect>
#include <cmath>
#include <QElapsedTimer>
#include <QPixmapCache>
#include <iostream>
#include <cstdint>
#include <cstring>

#include "imageview.h"

ImageView::ImageView(QWidget *a_parent)
    : ImageViewBase ( a_parent                                        )
    , m_rb          ( new QRubberBand( QRubberBand::Rectangle, this ) )
{
}

ImageView::~ImageView()
{
    if ( m_image )
        delete m_image;

    if ( m_pixmap )
        delete m_pixmap;
}

void ImageView::init(int32_t a_width, int32_t a_height, QImage::Format a_format)
{
    const bool l_haveDifferentWidth = not m_image or a_width != m_image->width();
    const bool l_haveDifferentFormat = not m_image or a_format != m_image->format();
    const bool l_shouldRepaint = l_haveDifferentWidth or l_haveDifferentFormat;

    if ( l_shouldRepaint ) {

        delete m_image;
        delete m_pixmap;

        m_image = new QImage( a_width, a_height, a_format );
        m_image->fill( Qt::black );

        m_pixmap = new QPixmap();
        *m_pixmap = QPixmap::fromImage( *m_image );

        m_pixmapItem->setPixmap( *m_pixmap );

        m_pixmapItem->update();
        m_scene->update();

        fitInView( m_pixmapItem, Qt::KeepAspectRatio );

    }
}

void ImageView::updateImageData(const std::vector<uint8_t> &a_line)
{
    if ( a_line.empty() )
        return;

    // Scroll back by exactly one line
    //QElapsedTimer l_timer;
    //l_timer.start();
    auto *l_dst = m_image->scanLine( 0 );
    auto *l_src = m_image->scanLine( 1 );
    std::memcpy( l_dst, l_src, m_image->byteCount() - m_image->bytesPerLine() );

    //l_timer.restart();
    // Copy new buffer content in the last line
    l_src = const_cast<uint8_t*>( a_line.data() );
    l_dst = m_image->scanLine( m_image->height() - 1 );
    std::memcpy( l_dst, l_src, m_image->bytesPerLine() );

    //l_timer.restart();
    // Update viewer
    *m_pixmap = QPixmap::fromImage( *m_image );

    m_pixmapItem->setPixmap( *m_pixmap );
    m_pixmapItem->update();
    //m_scene->update();
}

std::optional<QRectF> ImageView::isValidRectangle()
{
    const bool l_isValidROI = m_tl.x() < m_br.x() and m_tl.y() < m_br.y();

    QRectF l_rect;
    l_rect.setTopLeft( mapToScene( m_tl ) );
    l_rect.setBottomRight( mapToScene( m_br ) );

    const bool l_isValidHeight = static_cast<int32_t>( std::round( l_rect.top() + l_rect.height() ) ) <= m_image->height();
    const bool l_isValidWidth = static_cast<int32_t>( std::round( l_rect.left() + l_rect.width() ) ) <= m_image->width();

    return ( l_isValidROI && l_isValidHeight && l_isValidWidth ) ? std::make_optional( l_rect ) : std::nullopt;
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::RightButton ) {

        m_selectingROI = true;

        m_tl = event->pos();

        m_rb->setGeometry( QRect( m_tl, QSize() ) );
        m_rb->show();

    } else
        QGraphicsView::mousePressEvent( event );
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if ( m_selectingROI )
        m_rb->setGeometry( QRect( m_tl, event->pos() ).normalized() );

    QGraphicsView::mouseMoveEvent( event );
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if ( m_selectingROI ) {

        m_br = event->pos();

        if ( auto l_opt = isValidRectangle(); l_opt.has_value() ) {

            const int32_t l_ret = QMessageBox::question( this, "Select ROI", "Do you want to use current selection?" );

            if ( l_ret == QMessageBox::Yes ) {

                QImage l_crop = m_image->copy( l_opt.value().toRect() );

                Q_EMIT cropSelected( std::move( l_crop ) );

            }

        }

        m_rb->hide();

        m_selectingROI = false;

    } else {

        QGraphicsView::mouseReleaseEvent( event );

        viewport()->setCursor( Qt::CrossCursor );

    }
}
