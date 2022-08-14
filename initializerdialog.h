#pragma once

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
#include <QFileDialog>
#include <QSettings>

#include "updateui.h"

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
    double toDouble(QLineEdit&, bool&);
    int toInt(QLineEdit&, bool&);
    void save();
    void load();
    void customCondition();

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

    bhs::SimCondition m_simCondition;
};
