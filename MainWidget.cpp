#include "MainWidget.h"
#include "bhs.h"
#include <QComboBox>
#include <QMessageBox>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , m_simCondition()
    , m_graphicWindows(m_simCondition)
    , m_hLayout(this)
    , m_frameNumberLCD(12)
    , m_fpsLCD(12)
{
    this->setWindowTitle(tr("Blackhole Simulator 2.0 beta"));

    auto container = QWidget::createWindowContainer(&m_graphicWindows);
    container->setFocusPolicy(Qt::StrongFocus);
    container->setFocus();
    container->setMinimumSize(QSize(200, 100));
    QSize screenSize = m_graphicWindows.screen()->size();
    container->setMaximumSize(screenSize);

    m_vLayout.setAlignment(Qt::AlignTop);
    m_hLayout.addWidget(container, Qt::AlignLeft);
    m_hLayout.addLayout(&m_vLayout);

    initUi();
}

void MainWidget::initUi()
{
    // FPS
    auto fpsLayout = new QHBoxLayout;
    auto fpsLabel = new QLabel(tr("FPS"));
    fpsLayout->addWidget(fpsLabel);
    UpdateUi::style(m_fpsLCD);
    fpsLayout->addWidget(&m_fpsLCD);
    m_vLayout.addLayout(fpsLayout);
    connect(&UpdateUi::it(), &UpdateUi::displayFps, this, &MainWidget::displayFPS);

    // Frames
    auto frameNumberLayout = new QHBoxLayout;
    auto frameNumberLabel = new QLabel(tr("Frames"));
    frameNumberLayout->addWidget(frameNumberLabel);
    UpdateUi::style(m_frameNumberLCD);
    frameNumberLayout->addWidget(&m_frameNumberLCD);
    m_vLayout.addLayout(frameNumberLayout);
    connect(&UpdateUi::it(), &UpdateUi::displayFrameNumber, this, &MainWidget::displayFrameNumber);

    // Simulation Time
    auto simTimeLayout = new QHBoxLayout;
    auto simTimeLabel = new QLabel(tr("Time (day)"));
    simTimeLayout->addWidget(simTimeLabel);
    UpdateUi::style(m_simTimeValue);
    simTimeLayout->addWidget(&m_simTimeValue);
    m_vLayout.addLayout(simTimeLayout);

    // Time/Frame
    auto timePerFrameLayout = new QHBoxLayout;
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    timePerFrameLayout->addWidget(timePerFrameLabel);
    UpdateUi::style(m_timePerFrameValue);
    timePerFrameLayout->addWidget(&m_timePerFrameValue);
    m_vLayout.addLayout(timePerFrameLayout);
    connect(&UpdateUi::it(), &UpdateUi::displayTimePerFrame, this, &MainWidget::displayTimePerFrame);

    // Start Button
    m_startButton.setFocusPolicy(Qt::NoFocus);
    m_vLayout.addWidget(&m_startButton);
    connect(&m_startButton, &QPushButton::clicked, &m_graphicWindows, &GraphicWindow::startSim);
    connect(&UpdateUi::it(), &UpdateUi::updateStartButtonText, this, &MainWidget::updateStartButtonText);

    // Frame Advance
    auto frameAdvanseHbox = new QHBoxLayout;

    m_frameAdvance1.setText(tr("||> 1"));
    m_frameAdvance1.setFocusPolicy(Qt::NoFocus);
    m_frameAdvance1.setMaximumSize(50, 100);
    connect(&m_frameAdvance1, &QPushButton::clicked, &m_graphicWindows, &GraphicWindow::frameAdvance1);
    frameAdvanseHbox->addWidget(&m_frameAdvance1);

    m_frameAdvance10.setText(tr("||> 10"));
    m_frameAdvance10.setFocusPolicy(Qt::NoFocus);
    m_frameAdvance10.setMaximumSize(50, 100);
    connect(&m_frameAdvance10, &QPushButton::clicked, &m_graphicWindows, &GraphicWindow::frameAdvance10);
    frameAdvanseHbox->addWidget(&m_frameAdvance10);

    m_frameAdvance100.setText(tr("||> 100"));
    m_frameAdvance100.setFocusPolicy(Qt::NoFocus);
    m_frameAdvance100.setMaximumSize(50, 100);
    connect(&m_frameAdvance100, &QPushButton::clicked, &m_graphicWindows, &GraphicWindow::frameAdvance100);
    frameAdvanseHbox->addWidget(&m_frameAdvance100);

    m_vLayout.addLayout(frameAdvanseHbox);

    // Reset
    auto resetBtn = new QPushButton(tr("Reset"));
    resetBtn->setFocusPolicy(Qt::NoFocus);
    m_vLayout.addWidget(resetBtn);
    connect(resetBtn, &QPushButton::clicked, this, &MainWidget::resetInitial);

    // Settings
    auto newButton = new QPushButton(tr("Settings..."));
    newButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout.addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, &MainWidget::showInitializerDialog);

    // Circle strafing
    auto circleStrafingCB = new QCheckBox(tr("Moving in a circle"));
    circleStrafingCB->setFocusPolicy(Qt::NoFocus);
    circleStrafingCB->setChecked(false);
    m_vLayout.addWidget(circleStrafingCB);
    connect(circleStrafingCB, &QCheckBox::stateChanged, &m_graphicWindows, &GraphicWindow::circleStrafing);

    // Grid Lines
    auto gridLinesCB = new QCheckBox(tr("Grid Lines"));
    gridLinesCB->setChecked(true);
    gridLinesCB->setFocusPolicy(Qt::NoFocus);
    m_vLayout.addWidget(gridLinesCB);
    connect(gridLinesCB, &QCheckBox::stateChanged, &m_graphicWindows, &GraphicWindow::enableGridLines);

    // Line Type
    auto lineTypeCombo = new QComboBox();
    lineTypeCombo->setFocusPolicy(Qt::NoFocus);
    lineTypeCombo->setInsertPolicy(QComboBox::NoInsert);
    QListIterator<bhs::LineType> i(UpdateUi::gridList());
    while (i.hasNext())
    {
        auto p = i.next();
        QString s = UpdateUi::lineType().find(p).value();
        QVariant v = static_cast<int>(p);
        lineTypeCombo->addItem(s, v);
    }
    m_vLayout.addWidget(lineTypeCombo);
    connect(lineTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), &m_graphicWindows, &GraphicWindow::setLineType);

    // Scale
    auto scaleLabel = new QLabel(tr("Scale (m):"));
    m_vLayout.addWidget(scaleLabel);

    m_scaleValue.setAlignment(Qt::AlignRight);
    m_vLayout.addWidget(&m_scaleValue);
    connect(&UpdateUi::it(), &UpdateUi::displayModelScale, this, &MainWidget::displayModelScale);
    connect(&m_scaleValue, &QLineEdit::textChanged, &m_graphicWindows, &GraphicWindow::setModelScale);

    m_scaleSlider.setFocusPolicy(Qt::NoFocus);
    m_scaleSlider.setOrientation(Qt::Horizontal);
    m_scaleSlider.setMinimum(0);
    m_scaleSlider.setMaximum(UpdateUi::SCALE_SLIDER_CENTER * 2);
    m_scaleSlider.setSliderPosition(UpdateUi::SCALE_SLIDER_CENTER);
    m_vLayout.addWidget(&m_scaleSlider);
    connect(&m_scaleSlider, &QSlider::sliderMoved, &m_graphicWindows, &GraphicWindow::setModelScaleInt);

    // Simulation Engine
    auto engineLabel = new QLabel(tr("Simulation Engine"));
    m_vLayout.addWidget(engineLabel);
    UpdateUi::style(m_engineValue);
    m_vLayout.addWidget(&m_engineValue);
    connect(&UpdateUi::it(), &UpdateUi::displayEngineName, this, &MainWidget::displayEngineName);

    // Initial Conditions
    auto presetLabel = new QLabel(tr("Initial Conditions"));
    m_vLayout.addWidget(presetLabel);
    UpdateUi::style(m_presetValue);
    m_vLayout.addWidget(&m_presetValue);
    connect(&UpdateUi::it(), &UpdateUi::displayPresetName, this, &MainWidget::displayPresetName);

    // Number of particles
    auto particleNumLayout = new QHBoxLayout;
    auto particleNumLabel = new QLabel(tr("Number of particles"));
    particleNumLayout->addWidget(particleNumLabel);
    UpdateUi::style(m_particleNumValue);
    UpdateUi::width(m_particleNumValue);
    particleNumLayout->addWidget(&m_particleNumValue);
    m_vLayout.addLayout(particleNumLayout);
    connect(&UpdateUi::it(), &UpdateUi::displayNumberOfParticles, this, &MainWidget::displayNumberOfParticles);

    // Compute Device
    auto computeLabel = new QLabel(tr("Compute"));
    UpdateUi::style(m_computeValue);
    UpdateUi::width(m_computeValue);
    auto computeLayout = new QHBoxLayout;
    computeLayout->addWidget(computeLabel);
    computeLayout->addWidget(&m_computeValue);
    m_vLayout.addLayout(computeLayout);
    connect(&UpdateUi::it(), &UpdateUi::displayCompute, this, &MainWidget::displayCompute);

    // Precision
    auto precisionLayout = new QHBoxLayout;
    auto precisionLabel = new QLabel(tr("Precision"));
    precisionLayout->addWidget(precisionLabel);
    UpdateUi::style(m_precisionValue);
    UpdateUi::width(m_precisionValue);
    precisionLayout->addWidget(&m_precisionValue);
    m_vLayout.addLayout(precisionLayout);
    connect(&UpdateUi::it(), &UpdateUi::displayPrecision, this, &MainWidget::displayPrecision);

    // Monitoring
    auto monitorButton = new QPushButton(tr("Monitoring..."));
    monitorButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout.addWidget(monitorButton);
    connect(monitorButton, &QPushButton::clicked, this, &MainWidget::showMonitoringDialog);

    // Graph
    auto graphButton = new QPushButton(tr("Graph..."));
    graphButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout.addWidget(graphButton);
    connect(graphButton, &QPushButton::clicked, this, &MainWidget::showGraphDialog);

    updateStartButtonText(false);
}

