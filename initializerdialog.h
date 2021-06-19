#pragma once

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
    InitializerDialog(QWidget* parent = nullptr);

    //QString engineName();
    int engineIndex();

public slots:
    void newButtonClicked();

private:
    //QString m_engineName;
    QComboBox* m_engineCombo;
    int m_engineIndex;
    //QList<QString> m_nBodyEngineList;
};
