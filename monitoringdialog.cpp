#include "monitoringdialog.h"

MonitoringDialog::MonitoringDialog(QWidget* parent)
    : QDialog(parent)
{
    auto topLayout = new QVBoxLayout;
    setLayout(topLayout);

    UpdateUi::style(m_coordinateValue);
    topLayout->addWidget(&m_coordinateValue);


}
