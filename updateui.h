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

    static UpdateUi& it()
    {
        static UpdateUi instance;
        return instance;
    }

    static QMap<bhs::Engine, QString>& engine()
    {
        static QMap<bhs::Engine, QString> map = {
            {bhs::Engine::G3D, tr("Gravity 3D")},
            {bhs::Engine::G3D4D, tr("Gravity 3S-Velocity")},
            {bhs::Engine::G3D4DR1, tr("Gravity Lorentz Trans.")},
            {bhs::Engine::G4D3D, tr("Gravity 3S-Coordinate")},
        };
        return map;
    }

    static QMap<bhs::Precision, QString>& precision()
    {
        static QMap<bhs::Precision, QString> map = {
            {bhs::Precision::Float, tr("Float")},
            {bhs::Precision::Double, tr("Double")},
        };
        return map;
    }

    static QMap<bhs::Compute, QString>& compute()
    {
        static QMap<bhs::Compute, QString> map = {
            {bhs::Compute::CPU, tr("CPU")},
            {bhs::Compute::GPU, tr("GPU")},
        };
        return map;
    }

    static QMap<bhs::Preset, QString>& preset()
    {
        static QMap<bhs::Preset, QString> map = {
            {bhs::Preset::RandomCube, tr("Random Cube")},
            {bhs::Preset::RandomSphere, tr("Random Sphere")},
            {bhs::Preset::RandomBall, tr("Random Ball")},
            {bhs::Preset::SunEarth, tr("Sun Earth")},
            {bhs::Preset::EarthMoon, tr("Earth Moon")},
            {bhs::Preset::SunEarthVenus, tr("Sun Earth Venus")},
            {bhs::Preset::SunMercury, tr("Sun Mercury")},
            {bhs::Preset::EarthSun, tr("Test Earth Sun swap")},
            {bhs::Preset::TestSamePosition, tr("Test Same Position")},
        };
        return map;
    }

private:
    UpdateUi(){};
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
