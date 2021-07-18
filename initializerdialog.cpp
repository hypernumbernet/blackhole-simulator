#include "initializerdialog.h"

InitializerDialog::InitializerDialog(QWidget* parent)
    : QDialog(parent)
    , m_normalPal(palette())
    , m_NGPal(palette())
{
    m_NGPal.setColor(QPalette::Base, RE_ENTER_COLOR);

    auto firstLayout = new QVBoxLayout;
    firstLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine

    auto engineGroup = new QGroupBox(tr("Simulation Engine"));
    auto engineVbox = new QVBoxLayout;
    engineGroup->setLayout(engineVbox);
    firstLayout->addWidget(engineGroup);

    for (const auto& e : UpdateUi::engine().toStdMap())
    {
        auto radio = new QRadioButton(e.second);
        m_engineButtonGroup.addButton(radio, static_cast<int>(e.first));
        engineVbox->addWidget(radio);
        if (e.first == bhs::Engine::G3D)
            radio->setChecked(true);
    }

    // Precision

    auto precisionGroup = new QGroupBox(tr("Precision"));
    auto precisionHbox = new QHBoxLayout;
    precisionGroup->setLayout(precisionHbox);
    firstLayout->addWidget(precisionGroup);

    for (const auto& e : UpdateUi::precision().toStdMap())
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

    for (const auto& e : UpdateUi::compute().toStdMap())
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

    for (const auto& e : UpdateUi::preset().toStdMap())
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

    // For Random Preset
    auto forRandomGroup = new QGroupBox(tr("For Random Preset"));
    secondLayout->addWidget(forRandomGroup);
    auto forRandomVbox = new QVBoxLayout;
    forRandomGroup->setLayout(forRandomVbox);

    // Number of particles
    auto particleNumLabel = new QLabel(tr("Number of particles"));
    forRandomVbox->addWidget(particleNumLabel);

    m_particleNumEdit.setAlignment(Qt::AlignRight);
    forRandomVbox->addWidget(&m_particleNumEdit);

    // Scale

    auto scaleLabel = new QLabel(tr("Scale (m)"));
    forRandomVbox->addWidget(scaleLabel);

    m_scaleEdit.setAlignment(Qt::AlignRight);
    forRandomVbox->addWidget(&m_scaleEdit);

    // Mass

    auto massLabel = new QLabel(tr("Mass (Avg.) (kg)"));
    forRandomVbox->addWidget(massLabel);

    m_massAvgEdit.setAlignment(Qt::AlignRight);
    forRandomVbox->addWidget(&m_massAvgEdit);

    m_massRandomCheckBox.setText(tr("Random Mass"));
    m_massRandomCheckBox.setChecked(true);
    forRandomVbox->addWidget(&m_massRandomCheckBox);

    // Speed

    auto speedLabel = new QLabel(tr("Initial Speed (Avg.) (m/s)"));
    forRandomVbox->addWidget(speedLabel);

    m_speedEdit.setAlignment(Qt::AlignRight);
    forRandomVbox->addWidget(&m_speedEdit);

    // Rotation

    auto rotationLabel = new QLabel(tr("Z-X Rotation (degree/s)"));
    forRandomVbox->addWidget(rotationLabel);

    m_rotationEdit.setAlignment(Qt::AlignRight);
    forRandomVbox->addWidget(&m_rotationEdit);

//    auto massRangeLabel = new QLabel(tr("Mass Range (%)"));
//    massVbox->addWidget(massRangeLabel);

//    auto massRangeValue = new QLineEdit("20");
//    massRangeValue->setEnabled(false);
//    massVbox->addWidget(massRangeValue);

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

    m_simCondition.engine = static_cast<bhs::Engine>(m_engineButtonGroup.checkedId());
    m_simCondition.preset = static_cast<bhs::Preset>(m_presetButtonGroup.checkedId());
    m_simCondition.massRandom = m_massRandomCheckBox.isChecked();
    m_simCondition.precision = static_cast<bhs::Precision>(m_precisionButtonGroup.checkedId());
    m_simCondition.compute = static_cast<bhs::Compute>(m_computeButtonGroup.checkedId());

    m_simCondition.timePerFrame = toDouble(m_timePerFrameEdit, allOk);
    m_simCondition.numberOfParticles = toInt(m_particleNumEdit, allOk);
    m_simCondition.massAvg = toDouble(m_massAvgEdit, allOk);
    m_simCondition.scale = toDouble(m_scaleEdit, allOk);
    m_simCondition.speed = toDouble(m_speedEdit, allOk);
    m_simCondition.rotation = toDouble(m_rotationEdit, allOk);

    if ((m_simCondition.engine == bhs::Engine::G4D3D && m_simCondition.compute == bhs::Compute::GPU) ||
        (m_simCondition.engine == bhs::Engine::G3D4DR1 && m_simCondition.compute == bhs::Compute::GPU))
    {
        QMessageBox::information(this, tr("Information"), tr("The GPU Engine is not implemented"));
        allOk = false;
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
    m_speedEdit.setText(QString::number(sim.speed));
    m_rotationEdit.setText(QString::number(sim.rotation));
}

double InitializerDialog::toDouble(QLineEdit& edit, bool& success)
{
    bool ok;
    auto speed = edit.text().toDouble(&ok);
    if (ok)
    {
        edit.setPalette(m_normalPal);
    } else {
        success = false;
        edit.setPalette(m_NGPal);
    }
    return speed;
}

int InitializerDialog::toInt(QLineEdit& edit, bool& success)
{
    bool ok;
    auto speed = edit.text().toInt(&ok);
    if (ok)
    {
        edit.setPalette(m_normalPal);
    } else {
        success = false;
        edit.setPalette(m_NGPal);
    }
    return speed;
}
