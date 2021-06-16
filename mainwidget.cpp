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
    auto fpsLayout = new QHBoxLayout();
    auto fpsLabel = new QLabel;
    fpsLabel->setText("FPS");
    fpsLayout->addWidget(fpsLabel);
    m_fpsLCD = newCounterQLCDNumber(5);
    QObject::connect(m_updateUi, &UpdateUi::showFps, this, &MainWidget::fpsUpdate);
    fpsLayout->addWidget(m_fpsLCD);
    m_vLayout->addLayout(fpsLayout);

    m_counterLcd = newCounterQLCDNumber(10);
    m_vLayout->addWidget(m_counterLcd);
    QObject::connect(m_updateUi, &UpdateUi::showFrameNumber, this, &MainWidget::counterUpdate);

    m_startBtn = new QPushButton;
    m_startBtn->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(m_startBtn);
    QObject::connect(m_startBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::startSim);
    QObject::connect(m_updateUi, &UpdateUi::setStartButtonTest, this, &MainWidget::startButtonText);

    m_frameAdvanceBtn = new QPushButton;
    m_frameAdvanceBtn->setFocusPolicy(Qt::NoFocus);
    m_frameAdvanceBtn->setText(tr("Frame Advance"));
    m_vLayout->addWidget(m_frameAdvanceBtn);
    QObject::connect(m_frameAdvanceBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::frameAdvance);

    auto resetBtn = new QPushButton;
    resetBtn->setFocusPolicy(Qt::NoFocus);
    resetBtn->setText(tr("Reset"));
    m_vLayout->addWidget(resetBtn);
    QObject::connect(resetBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::reset);

    auto circleStrafingCB = new QCheckBox(tr("Circle strafing"));
    circleStrafingCB->setFocusPolicy(Qt::NoFocus);
    circleStrafingCB->setChecked(false);
    m_vLayout->addWidget(circleStrafingCB);
    QObject::connect(circleStrafingCB, &QCheckBox::stateChanged,
                     m_graphicWindows, &GraphicWindow::circleStrafing);

    auto gridLinesCB = new QCheckBox(tr("Grid Lines"));
    gridLinesCB->setChecked(true);
    gridLinesCB->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(gridLinesCB);
    QObject::connect(gridLinesCB, &QCheckBox::stateChanged, m_graphicWindows, &GraphicWindow::enableGridLines);

    auto btnLineType = new QPushButton(tr("Line Type"));
    btnLineType->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(btnLineType);
    QObject::connect(btnLineType, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::changeLinePosition);

    auto particleNumLabel = new QLabel;
    particleNumLabel->setText(tr("Number of particles:"));
    m_vLayout->addWidget(particleNumLabel);

    auto particleNumValue = newNumberQLabel();
    m_vLayout->addWidget(particleNumValue);
    QObject::connect(m_updateUi, &UpdateUi::showNumberOfParticles,
                     particleNumValue, &QLabel::setText);

    auto scaleLabel = new QLabel;
    scaleLabel->setText(tr("Scale (m):"));
    m_vLayout->addWidget(scaleLabel);

    auto scaleValue = newNumberQLabel();
    m_vLayout->addWidget(scaleValue);
    QObject::connect(m_updateUi, &UpdateUi::showModelScale,
                     scaleValue, &QLabel::setText);

    auto timePerFrameLabel = new QLabel;
    timePerFrameLabel->setText(tr("Time/Frame (s):"));
    m_vLayout->addWidget(timePerFrameLabel);

    auto timePerFrameValue = newNumberQLabel();
    m_vLayout->addWidget(timePerFrameValue);
    QObject::connect(m_updateUi, &UpdateUi::showTimePerFrame,
                     timePerFrameValue, &QLabel::setText);

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

    auto newBtn = new QPushButton;
    newBtn->setFocusPolicy(Qt::NoFocus);
    newBtn->setText(tr("New"));
    m_vLayout->addWidget(newBtn);

    startButtonText(false);
}

void MainWidget::counterUpdate(int num)
{
    m_counterLcd->display(num);
}

void MainWidget::fpsUpdate(int fps)
{
    m_fpsLCD->display(fps);
}

void MainWidget::startButtonText(const bool setStop)
{
    if (setStop) {
        m_startBtn->setText(tr("Stop"));
    } else {
        m_startBtn->setText(tr("Start"));
    }
    m_frameAdvanceBtn->setDisabled(setStop);
}

QLCDNumber* MainWidget::newCounterQLCDNumber(int numDigits)
{
    auto myLCD = new QLCDNumber(numDigits);
    myLCD->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    myLCD->setSegmentStyle(QLCDNumber::Flat);
    return myLCD;
}

QLabel* MainWidget::newNumberQLabel()
{
    auto lbl = new QLabel();
    lbl->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    lbl->setAlignment(Qt::AlignRight);
    return lbl;
}
