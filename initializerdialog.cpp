#include "initializerdialog.h"
#include "updateui.h"

Q_GLOBAL_STATIC(UpdateUi, config)

InitializerDialog::InitializerDialog(QWidget* parent)
    : QDialog(parent)
{
//    m_nBodyEngineList = {
//        //config->NBODY_ENGINE_MAP[1],
//        tr("Gravity3DMassDifferential"),
//        tr("Gravity3DMassIntegral"),
//        tr("Gravity3DDifferential"),
//        tr("Gravity3DIntegral"),
//        tr("Gravity2DMassDifferential"),
//        tr("Gravity2DMassIntegral"),
//        tr("Gravity2DDifferential"),
//        tr("Gravity2DIntegral"),
//    };

    auto vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine
    auto simTypeLabel = new QLabel;
    simTypeLabel->setText(tr("Simulation Engine:"));
    vLayout->addWidget(simTypeLabel);

    m_engineCombo = new QComboBox;
    m_engineCombo->setFocusPolicy(Qt::NoFocus);
    m_engineCombo->setInsertPolicy(QComboBox::NoInsert);
    //m_engineCombo->addItems(m_nBodyEngineList);
    m_engineCombo->addItems(config->NBODY_ENGINE_MAP->values());
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
    //m_engineName = m_engineCombo->currentText();
    m_engineIndex = m_engineCombo->currentIndex();
    accept();
}

//QString InitializerDialog::engineName()
//{
//    return m_engineName;
//}

int InitializerDialog::engineIndex()
{
    return m_engineIndex;
}
