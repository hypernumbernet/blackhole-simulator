#pragma once

#include <QObject>

class UpdateUi : public QObject
{
    Q_OBJECT
public:
    explicit UpdateUi(QObject* = nullptr) {};

signals:
    void showNumberOfParticles(QString);
    void showFrameNumber(int);
    void showFps(int);
    void showModelScale(QString);
    void showTimePerFrame(QString);
    void setStartButtonTest(bool);
};
