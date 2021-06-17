#pragma once

#include <QObject>

class UpdateUi : public QObject
{
    Q_OBJECT
public:
    explicit UpdateUi(QObject* = nullptr) {};

signals:
    void showNumberOfParticles(const QString&);
    void showFrameNumber(int);
    void showFps(int);
    void showModelScale(const QString&);
    void showTimePerFrame(const QString&);
    void setStartButtonTest(bool);
};
