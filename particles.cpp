#include "particles.h"

Particles::Particles(UpdateUi* const updateUi)
    : m_pointSizeScale(1.0f)
    , m_pointSize(30.0f)
    , m_numberOfParticle(400)
    , m_updateUi(updateUi)
{
}

Particles::~Particles()
{
    m_vao.destroy();
}

bool Particles::initialize(const int screenHeight)
{
    m_initHeight = screenHeight;
    if (!initializeOpenGLFunctions()) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particle.vert")) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particle.frag")) {
        return false;
    }
    if (!m_program.link()) {
        return false;
    }
    if (!m_vao.create()) {
        return false;
    }

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    //glPointSize(pointSize);

    return true;
}

void Particles::selectNBodyEngine(const UpdateUi::SimCondition& sim)
{
    auto presetValue = static_cast<AbstractNBodyEngine::Preset>(sim.preset);

    switch (sim.engine) {
    default:
        m_NBodyEngine = new G3DMassDiffNBE(
                    m_updateUi,
                    m_numberOfParticle,
                    500.0f,
                    presetValue);
        break;
    case 1:
        m_NBodyEngine = new G3DMassIntegralNBE(
                    m_updateUi,
                    m_numberOfParticle,
                    500.0f,
                    presetValue);
        break;
    }

    QString name = m_updateUi->NBODY_ENGINE_MAP->value(sim.engine);
    emit m_updateUi->displayEngineName(name);

    QString presetDisplay = m_updateUi->INITIAL_CONDITION_MAP->value(sim.preset);
    emit m_updateUi->displayPresetName(presetDisplay);

    updateParticles();
}

void Particles::updateParticles()
{
    if (m_NBodyEngine->numberOfParticle() == 0)
        return;

    m_vao.bind();

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(m_NBodyEngine->coordinates(), m_NBodyEngine->numberOfParticle() * 12);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_vao.release();

    m_NBodyEngine->calculateTimeProgress();
    m_NBodyEngine->calculateInteraction();
}

void Particles::paint(const QMatrix4x4& viewProjection)
{
    if (m_NBodyEngine->numberOfParticle() == 0)
        return;
    m_program.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(m_NBodyEngine->modelScale());

    m_program.setUniformValue("mvp_matrix", viewProjection * modelMatrix);
    m_program.setUniformValue("size", m_pointSize * m_pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, m_NBodyEngine->numberOfParticle());

    m_vao.release();
    m_program.release();
}

void Particles::resize(int height)
{
    m_pointSizeScale = (float)height / (float)m_initHeight;
}

void Particles::reset(const UpdateUi::SimCondition& sim)
{
    delete m_NBodyEngine;
    selectNBodyEngine(sim);
}

void Particles::setModelScale(float val)
{
    m_NBodyEngine->setModelScale(val);
}

void Particles::setModelScaleRatio(float val)
{
    m_NBodyEngine->setModelScaleRatio(val);
}
