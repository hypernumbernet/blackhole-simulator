#include "initializerdialog.h"

InitializerDialog::InitializerDialog(UpdateUi* updateUi, QWidget* parent)
    : QDialog(parent)
    , m_updateUi(updateUi)
{
    auto vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine

    auto engine0Radio = new QRadioButton(m_updateUi->ENGINE->value(0));
    auto engine1Radio = new QRadioButton(m_updateUi->ENGINE->value(1));
    auto engine2Radio = new QRadioButton(m_updateUi->ENGINE->value(2));
    m_engineButtonGroup.addButton(engine0Radio, 0);
    m_engineButtonGroup.addButton(engine1Radio, 1);
    m_engineButtonGroup.addButton(engine2Radio, 2);
    engine0Radio->setChecked(true);

    auto engineGroup = new QGroupBox(tr("Simulation Engine"));
    auto vbox = new QVBoxLayout;
    vbox->addWidget(engine0Radio);
    vbox->addWidget(engine1Radio);
    vbox->addWidget(engine2Radio);
    engineGroup->setLayout(vbox);
    vLayout->addWidget(engineGroup);

    // Initial Conditions Preset

    auto presetLabel = new QLabel(tr("Initial Conditions Preset"));
    vLayout->addWidget(presetLabel);

    m_presetCombo.setFocusPolicy(Qt::NoFocus);
    m_presetCombo.setInsertPolicy(QComboBox::NoInsert);
    m_presetCombo.addItems(m_updateUi->PRESET->values());
    vLayout->addWidget(&m_presetCombo);


    // Time/Frame
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    vLayout->addWidget(timePerFrameLabel);

    m_timePerFrameValue.setAlignment(Qt::AlignRight);
    vLayout->addWidget(&m_timePerFrameValue);

    // Number of particles
    auto particleNumLabel = new QLabel(tr("Number of particles (hint)"));
    vLayout->addWidget(particleNumLabel);

    m_particleNumValue.setAlignment(Qt::AlignRight);
    vLayout->addWidget(&m_particleNumValue);

    // Mass
    auto massLabel = new QLabel(tr("Mass (Avg.) (kg)"));
    vLayout->addWidget(massLabel);

    m_massAvgValue.setAlignment(Qt::AlignRight);
    vLayout->addWidget(&m_massAvgValue);

    m_massRandomCheck.setText(tr("Random Mass"));
    m_massRandomCheck.setChecked(true);
    vLayout->addWidget(&m_massRandomCheck);

    auto massRangeLabel = new QLabel(tr("Mass Range (%)"));
    vLayout->addWidget(massRangeLabel);

    auto massRangeValue = new QLineEdit("20");
    massRangeValue->setEnabled(false);
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
    QPalette NGPal(palette());
    NGPal.setColor(QPalette::Base, RE_ENTER_COLOR);

    m_simCondition.engine = m_engineButtonGroup.checkedId();
    m_simCondition.preset = static_cast<bhs::Preset>(m_presetCombo.currentIndex());

    bool ok;
    auto val = m_timePerFrameValue.text().toFloat(&ok);
    if (ok) {
        m_timePerFrameValue.setPalette(normalPal);
        m_simCondition.timePerFrame = val;
    } else {
        allOk = false;
        m_timePerFrameValue.setPalette(NGPal);
    }

    auto num = m_particleNumValue.text().toInt(&ok);
    if (ok) {
        m_particleNumValue.setPalette(normalPal);
        m_simCondition.numberOfParticles = num;
    } else {
        allOk = false;
        m_particleNumValue.setPalette(NGPal);
    }

    auto massAvg = m_massAvgValue.text().toFloat(&ok);
    if (ok) {
        m_massAvgValue.setPalette(normalPal);
        m_simCondition.massAvg = massAvg;
    } else {
        allOk = false;
        m_massAvgValue.setPalette(NGPal);
    }

    m_simCondition.massRandom = m_massRandomCheck.isChecked();

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
        emit accepted();
    }
}

bhs::SimCondition& InitializerDialog::simCondition()
{
    return m_simCondition;
}

void InitializerDialog::setValues(const bhs::SimCondition& sim)
{
    m_timePerFrameValue.setText(QString::number(sim.timePerFrame));
    m_particleNumValue.setText(QString::number(sim.numberOfParticles));
    m_massAvgValue.setText(QString::number(sim.massAvg));
    m_massRandomCheck.setChecked(sim.massRandom);
}
