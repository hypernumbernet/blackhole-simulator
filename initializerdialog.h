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

public slots:
    void newButtonClicked();

private:
    QComboBox* m_engineCombo;
    QComboBox* m_presetCombo;

    int m_engineIndex;
    int m_presetIndex;

    UpdateUi* const m_updateUi;
};
