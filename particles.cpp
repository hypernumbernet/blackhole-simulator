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
    if (sim.precision == bhs::Precision::Single) {
        switch (sim.engine) {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineSingle = new G3DMassDiffNBE<float>(sim);
            for (int i = 0; i < m_threadAdmin->size(); ++i) {
                m_threadAdmin->at(i)->initialize(new G3DMassDiffCoreSingle(m_NBodyEngineSingle));
            }
            break;
        case bhs::Engine::G3DMassIntegral:
            m_NBodyEngineSingle = new G3DMassIntegralNBE<float>(sim);
            for (int i = 0; i < m_threadAdmin->size(); ++i) {
                m_threadAdmin->at(i)->initialize(new G3DMassIntegralCoreSingle(m_NBodyEngineSingle));
            }
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineSingle = new G3D4DMassDiffNBE<float>(sim);
            for (int i = 0; i < m_threadAdmin->size(); ++i) {
                m_threadAdmin->at(i)->initialize(new G3D4DMassDiffCoreSingle(m_NBodyEngineSingle));
            }
            break;
        }
    } else {
        switch (sim.engine) {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineDouble = new G3DMassDiffNBE<double>(sim);
            for (int i = 0; i < m_threadAdmin->size(); ++i) {
                m_threadAdmin->at(i)->initialize(new G3DMassDiffCoreDouble(m_NBodyEngineDouble));
            }
            break;
        case bhs::Engine::G3DMassIntegral:
            m_NBodyEngineDouble = new G3DMassIntegralNBE<double>(sim);
            for (int i = 0; i < m_threadAdmin->size(); ++i) {
                m_threadAdmin->at(i)->initialize(new G3DMassIntegralCoreDouble(m_NBodyEngineDouble));
            }
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineDouble = new G3D4DMassDiffNBE<double>(sim);
            for (int i = 0; i < m_threadAdmin->size(); ++i) {
                m_threadAdmin->at(i)->initialize(new G3D4DMassDiffCoreDouble(m_NBodyEngineDouble));
            }
            break;
        }
    }
    emit UpdateUi::it().displayEngineName(sim.engine);
    emit UpdateUi::it().displayPrecision(sim.precision);
    emit UpdateUi::it().displayPresetName(sim.preset);

    m_precision = sim.precision;
}

// This function must be called from the GUI thread.
void Particles::updateGL()
{
    if (numberOfParticle() == 0)
        return;

    m_vao.bind();

    const int VECTOR_SIZE = 3;
    GLenum precision = GL_DOUBLE;
    quint64 size = sizeof(double);

    if (m_precision == bhs::Precision::Single) {
        precision = GL_FLOAT;
        size = sizeof(float);
    }

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    const void* ret = coordinates();
    if (ret)
        glBuf.allocate(coordinates(), numberOfParticle() * VECTOR_SIZE * size);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, precision, 0, VECTOR_SIZE);

    m_vao.release();
}

void Particles::paint(const QMatrix4x4& viewProjection)
{
    if (numberOfParticle() == 0)
        return;
    m_program.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(modelScale());

    m_program.setUniformValue("mvp_matrix", viewProjection * modelMatrix);
    m_program.setUniformValue("size", m_pointSize * m_pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, numberOfParticle());

    m_vao.release();
    m_program.release();
}

void Particles::resize(int height)
{
    m_pointSizeScale = (double)height / (double)m_initHeight;
}

void Particles::reset(const bhs::SimCondition& sim)
{
    if (m_NBodyEngineSingle) {
        delete m_NBodyEngineSingle;
        m_NBodyEngineSingle = nullptr;
    }
    if (m_NBodyEngineDouble) {
        delete m_NBodyEngineDouble;
        m_NBodyEngineDouble = nullptr;
    }
    selectNBodyEngine(sim);
}

void Particles::setModelScale(double val)
{
    if (m_NBodyEngineSingle) {
        m_NBodyEngineSingle->setModelScale(val);
    }
    if (m_NBodyEngineDouble) {
        m_NBodyEngineDouble->setModelScale(val);
    }
}

void Particles::setModelScaleRatio(double val)
{
    if (m_NBodyEngineSingle) {
        m_NBodyEngineSingle->setModelScaleRatio(val);
    }
    if (m_NBodyEngineDouble) {
        m_NBodyEngineDouble->setModelScaleRatio(val);
    }
}

quint64 Particles::numberOfParticle() const
{
    quint64 ret = 0;
    if (m_NBodyEngineSingle) {
        ret = m_NBodyEngineSingle->numberOfParticle();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->numberOfParticle();
    }
    return ret;
}

const void* Particles::coordinates() const
{
    const void* ret = nullptr;
    if (m_NBodyEngineSingle) {
        ret = m_NBodyEngineSingle->coordinates();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->coordinates();
    }
    return ret;
}

double Particles::modelScale() const
{
    double ret = 0.0;
    if (m_NBodyEngineSingle) {
        ret = m_NBodyEngineSingle->modelScale();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->modelScale();
    }
    return ret;
}
