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
    counterLcd = new QLCDNumber(10);
    counterLcd->setSegmentStyle(QLCDNumber::Flat);
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

    auto particleNumValue = new QLabel;
    particleNumValue->setFrameStyle(QFrame::Box | QFrame::Raised);
    particleNumValue->setAlignment(Qt::AlignRight);
    //particleNumValue->setText(QString::number(numberOfParticles));
    vLayout->addWidget(particleNumValue);

//    auto frameNumValue = new QLabel;
//    frameNumValue->setFrameStyle(QFrame::Box | QFrame::Raised);
//    frameNumValue->setAlignment(Qt::AlignRight);
//    frameNumValue->setText(QString::number(graphicWindows->frameNum));
//    vLayout->addWidget(frameNumValue);
}

void MainWidget::counterUpdate()
{
    counterLcd->display(graphicWindows->frameNum);
}
