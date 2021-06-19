#pragma once

#include "updateui.h"

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class InitializerDialog : public QDialog
{
    Q_OBJECT

public:
    InitializerDialog(UpdateUi*, QWidget* parent = nullptr);

    int engineIndex();
    int presetIndex();
    void setTimePerFrame(float);
    float timePerFrame();

public slots:
    void okButtonClicked();

private:
    QComboBox* m_engineCombo;
    QComboBox* m_presetCombo;
    QLineEdit* m_timePerFrameValue;

    // These values are used when you press the reset button.
    int m_engineIndex;
    int m_presetIndex;
    float m_timePerFrame;

    UpdateUi* const m_updateUi;
};
