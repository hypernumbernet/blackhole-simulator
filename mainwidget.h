#pragma once

#include "graphicwindow.h"
#include "updateui.h"
#include "initializerdialog.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QScreen>
#include <QLCDNumber>
#include <QComboBox>
#include <QLineEdit>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);

public slots:
    void displayFrameNumber(int);
    void displayFPS(int);
    void updateStartButtonText(bool);
    void displayModelScale(float);
    void resetScaleSlider();
    void displayTimePerFrame(float);
    void showInitializerDialog();
    void acceptInitializerDialog();
    void displayEngineName(const QString&);
    void resetInitial();
    void displayPresetName(const QString&);
    void displatNumberOfParticles(int);

private:
    void initUi();
    void reset(const bhs::SimCondition&);

    void displayStyle(QLCDNumber* const);
    void displayStyle(QLabel* const);

    UpdateUi* const m_updateUi;
    GraphicWindow* const m_graphicWindows;
    QHBoxLayout* const m_hLayout;
    QVBoxLayout* const m_vLayout;

    QLCDNumber* const m_frameNumberLCD;
    QLCDNumber* const m_fpsLCD;
    QPushButton* const m_startButton;
    QPushButton* const m_frameAdvanceButton;
    QLineEdit* const m_scaleValue;
    QSlider* const m_scaleSlider;
    QLabel* const m_simTimeValue;
    QLabel* const m_timePerFrameValue;
    QLabel* const m_engineValue;
    QLabel* const m_presetValue;
    QLabel* const m_particleNumValue;

//    float m_timePerFrame;
//    int m_numberOfParticles;
    bhs::SimCondition m_simCondition;

    InitializerDialog* m_initializerDialog = nullptr;
};
