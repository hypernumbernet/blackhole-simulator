#pragma once

#include "bhs.h"
#include "GraphicWindow.h"
#include <QDialog>
#include <QButtonGroup>

class GraphDialog : public QDialog
{
    Q_OBJECT
public:
    GraphDialog(QWidget*, GraphicWindow*);

    void saveCondition(int, bhs::Preset, bhs::LineType);
    int numberOfParticles();
    bhs::Preset preset();
    bhs::LineType lineType();

protected:
    void closeEvent(QCloseEvent*);

private:
    void drawLine();

    GraphicWindow* const m_graphicWindow;
    QButtonGroup m_graphButtonGroup;
    int m_numberOfParticles;
    bhs::Preset m_preset;
    bhs::LineType m_lineType;
};
