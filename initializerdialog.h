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

    void setValues(const bhs::SimCondition&);
    bhs::SimCondition& simCondition();

public slots:
    void okButtonClicked();
    void applyButtonClicked();

private:
    static constexpr QColor RE_ENTER_COLOR = QColor(255, 200, 200);

    bool validate();

    UpdateUi* const m_updateUi;

    QComboBox m_engineCombo;
    QComboBox m_presetCombo;
    QLineEdit m_timePerFrameValue;
    QLineEdit m_particleNumValue;
    QLineEdit m_massAvgValue;
    QCheckBox m_massRandomCheck;

    bhs::SimCondition m_simCondition;
};
