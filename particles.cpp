#include "particles.h"

Particles::Particles(int screenHeight)
    : pointSizeScale(1.0f)
    , pointSize(30.0f)
    , initHeight(screenHeight)
{
}

Particles::~Particles()
{
    m_vao.destroy();
}

bool Particles::initialize()
{
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

void Particles::setNumberOfParticles(int num)
{
    m_NBodyEngine = new Gravity3DMassDifferentialNBodyEngine(num, 3600.0f);

    m_NBodyEngine->newParticles();

    //initParticlesRandam();
    m_NBodyEngine->initialize(1);

    updateParticles();
}

void Particles::updateParticles()
{
    if (m_NBodyEngine->getNumberOfParticle() == 0)
        return;

    m_vao.bind();

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(m_NBodyEngine->getCoordinates(), m_NBodyEngine->getNumberOfParticle() * 12);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

//    QOpenGLBuffer colorBuf;
//    colorBuf.create();
//    colorBuf.bind();
//    colorBuf.allocate(particleColor, sizeof(particleColor));
//    m_program.enableAttributeArray(1);
//    m_program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

    m_vao.release();

    m_NBodyEngine->timeProgress();
    m_NBodyEngine->calculateInteraction();
}

void Particles::paint(QMatrix4x4 viewProjection)
{
    if (m_NBodyEngine->getNumberOfParticle() == 0)
        return;
    m_program.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(1.0e-11f);

    m_program.setUniformValue("mvp_matrix", viewProjection * modelMatrix);
    m_program.setUniformValue("size", pointSize * pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, m_NBodyEngine->getNumberOfParticle());

    m_vao.release();
    m_program.release();
}

void Particles::resize(int height)
{
    pointSizeScale = (float)height / (float)initHeight;
}
