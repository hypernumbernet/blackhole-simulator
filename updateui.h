#pragma once

#include "global.h"
#include <QLabel>
#include <QLCDNumber>

class UpdateUi : public QObject
{
    Q_OBJECT
public:

    static constexpr int SCALE_SLIDER_CENTER = 5000;
    static UpdateUi& it();
    static QHash<bhs::Engine, QString>& engine();
    static QList<bhs::Engine>& engineUse();
    static QMap<bhs::Precision, QString>& precision();
    static QMap<bhs::Compute, QString>& compute();
    static QMap<bhs::Preset, QString>& preset();
    static QMap<bhs::LineType, QString>& lineType();
    static void style(QLCDNumber& lcd);
    static void style(QLabel& lbl);
    static QString debugString(quintptr);

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
