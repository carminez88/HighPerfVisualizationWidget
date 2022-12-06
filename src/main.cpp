#include <QApplication>

#include "imageview.h"

int32_t main(int32_t argc, char *argv[])
{
    QApplication a(argc, argv);

    constexpr int32_t k_width { 8192 };
    constexpr int32_t k_height { 500 };

    ImageView imgView;
    imgView.init( k_width, k_height, QImage::Format_Grayscale8 );
    imgView.show();

    // TODO: add data emitter

    return a.exec();
}
