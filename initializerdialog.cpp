#include "initializerdialog.h"

InitializerDialog::InitializerDialog(QWidget* parent)
    : QDialog(parent)
{
    auto firstLayout = new QVBoxLayout;
    firstLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine

    auto engineGroup = new QGroupBox(tr("Simulation Engine"));
    auto engineVbox = new QVBoxLayout;
    engineGroup->setLayout(engineVbox);
    firstLayout->addWidget(engineGroup);

    for (const auto& e : UpdateUi::it().ENGINE->toStdMap())
    {
        auto radio = new QRadioButton(e.second);
        m_engineButtonGroup.addButton(radio, static_cast<int>(e.first));
        engineVbox->addWidget(radio);
        if (e.first == bhs::Engine::G3DEuler)
            radio->setChecked(true);
    }

    // Precision

    auto precisionGroup = new QGroupBox(tr("Precision"));
    auto precisionHbox = new QHBoxLayout;
    precisionGroup->setLayout(precisionHbox);
    firstLayout->addWidget(precisionGroup);

    for (const auto& e : UpdateUi::it().PRECISION->toStdMap())
    {
        auto radio = new QRadioButton(e.second);
        m_precisionButtonGroup.addButton(radio, static_cast<int>(e.first));
        precisionHbox->addWidget(radio);
        if (e.first == bhs::Precision::Float)
            radio->setChecked(true);
    }

    // Compute Device

    auto computeGroup = new QGroupBox(tr("Compute Device"));
    auto computeHbox = new QHBoxLayout;
    computeGroup->setLayout(computeHbox);
    firstLayout->addWidget(computeGroup);

    for (const auto& e : UpdateUi::it().COMPUTE->toStdMap())
    {
        auto radio = new QRadioButton(e.second);
        m_computeButtonGroup.addButton(radio, static_cast<int>(e.first));
        computeHbox->addWidget(radio);
        if (e.first == bhs::Compute::GPU)
            radio->setChecked(true);
    }

    // Initial Conditions Preset

    auto presetGroup = new QGroupBox(tr("Initial Conditions Preset"));
    auto presetVbox = new QVBoxLayout;
    presetGroup->setLayout(presetVbox);
    firstLayout->addWidget(presetGroup);

    for (const auto& e : UpdateUi::it().PRESET->toStdMap())
    {
        auto radio = new QRadioButton(e.second);
        m_presetButtonGroup.addButton(radio, static_cast<int>(e.first));
        presetVbox->addWidget(radio);
        if (e.first == bhs::Preset::RandomBall)
            radio->setChecked(true);
    }

    //########################################################################
    auto secondLayout = new QVBoxLayout;
    secondLayout->setAlignment(Qt::AlignTop);

    // Time/Frame
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    secondLayout->addWidget(timePerFrameLabel);

    m_timePerFrameEdit.setAlignment(Qt::AlignRight);
    secondLayout->addWidget(&m_timePerFrameEdit);

    // Number of particles
    auto particleNumLabel = new QLabel(tr("Number of particles (hint)"));
    secondLayout->addWidget(particleNumLabel);

    m_particleNumEdit.setAlignment(Qt::AlignRight);
    secondLayout->addWidget(&m_particleNumEdit);

    // Mass

    auto massGroup = new QGroupBox(tr("Mass (for random preset)"));
    auto massVbox = new QVBoxLayout;

    auto massLabel = new QLabel(tr("Mass (Avg.) (kg)"));
    massVbox->addWidget(massLabel);

    m_massAvgEdit.setAlignment(Qt::AlignRight);
    massVbox->addWidget(&m_massAvgEdit);

    m_massRandomCheckBox.setText(tr("Random Mass"));
    m_massRandomCheckBox.setChecked(true);
    massVbox->addWidget(&m_massRandomCheckBox);

//    auto massRangeLabel = new QLabel(tr("Mass Range (%)"));
//    massVbox->addWidget(massRangeLabel);

//    auto massRangeValue = new QLineEdit("20");
//    massRangeValue->setEnabled(false);
//    massVbox->addWidget(massRangeValue);

    massGroup->setLayout(massVbox);
    secondLayout->addWidget(massGroup);

    // Scale

    auto scaleGroup = new QGroupBox(tr("Scale"));
    auto scaleVbox = new QVBoxLayout;

    auto scaleLabel = new QLabel(tr("Scale (m)"));
    scaleVbox->addWidget(scaleLabel);

    m_scaleEdit.setAlignment(Qt::AlignRight);
    scaleVbox->addWidget(&m_scaleEdit);

    scaleGroup->setLayout(scaleVbox);
    secondLayout->addWidget(scaleGroup);

    //########################################################################
    auto thirdLayout = new QHBoxLayout;
    thirdLayout->setAlignment(Qt::AlignRight);

    // OK
    auto okButton = new QPushButton(tr("OK"));
    okButton->setFocusPolicy(Qt::NoFocus);
    thirdLayout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, &InitializerDialog::okButtonClicked);

    // Apply
    auto applyButton = new QPushButton(tr("Apply"));
    applyButton->setFocusPolicy(Qt::NoFocus);
    thirdLayout->addWidget(applyButton);
    connect(applyButton, &QPushButton::clicked, this, &InitializerDialog::applyButtonClicked);

    // Cancel
    auto cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setDefault(true);
    thirdLayout->addWidget(cancelButton);
    connect(cancelButton, &QPushButton::clicked, this, &InitializerDialog::close);

    //########################################################################
    auto topLayout = new QVBoxLayout;
    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(firstLayout);
    mainLayout->addLayout(secondLayout);
    topLayout->addLayout(mainLayout);
    topLayout->addLayout(thirdLayout);
    setLayout(topLayout);
}

