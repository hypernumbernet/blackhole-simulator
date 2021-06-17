#pragma once

#include <QObject>

class UpdateUi : public QObject
{
    Q_OBJECT
public:
    explicit UpdateUi(QObject* = nullptr) {};

    static constexpr int SCALE_SLIDER_CENTER = 5000;

signals:
    void showNumberOfParticles(const QString&);
    void showFrameNumber(int);
    void showFps(int);
    void showModelScale(float);
    void showTimePerFrame(const QString&);
    void setStartButtonTest(bool);
};
