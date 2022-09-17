#include "InitializerDialog.h"
#include "UpdateUi.h"
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QVBoxLayout>

InitializerDialog::InitializerDialog(QWidget* parent)
    : QDialog(parent)
    , m_normalPal(palette())
    , m_NGPal(palette())
    , m_floatRadio(nullptr)
    , m_doubleRadio(nullptr)
{
    m_NGPal.setColor(QPalette::Base, RE_ENTER_COLOR);
    auto firstLayout = new QVBoxLayout;
    firstLayout->setAlignment(Qt::AlignTop);

    // Simulation Engine
    auto engineGroup = new QGroupBox(tr("Simulation Engine"));
    auto engineVbox = new QVBoxLayout;
    engineGroup->setLayout(engineVbox);
    firstLayout->addWidget(engineGroup);
    QListIterator<bhs::Engine> i(UpdateUi::engineUse());
    while (i.hasNext())
    {
        auto p = i.next();
        QString s = UpdateUi::engineLabel().find(p).value();
        auto radio = new QRadioButton(s);
        m_engineButtonGroup.addButton(radio, static_cast<int>(p));
        engineVbox->addWidget(radio);
        if (p == bhs::Engine::G3D)
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
        {
            radio->setChecked(true);
            connect(radio, &QRadioButton::clicked, this, &InitializerDialog::onGpuClicked);
        }
        else
            connect(radio, &QRadioButton::clicked, this, &InitializerDialog::onCpuClicked);
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
        {
            radio->setChecked(true);
            m_floatRadio = radio;
        }
        else
            m_doubleRadio = radio;
    }

    // Initial Conditions
    auto presetGroup = new QGroupBox(tr("Initial Conditions"));
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
    UpdateUi::style(m_presetCustomName);
    m_presetCustomName.setAlignment(Qt::AlignLeft);
    presetVbox->addWidget(&m_presetCustomName);

//    auto viewCustomButton = new QPushButton(tr("Custom Data..."));
//    presetVbox->addWidget(viewCustomButton);
//    connect(viewCustomButton, &QPushButton::clicked, this, &InitializerDialog::customCondition);

    //########################################################################
    auto secondLayout = new QVBoxLayout;
    secondLayout->setAlignment(Qt::AlignTop);

    // Time/Frame
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    secondLayout->addWidget(timePerFrameLabel);
    m_timePerFrameEdit.setAlignment(Qt::AlignRight);
    secondLayout->addWidget(&m_timePerFrameEdit);

    // For Random Conditions
    auto forRandomGroup = new QGroupBox(tr("For Random Conditions"));
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

    auto saveButton = new QPushButton(tr("Save..."));
    secondLayout->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, &InitializerDialog::save);
    auto loadButton = new QPushButton(tr("Load..."));
    secondLayout->addWidget(loadButton);
    connect(loadButton, &QPushButton::clicked, this, &InitializerDialog::load);

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
    m_sim.engine = static_cast<bhs::Engine>(m_engineButtonGroup.checkedId());
    m_sim.precision = static_cast<bhs::Precision>(m_precisionButtonGroup.checkedId());
    m_sim.compute = static_cast<bhs::Compute>(m_computeButtonGroup.checkedId());
    m_sim.preset = static_cast<bhs::Preset>(m_presetButtonGroup.checkedId());
    m_sim.timePerFrame = toDouble(m_timePerFrameEdit, allOk);
    m_sim.numberOfParticles = toInt(m_particleNumEdit, allOk);
    m_sim.scale = toDouble(m_scaleEdit, allOk);
    m_sim.massAvg = toDouble(m_massAvgEdit, allOk);
    m_sim.massRandom = m_massRandomCheckBox.isChecked();
    m_sim.speed = toDouble(m_speedEdit, allOk);
    m_sim.rotation = toDouble(m_rotationEdit, allOk);
    if (m_sim.compute == bhs::Compute::GPU &&
            (m_sim.engine == bhs::Engine::G4D3D
             || m_sim.engine == bhs::Engine::G3D4DR1
             || m_sim.engine == bhs::Engine::GravityCollision
             || m_sim.engine == bhs::Engine::Relativity2
             || m_sim.engine == bhs::Engine::Relativity3
             ))
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
    return m_sim;
}

