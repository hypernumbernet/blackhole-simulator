#pragma once

#include "updateui.h"

#include <QDialog>
#include <QLabel>

class MonitoringDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MonitoringDialog(QWidget* parent);

private:
    QLabel m_coordinateValue;

};
