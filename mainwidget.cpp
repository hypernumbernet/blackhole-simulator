#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , m_updateUi(new UpdateUi)
{
    this->setWindowTitle(tr("Blackhole Simulator 2.0 beta"));

    m_graphicWindows = new GraphicWindow(m_updateUi);
    auto container = QWidget::createWindowContainer(m_graphicWindows);
    container->setFocusPolicy(Qt::StrongFocus);
    container->setFocus();
    container->setMinimumSize(QSize(200, 100));
    QSize screenSize = m_graphicWindows->screen()->size();
    container->setMaximumSize(screenSize);

    m_hLayout = new QHBoxLayout(this);
    m_vLayout = new QVBoxLayout;
    m_vLayout->setAlignment(Qt::AlignTop);
    m_hLayout->addWidget(container, Qt::AlignLeft);
    m_hLayout->addLayout(m_vLayout);

    initUi();
}

void MainWidget::initUi()
{
    // FPS
    auto fpsLayout = new QHBoxLayout;
    auto fpsLabel = new QLabel(tr("FPS"));
    fpsLayout->addWidget(fpsLabel);
    m_fpsLCD = newCounterQLCDNumber(12);
    fpsLayout->addWidget(m_fpsLCD);
    m_vLayout->addLayout(fpsLayout);
    connect(m_updateUi, &UpdateUi::displayFps, this, &MainWidget::displayFPS);

    // Frames
    auto frameNumberLayout = new QHBoxLayout;
    auto frameNumberLabel = new QLabel(tr("Frames"));
    frameNumberLayout->addWidget(frameNumberLabel);
    m_frameNumberLCD = newCounterQLCDNumber(12);
    frameNumberLayout->addWidget(m_frameNumberLCD);
    m_vLayout->addLayout(frameNumberLayout);
    connect(m_updateUi, &UpdateUi::displayFrameNumber, this, &MainWidget::displayFrameNumber);

    // Time/Frame
    auto timePerFrameLayout = new QHBoxLayout;
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    timePerFrameLayout->addWidget(timePerFrameLabel);
    m_timePerFrameValue = newValueQLabel();
    timePerFrameLayout->addWidget(m_timePerFrameValue);
    m_vLayout->addLayout(timePerFrameLayout);
    connect(m_updateUi, &UpdateUi::displayTimePerFrame, this, &MainWidget::displayTimePerFrame);

    // Simulation Time
    auto simTimeLayout = new QHBoxLayout;
    auto simTimeLabel = new QLabel(tr("Time"));
    simTimeLayout->addWidget(simTimeLabel);
    m_simTimeValue = newValueQLabel();
    simTimeLayout->addWidget(m_simTimeValue);
    m_vLayout->addLayout(simTimeLayout);

    // Start Button
    m_startButton = new QPushButton;
    m_startButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(m_startButton);
    connect(m_startButton, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::startSim);
    connect(m_updateUi, &UpdateUi::updateStartButtonText, this, &MainWidget::updateStartButtonText);

    // Frame Advance
    m_frameAdvanceButton = new QPushButton(tr("Frame Advance"));
    m_frameAdvanceButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(m_frameAdvanceButton);
    connect(m_frameAdvanceButton, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::frameAdvance);

    // Reset
    auto resetBtn = new QPushButton(tr("Reset"));
    resetBtn->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(resetBtn);
    connect(resetBtn, &QPushButton::clicked, this, &MainWidget::resetInitial);

    // Circle strafing
    auto circleStrafingCB = new QCheckBox(tr("Circle strafing"));
    circleStrafingCB->setFocusPolicy(Qt::NoFocus);
    circleStrafingCB->setChecked(false);
    m_vLayout->addWidget(circleStrafingCB);
    connect(circleStrafingCB, &QCheckBox::stateChanged, m_graphicWindows, &GraphicWindow::circleStrafing);

    // Grid Lines
    auto gridLinesCB = new QCheckBox(tr("Grid Lines"));
    gridLinesCB->setChecked(true);
    gridLinesCB->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(gridLinesCB);
    connect(gridLinesCB, &QCheckBox::stateChanged, m_graphicWindows, &GraphicWindow::enableGridLines);

    // Line Type
    auto btnLineType = new QPushButton(tr("Line Type"));
    btnLineType->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(btnLineType);
    connect(btnLineType, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::changeLinePosition);

    // Model Scale
    auto scaleLabel = new QLabel(tr("Model Scale (m):"));
    m_vLayout->addWidget(scaleLabel);

    m_scaleValue = new QLineEdit;
    m_scaleValue->setAlignment(Qt::AlignRight);
    m_vLayout->addWidget(m_scaleValue);
    connect(m_updateUi, &UpdateUi::displayModelScale, this, &MainWidget::displayModelScale);
    connect(m_scaleValue, &QLineEdit::textChanged, m_graphicWindows, &GraphicWindow::setModelScale);

    m_scaleSlider = new QSlider;
    m_scaleSlider->setFocusPolicy(Qt::NoFocus);
    m_scaleSlider->setOrientation(Qt::Horizontal);
    m_scaleSlider->setMinimum(0);
    m_scaleSlider->setMaximum(UpdateUi::SCALE_SLIDER_CENTER * 2);
    m_scaleSlider->setSliderPosition(UpdateUi::SCALE_SLIDER_CENTER);
    m_vLayout->addWidget(m_scaleSlider);
    connect(m_scaleSlider, &QSlider::sliderMoved, m_graphicWindows, &GraphicWindow::setModelScaleInt);

    // Number of particles
    auto particleNumLabel = new QLabel(tr("Number of particles"));
    m_vLayout->addWidget(particleNumLabel);

    auto particleNumValue = newValueQLabel();
    m_vLayout->addWidget(particleNumValue);
    connect(m_updateUi, &UpdateUi::displayNumberOfParticles, particleNumValue, &QLabel::setText);

    // Simulation Engine
    auto engineLabel = new QLabel(tr("Simulation Engine"));
    m_vLayout->addWidget(engineLabel);

    m_engineValue = newValueQLabel();
    m_vLayout->addWidget(m_engineValue);
    connect(m_updateUi, &UpdateUi::displayEngineName, this, &MainWidget::displayEngineName);

    // Initial Condition Preset
    auto presetLabel = new QLabel(tr("Initial Conditions Preset"));
    m_vLayout->addWidget(presetLabel);

    m_presetValue = newValueQLabel();
    m_vLayout->addWidget(m_presetValue);
    connect(m_updateUi, &UpdateUi::displayPresetName, this, &MainWidget::displayPresetName);

    auto massLabel = new QLabel(tr("Mass (Avg.) (kg)"));
    m_vLayout->addWidget(massLabel);

    auto massQuantity = new QLineEdit("5.972e+24f");
    m_vLayout->addWidget(massQuantity);

    auto massRandomCheck = new QCheckBox(tr("Random Mass"));
    massRandomCheck->setChecked(true);
    massRandomCheck->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(massRandomCheck);

    auto massRangeLabel = new QLabel(tr("Mass Range (kg)"));
    m_vLayout->addWidget(massRangeLabel);

    auto massRangeValue = new QLineEdit("1.0e+10");
    m_vLayout->addWidget(massRangeValue);

    // New
    auto newButton = new QPushButton(tr("New..."));
    newButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, &MainWidget::showInitializerDialog);

    // Graph
    auto graphButton = new QPushButton(tr("Graph..."));
    graphButton->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(graphButton);
    //connect(newButton, &QPushButton::clicked, this, &MainWidget::showInitializerDialog);

    updateStartButtonText(false);
}

