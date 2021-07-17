#pragma once

#include "updateui.h"

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QMessageBox>

class InitializerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitializerDialog(QWidget* parent);

    void setValues(const bhs::SimCondition&);
    const bhs::SimCondition& simCondition() const;

public slots:
    void okButtonClicked();
    void applyButtonClicked();

private:
    static constexpr QColor RE_ENTER_COLOR = {255, 200, 200};

    bool validate();

    QLineEdit m_timePerFrameEdit;
    QLineEdit m_particleNumEdit;
    QLineEdit m_massAvgEdit;
    QCheckBox m_massRandomCheckBox;
    QButtonGroup m_engineButtonGroup;
    QButtonGroup m_presetButtonGroup;
    QLineEdit m_scaleEdit;
    QButtonGroup m_precisionButtonGroup;
    QButtonGroup m_computeButtonGroup;
    QLineEdit m_speedEdit;

    bhs::SimCondition m_simCondition;
};
