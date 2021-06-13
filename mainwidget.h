#pragma once

#include "graphicwindow.h"

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>

#include <QtGui/QScreen>

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
};
