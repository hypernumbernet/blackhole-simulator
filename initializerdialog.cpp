#include "initializerdialog.h"

InitializerDialog::InitializerDialog(QWidget*)
{
    auto vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine
    auto simTypeLabel = new QLabel;
    simTypeLabel->setText(tr("Simulation Engine:"));
    vLayout->addWidget(simTypeLabel);

    m_engineCombo = new QComboBox;
    m_engineCombo->setFocusPolicy(Qt::NoFocus);
    m_engineCombo->setInsertPolicy(QComboBox::NoInsert);
    m_engineCombo->insertItem(1, tr("Gravity3DMassDifferential"));
    m_engineCombo->insertItem(2, tr("Gravity3DMassIntegral"));
    m_engineCombo->insertItem(3, tr("Gravity3DDifferential"));
    m_engineCombo->insertItem(4, tr("Gravity3DIntegral"));
    m_engineCombo->insertItem(5, tr("Gravity2DMassDifferential"));
    m_engineCombo->insertItem(6, tr("Gravity2DMassIntegral"));
    m_engineCombo->insertItem(7, tr("Gravity2DDifferential"));
    m_engineCombo->insertItem(8, tr("Gravity2DIntegral"));
    vLayout->addWidget(m_engineCombo);

    // Initial Conditions
    auto initialConditionLabel = new QLabel;
    initialConditionLabel->setText(tr("Initial Conditions:"));
    vLayout->addWidget(initialConditionLabel);

    auto initialConditionCombo = new QComboBox;
    initialConditionCombo->setFocusPolicy(Qt::NoFocus);
    initialConditionCombo->setInsertPolicy(QComboBox::NoInsert);
    initialConditionCombo->insertItem(1, tr("Random Cube"));
    initialConditionCombo->insertItem(2, tr("Sun and Earth"));
    initialConditionCombo->insertItem(3, tr("Earth and Sun"));
    vLayout->addWidget(initialConditionCombo);

    auto massLabel = new QLabel;
    massLabel->setText(tr("Mass (Avg.) (kg):"));
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

    auto newButton = new QPushButton;
    newButton->setFocusPolicy(Qt::NoFocus);
    newButton->setText(tr("OK"));
    vLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, &InitializerDialog::newButtonClicked);

    auto cancelButton = new QPushButton;
    cancelButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setText(tr("Cancel"));
    vLayout->addWidget(cancelButton);
    connect(cancelButton, &QPushButton::clicked, this, &InitializerDialog::close);

    setLayout(vLayout);
}

void InitializerDialog::newButtonClicked()
{
    //m_engineName = m_engineCombo->itemText(m_engineCombo->currentIndex());
    m_engineName = m_engineCombo->currentText();
    accept();
}

QString InitializerDialog::engineName()
{
    return m_engineName;
}
