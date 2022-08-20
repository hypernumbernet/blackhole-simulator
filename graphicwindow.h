#pragma once

#include "global.h"
#include "camera.h"
#include "lineshaders.h"
#include "particleshaders.h"
#include "threadadmin.h"
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QKeyEvent>

class GraphicWindow : public QOpenGLWindow, private QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    explicit GraphicWindow(const bhs::SimCondition&);
    ~GraphicWindow();

public slots:
    void enableGridLines(bool);
    void setLineType(int);
    void startSim();
    void frameAdvance1();
    void frameAdvance10();
    void frameAdvance100();
    void circleStrafing(bool);
    void resetWaitForDone(const bhs::SimCondition&);
    void resetParticles();
    void setModelScale(const QString&);
    void setModelScaleInt(int);
    QString particleData();
    int frameNumber();

protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
    void timerEvent(QTimerEvent*) override;
    void focusOutEvent(QFocusEvent*) override;

private:
    static inline Vector3<double> CAMERA_INI_POS = {-1.5, -1, -5};

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    LineShaders* const m_lineShaders;
    ParticleShaders* const m_particleShaders;
    ComputeShaders* const m_computeShaders;
    ThreadAdmin m_threadAdmin;
    double m_walkSpeed;
    double m_lookAroundSpeed;
    bool m_mousePressing;
    Camera m_camera;
    int m_fpsPreFrame;
    bool m_isCircleStrafing;
    double m_circleStrafingSpeed;
    const bhs::SimCondition* m_simCondition;

    QVector<Qt::Key> m_keyPressing;
    QPointF m_mouseLastPosition;
    QPoint m_mousePressPosition;
    QMatrix4x4 m_projection;
    QBasicTimer m_uiTimer;
    QBasicTimer m_fpsTimer;
};
