#include "graphicwindow.h"

GraphicWindow::GraphicWindow(const bhs::SimCondition& simCondition)
    : m_lineShaders(new LineShaders)
    , m_particleShaders(new ParticleShaders(&m_threadAdmin))
    , m_computeShaders(new ComputeShaders)
    , m_threadAdmin(this, m_computeShaders)
    , m_walkSpeed(0.1f)
    , m_lookAroundSpeed(1.0f)
    , m_camera(CAMERA_INI_POS)
    , m_fpsPreFrame(0)
    , m_isCircleStrafing(false)
    , m_circleStrafingSpeed(1.0f)
    , m_simCondition(&simCondition)
{
    m_camera.lookAtZero(1.0f);
    m_camera.standXZ(false, 1.0f);
    m_camera.lookAtZero(1.0f);

    connect(&UpdateUi::it(), &UpdateUi::frameAdvance, &m_threadAdmin, &ThreadAdmin::frameAdvance);
    connect(&UpdateUi::it(), &UpdateUi::resultReady, &m_threadAdmin, &ThreadAdmin::handleResults);
    connect(&UpdateUi::it(), &UpdateUi::resetParticles, this, &GraphicWindow::resetParticles);

    m_threadAdmin.start();
}

GraphicWindow::~GraphicWindow()
{
    m_uiTimer.stop();
    m_fpsTimer.stop();

    m_threadAdmin.quit();
    m_threadAdmin.wait();

    makeCurrent();
    delete m_computeShaders;
    delete m_lineShaders;
    delete m_particleShaders;
    doneCurrent();
}

void GraphicWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glDepthFunc(GL_LESS);

    m_lineShaders->initialize();

    if (!m_particleShaders->initialize(this->height()))
        return; // TODO error message

    bhs::SimCondition sim;
    m_particleShaders->setNBodyEngine(sim);

    m_computeShaders->initialize();

    m_threadAdmin.setComputeDevice(sim.compute);

    m_uiTimer.start(30, this);
    m_fpsTimer.start(1000, this);
}

void GraphicWindow::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 1.0, zFar = 100.0, fov = 30.0;
    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);

    m_particleShaders->resize(h);
}

void GraphicWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto viewMatrix = m_camera.viewMatrix();

    m_lineShaders->paint(m_projection * viewMatrix);
    m_particleShaders->paint(m_projection * viewMatrix);
}

void GraphicWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat())
        return;

    if (ev->key() == Qt::Key_Escape)
    {
        m_camera.reset(CAMERA_INI_POS * 10.0f);
        m_camera.lookAt(CAMERA_INI_POS, 1.0f);
        m_camera.standXZ(false, 1.0f);
        m_camera.lookAt(CAMERA_INI_POS, 1.0f);
    }

    m_keyPressing.append(static_cast<Qt::Key>(ev->key()));
}

void GraphicWindow::keyReleaseEvent(QKeyEvent* ev)
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
    if (ev->buttons() == (Qt::MiddleButton))
        m_camera.lookAtZero(1.0f);
}

void GraphicWindow::mouseMoveEvent(QMouseEvent* ev)
{
    if (m_mousePressing)
    {
        QPointF pos = ev->localPos();
        QPointF diff = pos - m_mouseLastPosition;
        if (ev->buttons() == Qt::LeftButton)
        {
            diff *= 0.15f;
            m_camera.yaw(diff.x());
            m_camera.pitch(diff.y());
        }
        else if (ev->buttons() == Qt::RightButton)
        {
            diff *= 0.03f;
            m_camera.strafe(-diff.x());
            m_camera.jump(diff.y());
        }
        else if (ev->buttons() == (Qt::LeftButton | Qt::RightButton))
        {
            diff *= 0.1f;
            m_camera.roll(-diff.x());
        }
        m_mouseLastPosition = pos;
    }
}

void GraphicWindow::mouseReleaseEvent(QMouseEvent*)
{
    if (m_mousePressing)
    {
        setCursor(Qt::ArrowCursor);
        QCursor::setPos(mapToGlobal(m_mousePressPosition));
        m_mousePressing = false;
    }
}

void GraphicWindow::wheelEvent(QWheelEvent* ev)
{
    QPoint numDegrees = ev->angleDelta();
    if (!numDegrees.isNull())
        m_camera.walk(-numDegrees.y() * 0.01f);
}

