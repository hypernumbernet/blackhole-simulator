#include "graphicwindow.h"

GraphicWindow::GraphicWindow()
    : frameNum(0)
    , walkSpeed(0.1f)
    , lookAroundSpeed(1.0f)
    , m_cam(QVector3D(-0.6f, -0.3f, -6.0f))
    , fpsPreFrame(0)
    , isSimulating(false)
    , numberOfParticle(2)
{
    m_cam.lookAtZero();
}

GraphicWindow::~GraphicWindow()
{
    simulateTimer.stop();
    uiTimer.stop();
    fpsTimer.stop();

    makeCurrent();
    delete world;
    delete particleModel;
    doneCurrent();
}

void GraphicWindow::initializeGL()
{
    startTime = QTime::currentTime();

    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glDepthFunc(GL_LESS);

    world = new WorldModels();
    world->initialize();

    particleModel = new Particles(this->height());
    if (!particleModel->initialize()) {
        return; // TODO error message
    }
    particleModel->setNumberOfParticles(numberOfParticle);

    paintGL();

    uiTimer.start(30, this);
    fpsTimer.start(1000, this);
    simulateTimer.start(1, this);
}

void GraphicWindow::initShaders()
{
}

void GraphicWindow::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 1.0, zFar = 100.0, fov = 30.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);

    particleModel->resize(h);
}

void GraphicWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto viewMatrix = m_cam.viewMatrix();

    world->paint(projection * viewMatrix);
    particleModel->paint(projection * viewMatrix);
}

void GraphicWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat())
        return;

    keyPressing.append(static_cast<Qt::Key>(ev->key()));
}

void GraphicWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat())
        return;

    keyPressing.removeAll(static_cast<Qt::Key>(ev->key()));
}

void GraphicWindow::mousePressEvent(QMouseEvent* ev)
{
    mouseLastPosition = ev->localPos();
    mousePressing = true;
    setCursor(Qt::BlankCursor);
    mousePressPosition = QPoint(mouseLastPosition.x(), mouseLastPosition.y());
    if (ev->buttons() == (Qt::MiddleButton)) {
        m_cam.lookAtZero(0.5f);
    }
}

void GraphicWindow::mouseMoveEvent(QMouseEvent* ev)
{
    if (mousePressing) {
        QPointF pos = ev->localPos();
        QPointF diff = pos - mouseLastPosition;
        if (ev->buttons() == Qt::LeftButton) {
            diff *= 0.15f;
            m_cam.yaw(diff.x());
            m_cam.pitch(diff.y());
        } else if (ev->buttons() == Qt::RightButton) {
            diff *= 0.03f;
            m_cam.strafe(-diff.x());
            m_cam.jump(diff.y());
        } else if (ev->buttons() == (Qt::LeftButton | Qt::RightButton)) {
            diff *= 0.1f;
            m_cam.roll(-diff.x());
        }
        mouseLastPosition = pos;
    }
}

void GraphicWindow::mouseReleaseEvent(QMouseEvent*)
{
    mousePressing = false;
    setCursor(Qt::ArrowCursor);
    QCursor::setPos(mapToGlobal(mousePressPosition));
}

void GraphicWindow::wheelEvent(QWheelEvent* ev)
{
    QPoint numDegrees = ev->angleDelta();
    if (!numDegrees.isNull()) {
        m_cam.walk(-numDegrees.y() * 0.01f);
    }
}

void GraphicWindow::timerEvent(QTimerEvent* ev)
{
    if (isSimulating && ev->timerId() == simulateTimer.timerId()) {
        particleModel->updateParticles();
        ++frameNum;
    } else if (ev->timerId() == uiTimer.timerId()) {
        if (keyPressing.indexOf(Qt::Key_W) >= 0) {
            m_cam.walk(-walkSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_S) >= 0) {
            m_cam.walk(walkSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_D) >= 0) {
            m_cam.strafe(-walkSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_A) >= 0) {
            m_cam.strafe(walkSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_Space) >= 0) {
            m_cam.jump(-walkSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_Control) >= 0) {
            m_cam.jump(walkSpeed);
        }

        if (keyPressing.indexOf(Qt::Key_Up) >= 0) {
            m_cam.pitch(-lookAroundSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_Down) >= 0) {
            m_cam.pitch(lookAroundSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_Left) >= 0) {
            m_cam.yaw(-lookAroundSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_Right) >= 0) {
            m_cam.yaw(lookAroundSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_E) >= 0) {
            m_cam.roll(-lookAroundSpeed);
        }
        if (keyPressing.indexOf(Qt::Key_Q) >= 0) {
            m_cam.roll(lookAroundSpeed);
        }

        if (keyPressing.indexOf(Qt::Key_Shift) >= 0) {
            m_cam.standXZ();
        }

        if (keyPressing.indexOf(Qt::Key_Tab) >= 0) {
            m_cam.lookAtZero();
        }
        emit counterUpdate();
        update();
    } else if (ev->timerId() == fpsTimer.timerId()) {
        emit fpsUpdate(frameNum - fpsPreFrame);
        fpsPreFrame = frameNum;
    }
}

void GraphicWindow::focusOutEvent(QFocusEvent*)
{
    keyPressing.clear();
    mousePressing = false;
}

void GraphicWindow::enableGridLines(bool enabled)
{
    world->enableGridLines(enabled);
}

void GraphicWindow::startSim()
{
    isSimulating = !isSimulating;
}

void GraphicWindow::changeLinePosition()
{
    world->changeLineType();
}
