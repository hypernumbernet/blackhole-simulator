#include "particles.h"

Particles::Particles(ThreadAdmin* const threadAdmin)
    : m_threadAdmin(threadAdmin)
    , m_pointSizeScale(1.0f)
    , m_pointSize(30.0f)
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

void Particles::selectNBodyEngine(const bhs::SimCondition& sim)
{
    switch (sim.engine) {
    default:
        m_NBodyEngine = new G3DMassDiffNBE<float>(sim);

        for (int i = 0; i < m_threadAdmin->size(); ++i) {
            m_threadAdmin->at(i)->initialize(new G3DMassDiffCore(m_NBodyEngine));
        }
        break;
    case 1:
        m_NBodyEngine = new G3DMassIntegralNBE<float>(sim);

        for (int i = 0; i < m_threadAdmin->size(); ++i) {
            m_threadAdmin->at(i)->initialize(new G3DMassIntegralCore(m_NBodyEngine));
        }
        break;
    case 2:
        m_NBodyEngine = new G3SVMassDiffNBE<float>(sim);

        for (int i = 0; i < m_threadAdmin->size(); ++i) {
            m_threadAdmin->at(i)->initialize(new G3SVMassDiffCore(m_NBodyEngine));
        }
        break;
    }

    QString engine = UpdateUi::it().ENGINE->value(sim.engine);
    emit UpdateUi::it().displayEngineName(engine);

    emit UpdateUi::it().displayPrecision(sim.precision);

    QString preset = UpdateUi::it().PRESET->value(sim.preset);
    emit UpdateUi::it().displayPresetName(preset);
}

// This function must be called from the GUI thread.
void Particles::updateGL()
{
    if (m_NBodyEngine->numberOfParticle() == 0)
        return;

    m_vao.bind();

    const int VECTOR_SIZE = 3;

//    GLenum precision = GL_DOUBLE;
//    quint64 size = sizeof(double);
    GLenum precision = GL_FLOAT;
    quint64 size = sizeof(float);

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(m_NBodyEngine->coordinates(), m_NBodyEngine->numberOfParticle() * VECTOR_SIZE * size);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, precision, 0, VECTOR_SIZE);

    m_vao.release();
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

void Particles::reset(const bhs::SimCondition& sim)
{
    delete m_NBodyEngine;    
    selectNBodyEngine(sim);
}

void Particles::setModelScale(float val)
{
    m_NBodyEngine->setModelScale(val);
}

void Particles::setModelScaleRatio(double val)
{
    m_NBodyEngine->setModelScaleRatio(val);
}