void MainWidget::displayFrameNumber(const int num)
{
    m_frameNumberLCD.display(num);
    quint64 time = floor(m_simCondition.timePerFrame * (float)num);
    quint64 seconds = time % 60;
    quint64 remain = time / 60;
    quint64 minutes = remain % 60;
    remain /= 60;
    quint64 hours = remain % 24;
    remain /= 24;
    QString t = QString(tr("%1 %2:%3:%4"))
            .arg(remain)
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    m_simTimeValue.setText(t);
}

void MainWidget::displayFPS(const int fps)
{
    m_fpsLCD.display(fps);
}

void MainWidget::updateStartButtonText(const bool setStop)
{
    if (setStop)
        m_startButton.setText(tr("Stop"));
    else
        m_startButton.setText(tr("Start"));

    m_frameAdvance1.setDisabled(setStop);
    m_frameAdvance10.setDisabled(setStop);
    m_frameAdvance100.setDisabled(setStop);
}

void MainWidget::displayModelScale(const double val)
{
    if (val <= 0.0)
        return;

    bool state = m_scaleValue.blockSignals(true);
    m_scaleValue.setText(QString::number(1.0 / val));
    m_scaleValue.blockSignals(state);
}

void MainWidget::resetScaleSlider()
{
    bool state = m_scaleSlider.blockSignals(true);
    m_scaleSlider.setSliderPosition(UpdateUi::SCALE_SLIDER_CENTER);
    m_scaleSlider.blockSignals(state);
}

