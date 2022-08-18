#pragma once

#include <QDialog>
#include <QPushButton>
#include <QPlainTextEdit>

#include "updateui.h"
#include "graphicwindow.h"

class MonitoringDialog : public QDialog
{
    Q_OBJECT
public:
    MonitoringDialog(QWidget* parent, GraphicWindow*);
    void update();

private:
    GraphicWindow* const m_graphicWindow;

    QLabel m_frameValue;
    QPlainTextEdit m_dataTextEdit;
};
