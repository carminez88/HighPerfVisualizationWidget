#include <random>
#include <thread>
#include <chrono>

#include "spammer.h"

Spammer::Spammer(QObject *parent)
    : QObject(parent)
{
}

void Spammer::run()
{
    using namespace std::chrono_literals;

    constexpr uint8_t k_black { 0 };
    constexpr uint8_t k_grey { 128 };
    constexpr uint8_t k_white { 0255 };
    constexpr std::size_t k_dataSize { 8192 };

    std::vector<uint8_t> data ( k_dataSize, k_grey );

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int32_t> random( k_black, k_white );

    while( true ) {

        for ( auto& d : data ) {
            d = static_cast<uint8_t>( random( mt ) );
        }

        Q_EMIT spam( data );

        std::this_thread::sleep_for( 100ns );

    }

}
