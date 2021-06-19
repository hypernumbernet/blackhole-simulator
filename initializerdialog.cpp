#include "initializerdialog.h"

InitializerDialog::InitializerDialog(UpdateUi* updateUi, QWidget* parent)
    : QDialog(parent)
    , m_updateUi(updateUi)
{
    auto vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine
    auto engineLabel = new QLabel;
    engineLabel->setText(tr("Simulation Engine"));
    vLayout->addWidget(engineLabel);

    m_engineCombo = new QComboBox;
    m_engineCombo->setFocusPolicy(Qt::NoFocus);
    m_engineCombo->setInsertPolicy(QComboBox::NoInsert);
    m_engineCombo->addItems(m_updateUi->NBODY_ENGINE_MAP->values());
    vLayout->addWidget(m_engineCombo);

    // Initial Conditions Preset
    auto presetLabel = new QLabel;
    presetLabel->setText(tr("Initial Condition Preset"));
    vLayout->addWidget(presetLabel);

    m_presetCombo = new QComboBox;
    m_presetCombo->setFocusPolicy(Qt::NoFocus);
    m_presetCombo->setInsertPolicy(QComboBox::NoInsert);
    m_presetCombo->addItems(m_updateUi->INITIAL_CONDITION_MAP->values());
    vLayout->addWidget(m_presetCombo);

    // Mass
    auto massLabel = new QLabel;
    massLabel->setText(tr("Mass (Avg.) (kg)"));
    vLayout->addWidget(massLabel);

    auto massQuantity = new QLineEdit("5.972e+24f");
    vLayout->addWidget(massQuantity);

    auto massRandomCheck = new QCheckBox(tr("Random Mass"));
    massRandomCheck->setChecked(true);
    massRandomCheck->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(massRandomCheck);

    auto massRangeLabel = new QLabel;
    massRangeLabel->setText(tr("Mass Range (kg)"));
    vLayout->addWidget(massRangeLabel);

    auto massRangeValue = new QLineEdit("1.0e+10");
    vLayout->addWidget(massRangeValue);

    // OK
    auto newButton = new QPushButton;
    newButton->setFocusPolicy(Qt::NoFocus);
    newButton->setText(tr("OK"));
    vLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, &InitializerDialog::newButtonClicked);

    // Cancel
    auto cancelButton = new QPushButton;
    cancelButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setText(tr("Cancel"));
    vLayout->addWidget(cancelButton);
    connect(cancelButton, &QPushButton::clicked, this, &InitializerDialog::close);

    setLayout(vLayout);
}

void InitializerDialog::newButtonClicked()
{
    m_engineIndex = m_engineCombo->currentIndex();
    m_presetIndex = m_presetCombo->currentIndex();
    accept();
}

int InitializerDialog::engineIndex()
{
    return m_engineIndex;
}

int InitializerDialog::presetIndex()
{
    return m_presetIndex;
}
