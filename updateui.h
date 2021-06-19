#pragma once

#include <QObject>
#include <QMap>

class UpdateUi : public QObject
{
    Q_OBJECT
public:
    static constexpr int SCALE_SLIDER_CENTER = 5000;
    const QMap<int, QString>* const NBODY_ENGINE_MAP;

    explicit UpdateUi()
        : NBODY_ENGINE_MAP(new QMap<int, QString>{
    {0, tr("Gravity3DMassDifferential")},
    {1, tr("Gravity3DMassIntegral")},
//    {0, tr("Gravity3DDifferential")},
//    {0, tr("Gravity3DIntegral")},
//    {0, tr("Gravity2DDifferential")},
//    {0, tr("Gravity2DIntegral")},
        })
    { };

signals:
    void displayNumberOfParticles(QString);
    void displayFrameNumber(int);
    void displayFps(int);
    void displayModelScale(float);
    void displayTimePerFrame(float);
    void updateStartButtonText(bool);
    void displayEngineName(QString);
};