bool InitializerDialog::validate()
{
    bool allOk = true;
    QPalette normalPal(palette());
    QPalette NGPal(palette());
    NGPal.setColor(QPalette::Base, RE_ENTER_COLOR);

    m_simCondition.engine = static_cast<bhs::Engine>(m_engineButtonGroup.checkedId());
    m_simCondition.preset = static_cast<bhs::Preset>(m_presetButtonGroup.checkedId());
    m_simCondition.massRandom = m_massRandomCheckBox.isChecked();
    m_simCondition.precision = static_cast<bhs::Precision>(m_precisionButtonGroup.checkedId());
    m_simCondition.compute = static_cast<bhs::Compute>(m_computeButtonGroup.checkedId());

    bool ok;
    auto val = m_timePerFrameEdit.text().toDouble(&ok);
    if (ok)
    {
        m_timePerFrameEdit.setPalette(normalPal);
        m_simCondition.timePerFrame = val;
    } else {
        allOk = false;
        m_timePerFrameEdit.setPalette(NGPal);
    }

    auto num = m_particleNumEdit.text().toInt(&ok);
    if (ok)
    {
        m_particleNumEdit.setPalette(normalPal);
        m_simCondition.numberOfParticles = num;
    } else {
        allOk = false;
        m_particleNumEdit.setPalette(NGPal);
    }

    auto massAvg = m_massAvgEdit.text().toDouble(&ok);
    if (ok)
    {
        m_massAvgEdit.setPalette(normalPal);
        m_simCondition.massAvg = massAvg;
    } else {
        allOk = false;
        m_massAvgEdit.setPalette(NGPal);
    }

    auto scale = m_scaleEdit.text().toDouble(&ok);
    if (ok)
    {
        m_scaleEdit.setPalette(normalPal);
        m_simCondition.scale = scale;
    } else {
        allOk = false;
        m_scaleEdit.setPalette(NGPal);
    }

    return allOk;
}

void InitializerDialog::okButtonClicked()
{
    if (validate())
        accept();
}

void InitializerDialog::applyButtonClicked()
{
    if (validate())
        emit accepted();
}

const bhs::SimCondition& InitializerDialog::simCondition() const
{
    return m_simCondition;
}

void InitializerDialog::setValues(const bhs::SimCondition& sim)
{
    m_timePerFrameEdit.setText(QString::number(sim.timePerFrame));
    m_particleNumEdit.setText(QString::number(sim.numberOfParticles));
    m_massAvgEdit.setText(QString::number(sim.massAvg));
    m_massRandomCheckBox.setChecked(sim.massRandom);
    m_scaleEdit.setText(QString::number(sim.scale));
}
