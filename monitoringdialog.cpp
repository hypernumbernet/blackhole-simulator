#include "monitoringdialog.h"

MonitoringDialog::MonitoringDialog(QWidget* parent, GraphicWindow* graphicWindow)
    : QDialog(parent)
    , m_graphicWindow(graphicWindow)
{
    auto topLayout = new QVBoxLayout;
    setLayout(topLayout);

    // Update
    auto updateButton = new QPushButton(tr("Update"));
    updateButton->setFocusPolicy(Qt::NoFocus);
    topLayout->addWidget(updateButton);
    connect(updateButton, &QPushButton::clicked, this, &MonitoringDialog::update);

    auto frameNumberLayout = new QHBoxLayout;
    auto frameNumberLabel = new QLabel(tr("Frame Number"));
    frameNumberLayout->addWidget(frameNumberLabel);
    UpdateUi::style(m_frameValue);
    frameNumberLayout->addWidget(&m_frameValue);
    topLayout->addLayout(frameNumberLayout);

    m_dataTextEdit.setReadOnly(true);
    m_dataTextEdit.setMinimumSize(400, 200);
    m_dataTextEdit.setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    topLayout->addWidget(&m_dataTextEdit);

    update();
}

void MonitoringDialog::update()
{
    m_frameValue.setText(QString::number(m_graphicWindow->frameNumber()));
    m_dataTextEdit.setPlainText(m_graphicWindow->particleData());
}
