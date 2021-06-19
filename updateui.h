#pragma once

#include <QObject>
#include <QMap>

class UpdateUi : public QObject
{
    Q_OBJECT

public:

    struct SimCondition
    {
        int engine = 0;
        int preset = 0;
    };

    static constexpr int SCALE_SLIDER_CENTER = 5000;

    const QMap<int, QString>* const NBODY_ENGINE_MAP;
    const QMap<int, QString>* const INITIAL_CONDITION_MAP;

    explicit UpdateUi()
        : NBODY_ENGINE_MAP(new QMap<int, QString>{
    {0, tr("Gravity 3D Mass Differential")},
    {1, tr("Gravity 3D Mass Integral")},
//    {0, tr("Gravity3DDifferential")},
//    {0, tr("Gravity3DIntegral")},
//    {0, tr("Gravity2DDifferential")},
//    {0, tr("Gravity2DIntegral")},
        })
        , INITIAL_CONDITION_MAP(new QMap<int, QString>{
    {0, tr("Random Cube")},
    {1, tr("Sun Earth")},
    {2, tr("Earth Sun")},
    {3, tr("Earth Moon")},
    {4, tr("Sun Earth Venus")},
    {5, tr("Test Same Position")},
        })
    { };

signals:
    void displayNumberOfParticles(const QString&);
    void displayFrameNumber(int);
    void displayFps(int);
    void displayModelScale(float);
    void displayTimePerFrame(float);
    void updateStartButtonText(bool);
    void displayEngineName(const QString&);
    void displayPresetName(const QString&);
};
