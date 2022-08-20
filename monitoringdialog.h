#pragma once

#include "graphicwindow.h"
#include <QDialog>
#include <QPlainTextEdit>

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