void InitializerDialog::setValues(const bhs::SimCondition& sim)
{
    QAbstractButton* engine = m_engineButtonGroup.button(static_cast<int>(sim.engine));
    if (engine != nullptr)
        engine->setChecked(true);
    QAbstractButton* precision = m_precisionButtonGroup.button(static_cast<int>(sim.precision));
    if (precision != nullptr)
        precision->setChecked(true);
    QAbstractButton* compute = m_computeButtonGroup.button(static_cast<int>(sim.compute));
    if (compute != nullptr)
        compute->setChecked(true);
    QAbstractButton* preset = m_presetButtonGroup.button(static_cast<int>(sim.preset));
    if (preset != nullptr)
        preset->setChecked(true);
    m_timePerFrameEdit.setText(QString::number(sim.timePerFrame));
    m_particleNumEdit.setText(QString::number(sim.numberOfParticles));
    m_scaleEdit.setText(QString::number(sim.scale));
    m_massAvgEdit.setText(QString::number(sim.massAvg));
    m_massRandomCheckBox.setChecked(sim.massRandom);
    m_speedEdit.setText(QString::number(sim.speed));
    m_rotationEdit.setText(QString::number(sim.rotation));
    m_presetCustomName.setText(m_sim.custom.name);
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

void InitializerDialog::save()
{
    if ( ! validate())
    {
        return;
    }
    QFileDialog saveDlg(this, tr("Save Settings"), "settings/", tr("Ini Files (*.ini)"));
    saveDlg.setAcceptMode(QFileDialog::AcceptSave);
    QStringList fileNames;
    if (saveDlg.exec())
    {
        fileNames = saveDlg.selectedFiles();
    }
    if (fileNames.size() == 0)
    {
        return;
    }
    QString fileName = fileNames[0];
    if ( ! fileName.endsWith(".ini"))
    {
        fileName += ".ini";
    }
    QSettings stg(fileName, QSettings::IniFormat);
    stg.setValue("Name", INI_NAME);
    stg.setValue("SettingsVersion", 1);
    stg.setValue("Description", "Blackhole-Simulator Settings");

    stg.beginGroup("SimCondition");
    stg.setValue("Engine", static_cast<int>(m_sim.engine));
    stg.setValue("Precision", static_cast<int>(m_sim.precision));
    stg.setValue("Compute", static_cast<int>(m_sim.compute));
    stg.setValue("Preset", static_cast<int>(m_sim.preset));
    stg.setValue("TimePerFrame", m_sim.timePerFrame);
    stg.setValue("NumberOfParticles", m_sim.numberOfParticles);
    stg.setValue("Scale", m_sim.scale);
    stg.setValue("MassAvg", m_sim.massAvg);
    stg.setValue("MassRandom", m_sim.massRandom);
    stg.setValue("Speed", m_sim.speed);
    stg.setValue("Rotation", m_sim.rotation);
    stg.endGroup();

    stg.beginGroup("Custom");
    stg.setValue("Name", m_sim.custom.name);
    stg.setValue("Scale", m_sim.custom.scale);
    stg.endGroup();

    stg.beginWriteArray("Particles");
    int i = 0;
    for (bhs::Particle& e : m_sim.custom.particles)
    {
        stg.setArrayIndex(i);
        stg.setValue("Name", e.name);
        stg.setValue("M", e.mass);
        stg.setValue("CX", e.coordinate.x());
        stg.setValue("CY", e.coordinate.y());
        stg.setValue("CZ", e.coordinate.z());
        stg.setValue("VX", e.velocity.x());
        stg.setValue("VY", e.velocity.y());
        stg.setValue("VZ", e.velocity.z());
        ++i;
    }
    stg.endArray();

    QMessageBox::information(this, tr("Information"), tr("Saved a setting file."));
}

void InitializerDialog::load()
{
    QFileDialog dlg(this, tr("Load Settings"), "settings/", tr("Ini Files (*.ini)"));
    QStringList fileNames;
    if (dlg.exec())
    {
        fileNames = dlg.selectedFiles();
    }
    if (fileNames.size() == 0)
    {
        return;
    }
    QString fileName = fileNames[0];
    QSettings stg(fileName, QSettings::IniFormat);

    if (stg.value("Name", "").toString() != INI_NAME)
    {
        QMessageBox::warning(this, tr("Warning"), tr("This file format cannot be read."));
        return;
    }

    int settingsVersion = stg.value("SettingsVersion", 0).toInt();
    if (settingsVersion == 1)
    {
        bhs::SimCondition sim;
        stg.beginGroup("SimCondition");
        sim.engine = static_cast<bhs::Engine>(stg.value("Engine", 0).toInt());
        sim.precision = static_cast<bhs::Precision>(stg.value("Precision", 0).toInt());
        sim.compute = static_cast<bhs::Compute>(stg.value("Compute", 0).toInt());
        sim.preset = static_cast<bhs::Preset>(stg.value("Preset", 0).toInt());
        sim.timePerFrame = stg.value("TimePerFrame", sim.timePerFrame).toDouble();
        sim.numberOfParticles = stg.value("NumberOfParticles", sim.numberOfParticles).toInt();
        sim.scale = stg.value("Scale", sim.scale).toDouble();
        sim.massAvg = stg.value("MassAvg", sim.massAvg).toDouble();
        sim.massRandom = stg.value("MassRandom", sim.massRandom).toBool();
        sim.speed = stg.value("Speed", sim.speed).toDouble();
        sim.rotation = stg.value("Rotation", sim.rotation).toDouble();
        stg.endGroup();

        stg.beginGroup("Custom");
        QFileInfo fileInfo(fileName);
        m_sim.custom.name = fileInfo.fileName();
        m_sim.custom.scale = stg.value("Scale", sim.custom.scale).toDouble();
        stg.endGroup();

        m_sim.custom.particles.clear();
        int size = stg.beginReadArray("Particles");
        for (int i = 0; i < size; ++i)
        {
            bhs::Particle p;
            stg.setArrayIndex(i);
            p.name = stg.value("Name").toString();
            p.mass = stg.value("M").toDouble();
            p.coordinate.setX(stg.value("CX").toDouble());
            p.coordinate.setY(stg.value("CY").toDouble());
            p.coordinate.setZ(stg.value("CZ").toDouble());
            p.velocity.setX(stg.value("VX").toDouble());
            p.velocity.setY(stg.value("VY").toDouble());
            p.velocity.setZ(stg.value("VZ").toDouble());
            m_sim.custom.particles.append(p);
        }
        stg.endArray();

        setValues(sim);
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("This file format cannot be read."));
    }
}

void InitializerDialog::customCondition()
{

}

void InitializerDialog::onCpuClicked()
{
    if (m_floatRadio == nullptr || m_doubleRadio == nullptr)
        return;
    m_doubleRadio->click();
    m_floatRadio->setEnabled(false);
}

void InitializerDialog::onGpuClicked()
{
    if (m_floatRadio == nullptr || m_doubleRadio == nullptr)
        return;
    m_floatRadio->setEnabled(true);
    m_floatRadio->click();
}
