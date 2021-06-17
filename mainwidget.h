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
    void counterUpdate(int);
    void fpsUpdate(int);
    void startButtonText(bool);
    void showModelScale(const QString&);

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

    UpdateUi* const m_updateUi;
};
