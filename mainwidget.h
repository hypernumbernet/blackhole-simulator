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
    void displayEngineName(QString);
    void reset();

protected:
    void closeEvent(QCloseEvent*) override;

private:
    void initUi();

    QLCDNumber* newCounterQLCDNumber(int);
    QLabel* newNumberQLabel();

    GraphicWindow* m_graphicWindows;
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;

    QLCDNumber* m_frameNumberLCD;
    QLCDNumber* m_fpsLCD;
    QPushButton* m_startButton;
    QPushButton* m_frameAdvanceButton;
    QLineEdit* m_scaleValue;
    QSlider* m_scaleSlider;
    QLabel* m_simTimeValue;
    float m_timePerFrame;
    QLabel* m_timePerFrameValue;
    QLabel* m_simEngineValue;

    InitializerDialog* m_initializerDialog = nullptr;

    UpdateUi* const m_updateUi;
};
