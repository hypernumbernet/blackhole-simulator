#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , particleNumber(1000)
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
    particleNumValue->setText(QString::number(particleNumber));
    vLayout->addWidget(particleNumValue);

    auto button = new QPushButton(tr("001"));
    button->setFocusPolicy(Qt::NoFocus);
    vLayout->addWidget(button);
    QObject::connect(button, &QPushButton::clicked, graphicWindows, &GraphicWindow::btn001);
}
