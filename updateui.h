#pragma once

#include "global.h"

#include <QMap>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>

class UpdateUi : public QObject
{
    Q_OBJECT
public:

    static constexpr int SCALE_SLIDER_CENTER = 5000;

    const QMap<bhs::Engine, QString>* const ENGINE;
    const QMap<bhs::Precision, QString>* const PRECISION;
    const QMap<bhs::Compute, QString>* const COMPUTE;
    const QMap<bhs::Preset, QString>* const PRESET;

    static UpdateUi& it()
    {
        static UpdateUi instance;
        return instance;
    }

private:
    UpdateUi()
        : ENGINE(new QMap<bhs::Engine, QString>
        {
            {bhs::Engine::G3DMassDiff, tr("Gravity 3D Mass Differential")},
            {bhs::Engine::G3D4DMassDiff, tr("Gravity 3S-Velocity M. Diff.")},
            //{0, tr("Gravity3DIntegral")},
            //{0, tr("Gravity2DDifferential")},
            //{0, tr("Gravity2DIntegral")},
        })
        , PRECISION(new QMap<bhs::Precision, QString>
        {
            {bhs::Precision::Float, tr("Float")},
            {bhs::Precision::Double, tr("Double")},
        })
        , COMPUTE(new QMap<bhs::Compute, QString>
        {
            {bhs::Compute::CPU, tr("CPU")},
            {bhs::Compute::GPU, tr("GPU")},
        })
        , PRESET(new QMap<bhs::Preset, QString>
        {
            {bhs::Preset::RandomCube, tr("Random Cube")},
            {bhs::Preset::RandomSphere, tr("Random Sphere")},
            {bhs::Preset::RandomBall, tr("Random Ball")},
            {bhs::Preset::SunEarth, tr("Sun Earth")},
            {bhs::Preset::EarthMoon, tr("Earth Moon")},
            {bhs::Preset::SunEarthVenus, tr("Sun Earth Venus")},
            {bhs::Preset::EarthSun, tr("Test Earth Sun swap")},
            {bhs::Preset::TestSamePosition, tr("Test Same Position")},
        })
    {
    };
    UpdateUi(const UpdateUi&);

signals:
    void displayNumberOfParticles(int);
    void displayFrameNumber(int);
    void displayFps(int);
    void displayModelScale(double);
    void displayTimePerFrame(double);
    void updateStartButtonText(bool);
    void frameAdvance(int);
    void resultReady();
    void resetParticles();
    void displayEngineName(bhs::Engine);
    void displayPrecision(bhs::Precision);
    void displayPresetName(bhs::Preset);
};