void MainWidget::displayTimePerFrame(const double time)
{
    m_simCondition.timePerFrame = time;
    m_timePerFrameValue.setText(QString::number(time));
}

void MainWidget::showInitializerDialog()
{
    if (!m_initializerDialog)
    {
        m_initializerDialog = new InitializerDialog(this);
        connect(m_initializerDialog, &InitializerDialog::accepted, this, &MainWidget::acceptInitializerDialog);
    }
    m_initializerDialog->setValues(m_simCondition);

    m_initializerDialog->show();
    m_initializerDialog->raise();
    m_initializerDialog->activateWindow();
}

void MainWidget::acceptInitializerDialog()
{
    m_simCondition = m_initializerDialog->simCondition();
    reset();
}

void MainWidget::displayEngineName(const bhs::Engine engine)
{
    m_engineValue.setText(UpdateUi::engineLabel().value(engine));
}

void MainWidget::reset()
{
    m_graphicWindows.resetWaitForDone(m_simCondition);
    resetScaleSlider();
    updateStartButtonText(false);
}

void MainWidget::displayPresetName(const bhs::Preset preset, const QString name)
{
    if (preset == bhs::Preset::Custom)
    {
        m_presetValue.setText(name);
    } else {
        m_presetValue.setText(UpdateUi::preset().value(preset));
    }
}

void MainWidget::resetInitial()
{
    reset();
}

void MainWidget::displayNumberOfParticles(const int num)
{
    m_particleNumValue.setText(QString::number(num));
}

void MainWidget::displayPrecision(const bhs::Precision precision)
{
    QString s = UpdateUi::precision().value(precision);
    m_precisionValue.setText(s);
}

void MainWidget::showMonitoringDialog()
{
    if (!m_monitoringDialog)
    {
        m_monitoringDialog = new MonitoringDialog(this, &m_graphicWindows);
    }
    m_monitoringDialog->show();
    m_monitoringDialog->raise();
    m_monitoringDialog->activateWindow();
    m_monitoringDialog->update();
}

void MainWidget::displayCompute(const bhs::Compute c)
{
    QString s = UpdateUi::compute().value(c);
    m_computeValue.setText(s);
}

void MainWidget::showGraphDialog()
{
    if (m_graphicWindows.frameNumber() > 0)
    {
        auto rep = QMessageBox::question(this, tr("Confirm"), tr("Reset the simulation. Is it OK?"));
        if (rep != QMessageBox::Yes)
            return;
    }

    if ( ! m_graphDialog)
    {
        m_graphDialog = new GraphDialog(this, &m_graphicWindows);
        connect(m_graphDialog, &GraphDialog::finished, this, &MainWidget::finishedGraphDialog);
    }

    m_graphDialog->saveCondition(m_simCondition.numberOfParticles, m_simCondition.preset, bhs::LineType::XZMeshes);
    m_simCondition.numberOfParticles = 0;
    m_simCondition.preset = bhs::Preset::RandomCube;
    reset();

    m_graphDialog->show();
    m_graphDialog->raise();
    m_graphDialog->activateWindow();
}

void MainWidget::finishedGraphDialog(int)
{
    m_simCondition.numberOfParticles = m_graphDialog->numberOfParticles();
    m_simCondition.preset = m_graphDialog->preset();
    reset();
    m_graphicWindows.setLineType(static_cast<int>(m_graphDialog->lineType()));
}
