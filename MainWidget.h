#pragma once

#include "GraphicWindow.h"
#include "InitializerDialog.h"
#include "MonitoringDialog.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);

public slots:
    void displayFrameNumber(int);
    void displayFPS(int);
    void updateStartButtonText(bool);
    void displayModelScale(double);
    void resetScaleSlider();
    void displayTimePerFrame(double);
    void resetInitial();
    void displayNumberOfParticles(int);
    void displayEngineName(bhs::Engine);
    void displayPrecision(bhs::Precision);
    void displayPresetName(bhs::Preset, QString);
    void displayCompute(bhs::Compute);

    void showInitializerDialog();
    void acceptInitializerDialog();
    void showMonitoringDialog();

private:
    void initUi();
    void reset(const bhs::SimCondition&);

    bhs::SimCondition m_simCondition;
    GraphicWindow m_graphicWindows;
    QHBoxLayout m_hLayout;
    QVBoxLayout m_vLayout;

    QLCDNumber m_frameNumberLCD;
    QLCDNumber m_fpsLCD;
    QPushButton m_startButton;
    QPushButton m_frameAdvance1;
    QPushButton m_frameAdvance10;
    QPushButton m_frameAdvance100;
    QLineEdit m_scaleValue;
    QSlider m_scaleSlider;
    QLabel m_simTimeValue;
    QLabel m_timePerFrameValue;
    QLabel m_engineValue;
    QLabel m_presetValue;
    QLabel m_particleNumValue;
    QLabel m_precisionValue;
    QLabel m_computeValue;

    InitializerDialog* m_initializerDialog = nullptr;
    MonitoringDialog* m_monitoringDialog = nullptr;
};
