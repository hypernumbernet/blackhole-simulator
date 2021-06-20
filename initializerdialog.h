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

    void setTimePerFrame(float);
    void setNumberOfParticles(int);
    bhs::SimCondition& simCondition();

public slots:
    void okButtonClicked();
    void applyButtonClicked();

private:
    static constexpr QColor RE_ENTER_COLOR = QColor(255, 200, 200);

    bool validate();

    UpdateUi* const m_updateUi;

    QComboBox* const m_engineCombo;
    QComboBox* const m_presetCombo;
    QLineEdit* const m_timePerFrameValue;
    QLineEdit* const m_particleNumValue;

    bhs::SimCondition m_simCondition;
};
