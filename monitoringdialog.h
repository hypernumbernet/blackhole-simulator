#pragma once

#include <QDialog>
#include <QLabel>

#include "updateui.h"

class MonitoringDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MonitoringDialog(QWidget* parent);

private:
    QLabel m_coordinateValue;

};
