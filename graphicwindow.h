#pragma once

#include "camera.h"
#include "worldmodels.h"
#include "particles.h"
#include "updateui.h"
#include "threadadmin.h"

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QKeyEvent>
#include <QTime>
#include <QVector>
#include <QBasicTimer>
#include <QCursor>
#include <QMutex>

class GraphicWindow : public QOpenGLWindow, private QOpenGLFunctions_4_5_Core
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
    void circleStrafing(bool);
    void reset(const bhs::SimCondition&);
    void setModelScale(const QString&);
    void setModelScaleInt(int);

protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
    void timerEvent(QTimerEvent*) override;
    void focusOutEvent(QFocusEvent*) override;

private:
    static constexpr QVector3D CAMERA_INI_POS= {-1.5f, -1.0f, -5.0f};

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    UpdateUi* const m_updateUi;
    ThreadAdmin  m_threadAdmin;
    WorldModels* const m_worldModels; // TODO not pointer
    Particles* const m_particleModels; // TODO not pointer

    float m_walkSpeed;
    float m_lookAroundSpeed;
    Camera m_camera;
    QVector<Qt::Key> m_keyPressing;
    QPointF m_mouseLastPosition;
    bool m_mousePressing = false;
    QPoint m_mousePressPosition;
    QMatrix4x4 m_projection;
    QTime m_startTime;
    QBasicTimer m_uiTimer;
    QBasicTimer m_fpsTimer;
    int m_fpsPreFrame;
    bool m_isCircleStrafing;
    float m_circleStrafingSpeed;
    QMutex m_guiMutex;
};
