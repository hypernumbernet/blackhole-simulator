#pragma once

#include <QObject>

class UpdateUi : public QObject
{
    Q_OBJECT
public:
    explicit UpdateUi() {};

    static constexpr int SCALE_SLIDER_CENTER = 5000;

signals:
    void displayNumberOfParticles(QString);
    void displayFrameNumber(int);
    void displayFps(int);
    void displayModelScale(float);
    void displayTimePerFrame(float);
    void updateStartButtonText(bool);
    void displayEngineName(QString);
};
