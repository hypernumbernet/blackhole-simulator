#pragma once

#include "camera.h"
#include "worldmodels.h"
#include "particles.h"
#include "updateui.h"

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

class GraphicWindow : public QOpenGLWindow, protected QOpenGLFunctions_4_5_Core
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
    void reset();
    void setModelScale(const QString&);
    void setModelScaleInt(int);

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
    static constexpr QVector3D CAMERA_INI_POS= {-1.5f, -1.0f, -5.0f};

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

    bool m_isCircleStrafing;
    float m_circleStrafingSpeed;

    QMutex m_guiMutex;
};
