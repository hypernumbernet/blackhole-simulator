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

    auto startBtn = new QPushButton(tr("Start"));
    startBtn->setFocusPolicy(Qt::NoFocus);
    m_vLayout->addWidget(startBtn);
    QObject::connect(startBtn, &QPushButton::clicked, m_graphicWindows, &GraphicWindow::startSim);

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
}

void MainWidget::counterUpdate(int num)
{
    m_counterLcd->display(num);
}

void MainWidget::fpsUpdate(int fps)
{
    m_fpsLCD->display(fps);
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
