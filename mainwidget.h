#pragma once

#include "graphicwindow.h"

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
    explicit MainWidget(QWidget *parent = nullptr);

    void setNumberOfParticles(quint64);

public slots:
    void counterUpdate();
    void fpsUpdate(int);

protected:
    void initUi();

private:
    QLCDNumber* newCounterQLCDNumber(int);
    QLabel* newNumberQLabel();

    GraphicWindow* graphicWindows;
    QWidget* container;
    QHBoxLayout* hLayout;
    QVBoxLayout* vLayout;
    QLCDNumber* counterLcd;
    QLCDNumber* fpsLCD;
    QLabel* particleNumValue;
};
