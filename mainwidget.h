﻿#pragma once

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

public slots:
    void counterUpdate();

protected:
    void initUi();

private:
    GraphicWindow *graphicWindows;
    QWidget *container;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    QLCDNumber* counterLcd;
};