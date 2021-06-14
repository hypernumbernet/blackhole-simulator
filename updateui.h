#pragma once

#include <QObject>

class UpdateUi : public QObject
{
    Q_OBJECT
public:
    explicit UpdateUi(QObject* parent = nullptr);

signals:
    void setNumberOfParticles(QString);
    void setFrameNumber(int);

};
