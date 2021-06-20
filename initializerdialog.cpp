#include "initializerdialog.h"

InitializerDialog::InitializerDialog(UpdateUi* updateUi, QWidget* parent)
    : QDialog(parent)
    , m_updateUi(updateUi)
    , m_engineCombo(new QComboBox)
    , m_presetCombo(new QComboBox)
    , m_timePerFrameValue(new QLineEdit)
    , m_particleNumValue(new QLineEdit)
{
    auto vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine
    auto engineLabel = new QLabel(tr("Simulation Engine"));
    vLayout->addWidget(engineLabel);

    m_engineCombo->setFocusPolicy(Qt::NoFocus);
    m_engineCombo->setInsertPolicy(QComboBox::NoInsert);
    m_engineCombo->addItems(m_updateUi->NBODY_ENGINE_MAP->values());
    vLayout->addWidget(m_engineCombo);

    // Initial Conditions Preset
    auto presetLabel = new QLabel(tr("Initial Conditions Preset"));
    vLayout->addWidget(presetLabel);

    m_presetCombo->setFocusPolicy(Qt::NoFocus);
    m_presetCombo->setInsertPolicy(QComboBox::NoInsert);
    m_presetCombo->addItems(m_updateUi->INITIAL_CONDITION_MAP->values());
    vLayout->addWidget(m_presetCombo);

    // Time/Frame
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    vLayout->addWidget(timePerFrameLabel);

    m_timePerFrameValue->setAlignment(Qt::AlignRight);
    vLayout->addWidget(m_timePerFrameValue);

    // Number of particles
    auto particleNumLabel = new QLabel(tr("Number of particles"));
    vLayout->addWidget(particleNumLabel);

    m_particleNumValue->setAlignment(Qt::AlignRight);
    vLayout->addWidget(m_particleNumValue);

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
    vLayout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, &InitializerDialog::okButtonClicked);

    // Apply
    auto applyButton = new QPushButton(tr("Apply"));
    applyButton->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(applyButton);
    connect(applyButton, &QPushButton::clicked, this, &InitializerDialog::applyButtonClicked);

    // Cancel
    auto cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setDefault(true);
    vLayout->addWidget(cancelButton);
    connect(cancelButton, &QPushButton::clicked, this, &InitializerDialog::close);

    setLayout(vLayout);
}

bool InitializerDialog::validate()
{
    bool allOk = true;
    QPalette normalPal(palette());
    QPalette ngPal(palette());
    ngPal.setColor(QPalette::Base, RE_ENTER_COLOR);

    m_simCondition.engine = m_engineCombo->currentIndex();
    m_simCondition.preset = m_presetCombo->currentIndex();

    bool ok;
    auto val = m_timePerFrameValue->text().toFloat(&ok);
    if (ok) {
        m_timePerFrameValue->setPalette(normalPal);
        m_simCondition.timePerFrame = val;
    } else {
        allOk = false;
        m_timePerFrameValue->setPalette(ngPal);
    }

    auto num = m_particleNumValue->text().toInt(&ok);
    if (ok) {
        m_particleNumValue->setPalette(normalPal);
        m_simCondition.numberOfParticles = num;
    } else {
        allOk = false;
        m_particleNumValue->setPalette(ngPal);
    }
    return allOk;
}

void InitializerDialog::okButtonClicked()
{
    if (validate()) {
        accept();
    }
}

void InitializerDialog::applyButtonClicked()
{
    if (validate()) {
        emit m_updateUi->applyInitialConditions();
    }
}

void InitializerDialog::setTimePerFrame(const float time)
{
    m_timePerFrameValue->setText(QString::number(time));
}

void InitializerDialog::setNumberOfParticles(const int num)
{
    m_particleNumValue->setText(QString::number(num));
}

bhs::SimCondition& InitializerDialog::simCondition()
{
    return m_simCondition;
}
