#include "initializerdialog.h"

InitializerDialog::InitializerDialog(UpdateUi* updateUi, QWidget* parent)
    : QDialog(parent)
    , m_updateUi(updateUi)
{
    auto vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine
    auto engineLabel = new QLabel(tr("Simulation Engine"));
    vLayout->addWidget(engineLabel);

    m_engineCombo = new QComboBox;
    m_engineCombo->setFocusPolicy(Qt::NoFocus);
    m_engineCombo->setInsertPolicy(QComboBox::NoInsert);
    m_engineCombo->addItems(m_updateUi->NBODY_ENGINE_MAP->values());
    vLayout->addWidget(m_engineCombo);

    // Initial Conditions Preset
    auto presetLabel = new QLabel(tr("Initial Conditions Preset"));
    vLayout->addWidget(presetLabel);

    m_presetCombo = new QComboBox;
    m_presetCombo->setFocusPolicy(Qt::NoFocus);
    m_presetCombo->setInsertPolicy(QComboBox::NoInsert);
    m_presetCombo->addItems(m_updateUi->INITIAL_CONDITION_MAP->values());
    vLayout->addWidget(m_presetCombo);

    // Time/Frame
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    vLayout->addWidget(timePerFrameLabel);

    m_timePerFrameValue = new QLineEdit;
    m_timePerFrameValue->setAlignment(Qt::AlignRight);
    vLayout->addWidget(m_timePerFrameValue);

    // Mass
    auto massLabel = new QLabel(tr("Mass (Avg.) (kg)"));
    vLayout->addWidget(massLabel);

    auto massQuantity = new QLineEdit("5.972e+24f");
    vLayout->addWidget(massQuantity);

    auto massRandomCheck = new QCheckBox(tr("Random Mass"));
    massRandomCheck->setChecked(true);
    massRandomCheck->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(massRandomCheck);

    auto massRangeLabel = new QLabel(tr("Mass Range (kg)"));
    vLayout->addWidget(massRangeLabel);

    auto massRangeValue = new QLineEdit("1.0e+10");
    vLayout->addWidget(massRangeValue);

    // OK
    auto okButton = new QPushButton(tr("OK"));
    okButton->setFocusPolicy(Qt::NoFocus);
    okButton->setDefault(true);
    vLayout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, &InitializerDialog::okButtonClicked);

    // Cancel
    auto cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(cancelButton);
    connect(cancelButton, &QPushButton::clicked, this, &InitializerDialog::close);

    setLayout(vLayout);
}

void InitializerDialog::okButtonClicked()
{
    bool allOk = true;

    m_engineIndex = m_engineCombo->currentIndex();
    m_presetIndex = m_presetCombo->currentIndex();

    bool ok;
    auto val = m_timePerFrameValue->text().toFloat(&ok);
    if (ok) {
        QPalette Pal(palette());
        m_timePerFrameValue->setPalette(Pal);
        m_timePerFrame = val;
    } else {
        allOk = false;
        QPalette Pal(palette());
        Pal.setColor(QPalette::Base, QColor(255,200,200));
        m_timePerFrameValue->setPalette(Pal);
    }

    if (allOk) {
        accept();
    }
}

int InitializerDialog::engineIndex()
{
    return m_engineIndex;
}

int InitializerDialog::presetIndex()
{
    return m_presetIndex;
}

void InitializerDialog::setTimePerFrame(const float time)
{
    m_timePerFrameValue->setText(QString::number(time));
}

float InitializerDialog::timePerFrame()
{
    return m_timePerFrame;
}
