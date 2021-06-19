#include "graphicwindow.h"

GraphicWindow::GraphicWindow(UpdateUi* updateUi)
    : m_frameNum(0)
    , m_walkSpeed(0.1f)
    , m_lookAroundSpeed(1.0f)
    , m_camera(CAMERA_INI_POS)
    , m_fpsPreFrame(0)
    , m_worldModels(new WorldModels)
    , m_particleModels(new Particles(updateUi))
    , m_isSimulating(false)
    , m_updateUi(updateUi)
    , m_isCircleStrafing(false)
    , m_circleStrafingSpeed(1.0f)
{
    // Lock when using the camera. If not locked, it will vibrate at the same time.
    QMutexLocker locker(&m_guiMutex);

    m_camera.lookAtZero(1.0f);
    m_camera.standXZ(false, 1.0f);
    m_camera.lookAtZero(1.0f);
}

GraphicWindow::~GraphicWindow()
{
    m_simulateTimer.stop();
    m_uiTimer.stop();
    m_fpsTimer.stop();

    makeCurrent();
    delete m_worldModels;
    delete m_particleModels;
    doneCurrent();
}

void GraphicWindow::initializeGL()
{
    m_startTime = QTime::currentTime();

    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glDepthFunc(GL_LESS);

    m_worldModels->initialize();

    if (!m_particleModels->initialize(this->height())) {
        return; // TODO error message
    }
    m_particleModels->selectNBodyEngine(-1);

    paintGL();

    m_uiTimer.start(30, this);
    m_fpsTimer.start(1000, this);
    m_simulateTimer.start(0, this);
}

void GraphicWindow::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 1.0, zFar = 100.0, fov = 30.0;
    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);

    m_particleModels->resize(h);
}

void GraphicWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto viewMatrix = m_camera.viewMatrix();

    m_worldModels->paint(m_projection * viewMatrix);
    m_particleModels->paint(m_projection * viewMatrix);
}

void GraphicWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat())
        return;

    if (ev->key() == Qt::Key_Escape) {

        QMutexLocker locker(&m_guiMutex);

        m_camera.reset(CAMERA_INI_POS * 10.0f);
        m_camera.lookAt(CAMERA_INI_POS, 1.0f);
        m_camera.standXZ(false, 1.0f);
        m_camera.lookAt(CAMERA_INI_POS, 1.0f);
    }

    m_keyPressing.append(static_cast<Qt::Key>(ev->key()));
}

void GraphicWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat())
        return;

    m_keyPressing.removeAll(static_cast<Qt::Key>(ev->key()));
}

void GraphicWindow::mousePressEvent(QMouseEvent* ev)
{
    m_mouseLastPosition = ev->localPos();
    m_mousePressing = true;
    setCursor(Qt::BlankCursor);
    m_mousePressPosition = QPoint(m_mouseLastPosition.x(), m_mouseLastPosition.y());
    if (ev->buttons() == (Qt::MiddleButton)) {

        QMutexLocker locker(&m_guiMutex);

        m_camera.lookAtZero(1.0f);
    }
}

void GraphicWindow::mouseMoveEvent(QMouseEvent* ev)
{
    if (m_mousePressing) {
        QPointF pos = ev->localPos();
        QPointF diff = pos - m_mouseLastPosition;

        QMutexLocker locker(&m_guiMutex);

        if (ev->buttons() == Qt::LeftButton) {
            diff *= 0.15f;
            m_camera.yaw(diff.x());
            m_camera.pitch(diff.y());
        } else if (ev->buttons() == Qt::RightButton) {
            diff *= 0.03f;
            m_camera.strafe(-diff.x());
            m_camera.jump(diff.y());
        } else if (ev->buttons() == (Qt::LeftButton | Qt::RightButton)) {
            diff *= 0.1f;
            m_camera.roll(-diff.x());
        }
        m_mouseLastPosition = pos;
    }
}

void GraphicWindow::mouseReleaseEvent(QMouseEvent*)
{
    if (m_mousePressing) {
        setCursor(Qt::ArrowCursor);
        QCursor::setPos(mapToGlobal(m_mousePressPosition));
        m_mousePressing = false;
    }
}

void GraphicWindow::wheelEvent(QWheelEvent* ev)
{
    QPoint numDegrees = ev->angleDelta();
    if (!numDegrees.isNull()) {

        QMutexLocker locker(&m_guiMutex);

        m_camera.walk(-numDegrees.y() * 0.01f);
    }
}