void MainWidget::displayFrameNumber(const int num)
{
    m_frameNumberLCD->display(num);
    int time = floor(m_timePerFrame * (float)num);
    int seconds = time % 60;
    int remain = time / 60;
    int minutes = remain % 60;
    remain /= 60;
    int hours = remain % 24;
    remain /= 24;
    QString t = QString(tr("Day %1 %2:%3:%4"))
            .arg(remain)
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    m_simTimeValue->setText(t);
}

void MainWidget::displayFPS(const int fps)
{
    m_fpsLCD->display(fps);
}

void MainWidget::updateStartButtonText(const bool setStop)
{
    if (setStop) {
        m_startButton->setText(tr("Stop"));
    } else {
        m_startButton->setText(tr("Start"));
    }
    m_frameAdvanceButton->setDisabled(setStop);
}

QLCDNumber* MainWidget::newCounterQLCDNumber(const int numDigits)
{
    auto lcd = new QLCDNumber(numDigits);
    lcd->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    return lcd;
}

QLabel* MainWidget::newValueQLabel()
{
    auto lbl = new QLabel();
    lbl->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    lbl->setAlignment(Qt::AlignRight);
    return lbl;
}

void MainWidget::displayModelScale(const float val)
{
    if (val <= 0.0f) {
        return;
    }
    bool state = m_scaleValue->blockSignals(true);
    m_scaleValue->setText(QString::number(1.0f / val));
    m_scaleValue->blockSignals(state);
}

void MainWidget::resetScaleSlider()
{
    bool state = m_scaleSlider->blockSignals(true);
    m_scaleSlider->setSliderPosition(UpdateUi::SCALE_SLIDER_CENTER);
    m_scaleSlider->blockSignals(state);
}

void MainWidget::displayTimePerFrame(const float time)
{
    m_timePerFrame = time;
    m_timePerFrameValue->setText(QString::number(time));
}

void MainWidget::showInitializerDialog()
{
    if (!m_initializerDialog) {
        m_initializerDialog = new InitializerDialog(m_updateUi, this);
        connect(m_initializerDialog, &InitializerDialog::accepted, this, &MainWidget::acceptInitializerDialog);
    }
    m_initializerDialog->setTimePerFrame(m_timePerFrame);
    m_initializerDialog->show();
    m_initializerDialog->raise();
    m_initializerDialog->activateWindow();
}

void MainWidget::acceptInitializerDialog()
{
    UpdateUi::SimCondition sim;
    sim.engine = m_initializerDialog->engineIndex();
    sim.preset = m_initializerDialog->presetIndex();
    sim.timePerFrame = m_initializerDialog->timePerFrame();
    reset(sim);
}

void MainWidget::displayEngineName(const QString& name)
{
    m_engineValue->setText(name);
}

void MainWidget::reset(const UpdateUi::SimCondition& sim)
{
    m_graphicWindows->reset(sim);
    resetScaleSlider();
    updateStartButtonText(false);
}

void MainWidget::displayPresetName(const QString& name)
{
    m_presetValue->setText(name);
}

void MainWidget::resetInitial()
{
    UpdateUi::SimCondition sim;
    if (m_initializerDialog) {
        sim.engine = m_initializerDialog->engineIndex();
        sim.preset = m_initializerDialog->presetIndex();
        sim.timePerFrame = m_initializerDialog->timePerFrame();
    }
    reset(sim);
}
