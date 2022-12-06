#ifndef SPAMMER_H
#define SPAMMER_H

#include <QtConcurrent>
#include <QObject>

class Spammer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Spammer(QObject* parent = nullptr);

    void run() override;

Q_SIGNALS:
    void spam(const std::vector<uint8_t>&);

};

#endif // SPAMMER_H