void GraphicWindow::timerEvent(QTimerEvent* ev)
{
    if (m_isSimulating && ev->timerId() == m_simulateTimer.timerId()) {
        m_particleModels->updateParticles();
        ++m_frameNum;
    } else if (ev->timerId() == m_uiTimer.timerId()) {

        QMutexLocker locker(&m_guiMutex);

        if (m_keyPressing.indexOf(Qt::Key_W) >= 0) {
            m_camera.walk(-m_walkSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_S) >= 0) {
            m_camera.walk(m_walkSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_D) >= 0) {
            m_camera.strafe(-m_walkSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_A) >= 0) {
            m_camera.strafe(m_walkSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_Space) >= 0) {
            m_camera.jump(-m_walkSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_Control) >= 0) {
            m_camera.jump(m_walkSpeed);
        }

        if (m_keyPressing.indexOf(Qt::Key_Up) >= 0) {
            m_camera.pitch(-m_lookAroundSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_Down) >= 0) {
            m_camera.pitch(m_lookAroundSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_Left) >= 0) {
            m_camera.yaw(-m_lookAroundSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_Right) >= 0) {
            m_camera.yaw(m_lookAroundSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_E) >= 0) {
            m_camera.roll(-m_lookAroundSpeed);
        }
        if (m_keyPressing.indexOf(Qt::Key_Q) >= 0) {
            m_camera.roll(m_lookAroundSpeed);
        }

        if (m_keyPressing.indexOf(Qt::Key_Shift) >= 0) {
            m_camera.standXZ();
        }
        if (m_keyPressing.indexOf(Qt::Key_Tab) >= 0) {
            m_camera.lookAtZero();
        }
        if (m_keyPressing.indexOf(Qt::Key_Escape) >= 0) {
            m_camera.setPosition(CAMERA_INI_POS, 0.1f);
        }
        if (m_keyPressing.indexOf(Qt::Key_Home) >= 0) {
            m_camera.setPosition(QVector3D(), 0.1f);
        }
        if (m_keyPressing.indexOf(Qt::Key_End) >= 0) {
            m_camera.setPosition(QVector3D(), -0.1f);
        }

        if (m_isCircleStrafing) {
            m_camera.circleStrafing(m_circleStrafingSpeed * m_walkSpeed);
        }

        emit m_updateUi->displayFrameNumber(m_frameNum);
        update();
    } else if (ev->timerId() == m_fpsTimer.timerId()) {
        emit m_updateUi->displayFps(m_frameNum - m_fpsPreFrame);
        m_fpsPreFrame = m_frameNum;
    }
}

void GraphicWindow::focusOutEvent(QFocusEvent*)
{
    m_keyPressing.clear();
    m_mousePressing = false;
}

void GraphicWindow::enableGridLines(const bool enabled)
{
    m_worldModels->enableGridLines(enabled);
}

void GraphicWindow::startSim()
{
    m_isSimulating = !m_isSimulating;
    emit m_updateUi->updateStartButtonText(m_isSimulating);
}

void GraphicWindow::changeLinePosition()
{
    m_worldModels->changeLineType();
}

void GraphicWindow::frameAdvance()
{
    if (!m_isSimulating) {
        m_particleModels->updateParticles();
        ++m_frameNum;
    }
}

void GraphicWindow::circleStrafing(const bool on)
{
    if (on)
        m_circleStrafingSpeed = -m_circleStrafingSpeed;
    m_isCircleStrafing = on;

    QMutexLocker locker(&m_guiMutex);

    m_camera.lookAtZero(1.0f);
}

void GraphicWindow::reset(const int engineIndex)
{
    m_isSimulating = false;
    m_frameNum = 0;
    m_fpsPreFrame = 0;
    m_particleModels->reset(engineIndex);
}

void GraphicWindow::setModelScale(const QString& text)
{
    bool ok;
    auto val = text.toFloat(&ok);
    if (ok && val > 0.0f)
    {
        m_particleModels->setModelScale(1.0f / val);
    }
}

void GraphicWindow::setModelScaleInt(int val)
{
    float r1 = (float)val / (float)UpdateUi::SCALE_SLIDER_CENTER;
    m_particleModels->setModelScaleRatio(pow(r1, 10.0f));
}
