#include "particles.h"

Particles::Particles(UpdateUi* const updateUi)
    : pointSizeScale(1.0f)
    , pointSize(30.0f)
    , numberOfParticle(400)
    , m_updateUi(updateUi)
{
}

Particles::~Particles()
{
    m_vao.destroy();
}

bool Particles::initialize(const int screenHeight)
{
    initHeight = screenHeight;
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

void Particles::selectNBodyEngine(const int engineIndex)
{
    switch (engineIndex) {
    default:
        m_NBodyEngine = new G3DMassDiffNBE(
                    m_updateUi,
                    numberOfParticle,
                    500.0f,
                    G3DMassDiffNBE::Preset::SunEarth);
        break;
    case 1:
        m_NBodyEngine = new G3DMassIntegralNBE(
                    m_updateUi,
                    numberOfParticle,
                    500.0f,
                    G3DMassIntegralNBE::Preset::SunEarth);
        break;
    }
    QString name = m_updateUi->NBODY_ENGINE_MAP->value(engineIndex < 0 ? 0 : engineIndex);
    emit m_updateUi->displayEngineName(name);
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
    m_program.setUniformValue("size", pointSize * pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, m_NBodyEngine->numberOfParticle());

    m_vao.release();
    m_program.release();
}

void Particles::resize(int height)
{
    pointSizeScale = (float)height / (float)initHeight;
}

void Particles::reset(const int engineIndex)
{
    delete m_NBodyEngine;
    selectNBodyEngine(engineIndex);
}

void Particles::setModelScale(float val)
{
    m_NBodyEngine->setModelScale(val);
}

void Particles::setModelScaleRatio(float val)
{
    m_NBodyEngine->setModelScaleRatio(val);
}
