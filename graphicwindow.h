#pragma once

#include "camera.h"
#include "worldmodels.h"
#include "particles.h"
#include "updateui.h"

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>
#include <QTime>
#include <QVector>
#include <QBasicTimer>
#include <QCursor>

class GraphicWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GraphicWindow(UpdateUi*);
    ~GraphicWindow();

public slots:
    void enableGridLines(bool);
    void changeLinePosition();
    void startSim();
    void frameAdvance();

protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
    void timerEvent(QTimerEvent*) override;
    void focusOutEvent(QFocusEvent*) override;

private:
    int m_frameNum;
    float m_walkSpeed;
    float m_lookAroundSpeed;

    Camera m_camera;

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    QVector<Qt::Key> m_keyPressing;

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    QPointF m_mouseLastPosition;
    bool m_mousePressing = false;
    QPoint m_mousePressPosition;

    QMatrix4x4 m_projection;
    QTime m_startTime;
    QBasicTimer m_uiTimer;
    QBasicTimer m_simulateTimer;

    QBasicTimer m_fpsTimer;
    int m_fpsPreFrame;

    WorldModels* const m_worldModels;
    Particles* const m_particleModels;

    bool m_isSimulating;
    UpdateUi* const m_updateUi;
};
