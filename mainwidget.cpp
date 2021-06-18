#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , m_updateUi(new UpdateUi(this))
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
    m_fpsLcd = newCounterQLCDNumber(12);
    fpsLayout->addWidget(m_fpsLcd);
    m_vLayout->addLayout(fpsLayout);
    connect(m_updateUi, &UpdateUi::displayFps, this, &MainWidget::displayFps);

    // Frames
    auto frameNumberLayout = new QHBoxLayout;
    auto frameNumberLabel = new QLabel(tr("Frames"));
    frameNumberLayout->addWidget(frameNumberLabel);
    m_frameNumberLcd = newCounterQLCDNumber(12);
    frameNumberLayout->addWidget(m_frameNumberLcd);
    m_vLayout->addLayout(frameNumberLayout);
    connect(m_updateUi, &UpdateUi::displayFrameNumber, this, &MainWidget::displayFrameNumber);

    // Time/Frame
    auto timePerFrameLayout = new QHBoxLayout;
    auto timePerFrameLabel = new QLabel(tr("Time/Frame (s)"));
    timePerFrameLayout->addWidget(timePerFrameLabel);
    m_timePerFrameValue = newNumberQLabel();
    timePerFrameLayout->addWidget(m_timePerFrameValue);
    m_vLayout->addLayout(timePerFrameLayout);
    connect(m_updateUi, &UpdateUi::displayTimePerFrame, this, &MainWidget::displayTimePerFrame);

    // Simulation Time
    auto simTimeLayout = new QHBoxLayout;
    auto simTimeLabel = new QLabel(tr("Time"));
    simTimeLayout->addWidget(simTimeLabel);
    m_simTimeValue = newNumberQLabel();
    simTimeLayout->addWidget(m_simTimeValue);
    m_vLayout->addLayout(simTimeLayout);
    //connect(m_updateUi, &UpdateUi::displaySimulationTime, this, &MainWidget::displaySimulationTime);

    // Start Button
    m_startBtn = new QPushButton;
    m_startBtn->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(m_startBtn);
    connect(m_startBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::startSim);
    connect(m_updateUi, &UpdateUi::updateStartButtonText, this, &MainWidget::updateStartButtonText);

    // Frame Advance
    m_frameAdvanceBtn = new QPushButton;
    m_frameAdvanceBtn->setFocusPolicy(Qt::NoFocus);
    m_frameAdvanceBtn->setText(tr("Frame Advance"));
    m_vLayout->addWidget(m_frameAdvanceBtn);
    connect(m_frameAdvanceBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::frameAdvance);

    // Reset
    auto resetBtn = new QPushButton;
    resetBtn->setFocusPolicy(Qt::NoFocus);
    resetBtn->setText(tr("Reset"));
    m_vLayout->addWidget(resetBtn);
    connect(resetBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::reset);
    connect(resetBtn, &QPushButton::clicked, this, &MainWidget::resetScaleSlider);

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
    auto scaleLabel = new QLabel;
    scaleLabel->setText(tr("Model Scale (m):"));
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
    auto particleNumLabel = new QLabel;
    particleNumLabel->setText(tr("Number of particles:"));
    m_vLayout->addWidget(particleNumLabel);

    auto particleNumValue = newNumberQLabel();
    m_vLayout->addWidget(particleNumValue);
    connect(m_updateUi, &UpdateUi::displayNumberOfParticles, particleNumValue, &QLabel::setText);

    // Simulation Engine
    auto simTypeLabel = new QLabel;
    simTypeLabel->setText(tr("Simulation Engine:"));
    m_vLayout->addWidget(simTypeLabel);

    auto simTypeCombo = new QComboBox;
    simTypeCombo->setFocusPolicy(Qt::NoFocus);
    simTypeCombo->setInsertPolicy(QComboBox::NoInsert);
    simTypeCombo->insertItem(1, tr("Gravity3DMassDifferential"));
    simTypeCombo->insertItem(2, tr("Gravity3DMassIntegral"));
    simTypeCombo->insertItem(3, tr("Gravity3DDifferential"));
    simTypeCombo->insertItem(4, tr("Gravity3DIntegral"));
    simTypeCombo->insertItem(5, tr("Gravity2DMassDifferential"));
    simTypeCombo->insertItem(6, tr("Gravity2DMassIntegral"));
    simTypeCombo->insertItem(7, tr("Gravity2DDifferential"));
    simTypeCombo->insertItem(8, tr("Gravity2DIntegral"));
    m_vLayout->addWidget(simTypeCombo);

    // Initial Conditions
    auto initialConditionLabel = new QLabel;
    initialConditionLabel->setText(tr("Initial Conditions:"));
    m_vLayout->addWidget(initialConditionLabel);

    auto initialConditionCombo = new QComboBox;
    initialConditionCombo->setFocusPolicy(Qt::NoFocus);
    initialConditionCombo->setInsertPolicy(QComboBox::NoInsert);
    initialConditionCombo->insertItem(1, tr("Random Cube"));
    initialConditionCombo->insertItem(2, tr("Sun and Earth"));
    initialConditionCombo->insertItem(3, tr("Earth and Sun"));
    m_vLayout->addWidget(initialConditionCombo);

    auto massLabel = new QLabel;
    massLabel->setText(tr("Mass (Avg.) (kg):"));
    m_vLayout->addWidget(massLabel);

    auto massQuantity = new QLineEdit("5.972e+24f");
    m_vLayout->addWidget(massQuantity);

    auto massRandomCheck = new QCheckBox(tr("Random Mass"));
    massRandomCheck->setChecked(true);
    massRandomCheck->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(massRandomCheck);

    auto massRangeLabel = new QLabel;
    massRangeLabel->setText(tr("Mass Range (±kg)"));
    m_vLayout->addWidget(massRangeLabel);

    auto massRangeValue = new QLineEdit("1.0e+10");
    m_vLayout->addWidget(massRangeValue);

    // New
    auto newBtn = new QPushButton;
    newBtn->setFocusPolicy(Qt::NoFocus);
    newBtn->setText(tr("New"));
    m_vLayout->addWidget(newBtn);
    connect(newBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::startSim);

    updateStartButtonText(false);
}

void MainWidget::displayFrameNumber(const int num)
{
    m_frameNumberLcd->display(num);
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
//    QString t;
//    t.asprintf("%d D %2d:%2d:%2d", days, hours, minutes, seconds);
    m_simTimeValue->setText(t);
}

void MainWidget::displayFps(const int fps)
{
    m_fpsLcd->display(fps);
}

void MainWidget::updateStartButtonText(const bool setStop)
{
    if (setStop) {
        m_startBtn->setText(tr("Stop"));
    } else {
        m_startBtn->setText(tr("Start"));
    }
    m_frameAdvanceBtn->setDisabled(setStop);
}

QLCDNumber* MainWidget::newCounterQLCDNumber(const int numDigits)
{
    auto lcd = new QLCDNumber(numDigits);
    lcd->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    return lcd;
}

QLabel* MainWidget::newNumberQLabel()
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
