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
    void enableGridLines(bool enabled);
    void changeLinePosition();
    void startSim();

protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
    void timerEvent(QTimerEvent*) override;
    void focusOutEvent(QFocusEvent*) override;

private:
    int frameNum;
    float walkSpeed;
    float lookAroundSpeed;

    Camera m_cam;

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    QVector<Qt::Key> keyPressing;

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    QPointF mouseLastPosition;
    bool mousePressing = false;
    QPoint mousePressPosition;

    QMatrix4x4 projection;
    QTime startTime;
    QBasicTimer uiTimer;
    QBasicTimer simulateTimer;

    QBasicTimer fpsTimer;
    int fpsPreFrame;

    WorldModels* world;
    Particles* particleModel;

    bool isSimulating;
    UpdateUi* m_updateUi;
};
