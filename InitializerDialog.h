#pragma once

#include "bhs.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

class InitializerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitializerDialog(QWidget*);

    void setValues(const bhs::SimCondition&);
    const bhs::SimCondition& simCondition() const;

public slots:
    void okButtonClicked();
    void applyButtonClicked();

private:
    static constexpr QColor RE_ENTER_COLOR = {255, 200, 200};
    static const inline QString INI_NAME = "Blackhole-Simulator-2";

    bool validate();
    double toDouble(QLineEdit&, bool&);
    int toInt(QLineEdit&, bool&);
    void save();
    void load();
    void customCondition();
    void onCpuClicked();
    void onGpuClicked();

    const QPalette m_normalPal;
    QPalette m_NGPal;

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
    QLineEdit m_rotationEdit;
    QLabel m_presetCustomName;
    QRadioButton* m_floatRadio;
    QRadioButton* m_doubleRadio;

    bhs::SimCondition m_sim;
};
