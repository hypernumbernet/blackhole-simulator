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

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);

public slots:
    void counterUpdate(int);
    void fpsUpdate(int);

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

    UpdateUi* const m_updateUi;
};