void GraphicWindow::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == m_uiTimer.timerId()) {

        if (m_keyPressing.indexOf(Qt::Key_W) >= 0)
            m_camera.walk(-m_walkSpeed);

        if (m_keyPressing.indexOf(Qt::Key_S) >= 0)
            m_camera.walk(m_walkSpeed);

        if (m_keyPressing.indexOf(Qt::Key_D) >= 0)
            m_camera.strafe(-m_walkSpeed);

        if (m_keyPressing.indexOf(Qt::Key_A) >= 0)
            m_camera.strafe(m_walkSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Space) >= 0)
            m_camera.jump(-m_walkSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Control) >= 0)
            m_camera.jump(m_walkSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Up) >= 0)
            m_camera.pitch(-m_lookAroundSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Down) >= 0)
            m_camera.pitch(m_lookAroundSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Left) >= 0)
            m_camera.yaw(-m_lookAroundSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Right) >= 0)
            m_camera.yaw(m_lookAroundSpeed);

        if (m_keyPressing.indexOf(Qt::Key_E) >= 0)
            m_camera.roll(-m_lookAroundSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Q) >= 0)
            m_camera.roll(m_lookAroundSpeed);

        if (m_keyPressing.indexOf(Qt::Key_Shift) >= 0)
            m_camera.standXZ();

        if (m_keyPressing.indexOf(Qt::Key_Tab) >= 0)
            m_camera.lookAtZero();

        if (m_keyPressing.indexOf(Qt::Key_Escape) >= 0)
            m_camera.setPosition(CAMERA_INI_POS, 0.1f);

        if (m_keyPressing.indexOf(Qt::Key_Home) >= 0)
            m_camera.setPosition(QVector3D(), 0.1f);

        if (m_keyPressing.indexOf(Qt::Key_End) >= 0)
            m_camera.setPosition(QVector3D(), -0.1f);

        if (m_isCircleStrafing)
            m_camera.circleStrafing(m_circleStrafingSpeed * m_walkSpeed);

        if (m_simCondition->compute == bhs::Compute::CPU)
            m_particleShaders->updateGL();

        emit UpdateUi::it().displayFrameNumber(m_threadAdmin.frameNum());
        update();
    }
    else if (ev->timerId() == m_fpsTimer.timerId())
    {
        emit UpdateUi::it().displayFps(m_threadAdmin.frameNum() - m_fpsPreFrame);
        m_fpsPreFrame = m_threadAdmin.frameNum();
    }
}

void GraphicWindow::focusOutEvent(QFocusEvent*)
{
    m_keyPressing.clear();
    m_mousePressing = false;
}

void GraphicWindow::enableGridLines(const bool enabled)
{
    m_lineShaders->enableGridLines(enabled);
}

void GraphicWindow::startSim()
{
    int msec = 0;
    if (m_simCondition->compute == bhs::Compute::GPU)
    {
        if (m_particleShaders->numberOfParticle() > 300)
            msec = 1;
        else if (m_particleShaders->numberOfParticle() > 2000)
            msec = 10;
    }
    m_threadAdmin.startSim(msec);
}

void GraphicWindow::setLineType(const int index)
{
    m_lineShaders->setLineType(index);
}

void GraphicWindow::frameAdvance1()
{
    emit UpdateUi::it().frameAdvance(1);
}

void GraphicWindow::frameAdvance10()
{
    emit UpdateUi::it().frameAdvance(10);
}

void GraphicWindow::frameAdvance100()
{
    emit UpdateUi::it().frameAdvance(100);
}

void GraphicWindow::circleStrafing(const bool on)
{
    if (on)
        m_circleStrafingSpeed = -m_circleStrafingSpeed;
    m_isCircleStrafing = on;
    m_camera.lookAtZero(1.0f);
}

void GraphicWindow::resetWaitForDone(const bhs::SimCondition& sim)
{
    m_fpsPreFrame = 0;
    m_simCondition = &sim;
    m_threadAdmin.reset();
}

void GraphicWindow::resetParticles()
{
    m_particleShaders->reset(*m_simCondition);
    m_threadAdmin.setComputeDevice(m_simCondition->compute);
}

void GraphicWindow::setModelScale(const QString& text)
{
    bool ok;
    auto val = text.toDouble(&ok);
    if (ok && val > 0.0)
        m_particleShaders->setModelScale(1.0 / val);
}

void GraphicWindow::setModelScaleInt(int val)
{
    double r1 = (double)val / (double)UpdateUi::SCALE_SLIDER_CENTER;
    m_particleShaders->setModelScaleRatio(pow(r1, 10.0));
}
