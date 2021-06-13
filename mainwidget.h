#pragma once

#include "graphicwindow.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

#include <QScreen>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

protected:
    void initUi();

private:
    GraphicWindow *graphicWindows;
    QWidget *container;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;

    int particleNumber;
};
