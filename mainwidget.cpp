#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(tr("Blackhole Simulator 2.0 beta"));

    graphicWindows = new GraphicWindow;
    container = QWidget::createWindowContainer(graphicWindows);
    container->setFocusPolicy(Qt::StrongFocus);
    container->setFocus();
    container->setMinimumSize(QSize(200, 100));
    QSize screenSize = graphicWindows->screen()->size();
    container->setMaximumSize(screenSize);

    hLayout = new QHBoxLayout(this);
    vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, Qt::AlignLeft);
    hLayout->addLayout(vLayout);

    initUi();
}

void MainWidget::initUi()
{
    auto fpsLayout = new QHBoxLayout();
    auto fpsLabel = new QLabel;
    fpsLabel->setText("FPS");
    fpsLayout->addWidget(fpsLabel);
    fpsLCD = newCounterQLCDNumber(5);
    QObject::connect(graphicWindows, &GraphicWindow::fpsUpdate, this, &MainWidget::fpsUpdate);
    fpsLayout->addWidget(fpsLCD);
    vLayout->addLayout(fpsLayout);

    counterLcd = newCounterQLCDNumber(10);
    counterLcd->display(graphicWindows->frameNum);
    vLayout->addWidget(counterLcd);
    QObject::connect(graphicWindows, &GraphicWindow::counterUpdate, this, &MainWidget::counterUpdate);

    auto startBtn = new QPushButton(tr("Start"));
    startBtn->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(startBtn);
    QObject::connect(startBtn, &QPushButton::clicked, graphicWindows, &GraphicWindow::startSim);

    auto gridLinesCB = new QCheckBox(tr("Grid Lines"));
    gridLinesCB->setChecked(true);
    gridLinesCB->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(gridLinesCB);
    QObject::connect(gridLinesCB, &QCheckBox::stateChanged, graphicWindows, &GraphicWindow::enableGridLines);

    auto btnLineType = new QPushButton(tr("Line Type"));
    btnLineType->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(btnLineType);
    QObject::connect(btnLineType, &QPushButton::clicked, graphicWindows, &GraphicWindow::changeLinePosition);

    auto particleNumLabel = new QLabel;
    particleNumLabel->setText(tr("Number of particles:"));
    vLayout->addWidget(particleNumLabel);

    particleNumValue = newNumberQLabel();
    vLayout->addWidget(particleNumValue);
}

void MainWidget::counterUpdate()
{
    counterLcd->display(graphicWindows->frameNum);
}

void MainWidget::fpsUpdate(int fps)
{
    fpsLCD->display(fps);
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

void MainWidget::setNumberOfParticles(quint64 num)
{
    particleNumValue->setText(QString::number(num));
}
