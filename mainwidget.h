#pragma once

#include "graphicwindow.h"
#include "updateui.h"

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
    void displayFps(int);
    void updateStartButtonText(bool);
    void displayModelScale(float);
    void resetScaleSlider();
    void displayTimePerFrame(float);
    //void displaySimulationTime(float);

protected:
    void initUi();

private:
    QLCDNumber* newCounterQLCDNumber(int);
    QLabel* newNumberQLabel();

    GraphicWindow* m_graphicWindows;
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;

    QLCDNumber* m_counterLcd;
    QLCDNumber* m_fpsLCD;
    QPushButton* m_startBtn;
    QPushButton* m_frameAdvanceBtn;
    QLineEdit* m_scaleValue;
    QSlider* m_scaleSlider;
    QLabel* m_simTimeValue;
    float m_timePerFrame;
    QLabel* m_timePerFrameValue;

    UpdateUi* const m_updateUi;
};
