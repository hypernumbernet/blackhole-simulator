#include "GraphDialog.h"
#include "UpdateUi.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QButtonGroup>

GraphDialog::GraphDialog(QWidget* parent, GraphicWindow* graphicWindow)
    : QDialog(parent)
    , m_graphicWindow(graphicWindow)
    , m_graphButtonGroup(this)
{
    auto firstLayout = new QVBoxLayout;
    firstLayout->setAlignment(Qt::AlignTop);
    setLayout(firstLayout);

    QListIterator<bhs::LineType> i(UpdateUi::graphList());
    while (i.hasNext())
    {
        auto p = i.next();
        QString s = UpdateUi::lineType().find(p).value();
        auto radio = new QRadioButton(s);
        m_graphButtonGroup.addButton(radio, static_cast<int>(p));
        firstLayout->addWidget(radio);
        connect(radio, &QRadioButton::clicked, this, &GraphDialog::drawLine);
    }
}

void GraphDialog::saveCondition(const int num, bhs::Preset preset, bhs::LineType lineType)
{
    m_numberOfParticles = num;
    m_preset = preset;
    m_lineType = lineType;
}

int GraphDialog::numberOfParticles()
{
    return m_numberOfParticles;
}

bhs::Preset GraphDialog::preset()
{
    return m_preset;
}

void GraphDialog::drawLine()
{
    int c = m_graphButtonGroup.checkedId();
    m_graphicWindow->setLineType(c);
}

bhs::LineType GraphDialog::lineType()
{
    return m_lineType;
}

void GraphDialog::closeEvent(QCloseEvent* ev)
{
    auto b = m_graphButtonGroup.button(m_graphButtonGroup.checkedId());
    if (b != nullptr)
    {
        m_graphButtonGroup.setExclusive(false);
        b->setChecked(false);
        m_graphButtonGroup.setExclusive(true);
    }
    QDialog::closeEvent(ev);
}
