#include "particleshaders.h"

ParticleShaders::ParticleShaders(ThreadAdmin* const threadAdmin)
    : m_threadAdmin(threadAdmin)
    , m_pointSizeScale(1.0f)
    , m_pointSize(30.0f)
    , m_computeShaders(new ComputeShaders)
{
}

ParticleShaders::~ParticleShaders()
{
    m_vao.destroy();
}

bool ParticleShaders::initialize(const int screenHeight)
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

    m_computeShaders->initialize();

    return true;
}

void ParticleShaders::setNBodyEngine(const bhs::SimCondition& sim)
{
    if (sim.precision == bhs::Precision::Float) {
        switch (sim.engine) {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineFloat = new G3DMassDiffNBE<float>(sim);
            m_threadAdmin->initializeFloat(m_NBodyEngineFloat, G3DMassDiffCoreFloat::factory);
            break;
        case bhs::Engine::G3DMassIntegral:
            m_NBodyEngineFloat = new G3DMassIntegralNBE<float>(sim);
            m_threadAdmin->initializeFloat(m_NBodyEngineFloat, G3DMassIntegralCoreFloat::factory);
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineFloat = new G3D4DMassDiffNBE<float>(sim);
            m_threadAdmin->initializeFloat(m_NBodyEngineFloat, G3D4DMassDiffCoreFloat::factory);
            break;
        }
    } else {
        switch (sim.engine) {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineDouble = new G3DMassDiffNBE<double>(sim);
            m_threadAdmin->initializeDouble(m_NBodyEngineDouble, G3DMassDiffCoreDouble::factory);
            break;
        case bhs::Engine::G3DMassIntegral:
            m_NBodyEngineDouble = new G3DMassIntegralNBE<double>(sim);
            m_threadAdmin->initializeDouble(m_NBodyEngineDouble, G3DMassIntegralCoreDouble::factory);
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineDouble = new G3D4DMassDiffNBE<double>(sim);
            m_threadAdmin->initializeDouble(m_NBodyEngineDouble, G3D4DMassDiffCoreDouble::factory);
            break;
        }
        m_computeShaders->bindDouble(m_NBodyEngineDouble);
    }
    emit UpdateUi::it().displayEngineName(sim.engine);
    emit UpdateUi::it().displayPrecision(sim.precision);
    emit UpdateUi::it().displayPresetName(sim.preset);

    m_precision = sim.precision;
}

// This function must be called from the GUI thread.
void ParticleShaders::updateGL()
{
    m_computeShaders->run();
    if (numberOfParticle() == 0)
        return;
    const void* const coord = coordinates();
    if (coord == nullptr)
        return;

    m_vao.bind();

    const int VECTOR_SIZE = 3;
    GLenum precision = GL_DOUBLE;
    quint64 size = sizeof(double);

    if (m_precision == bhs::Precision::Float) {
        precision = GL_FLOAT;
        size = sizeof(float);
    }

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(coord, numberOfParticle() * VECTOR_SIZE * size);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, precision, 0, VECTOR_SIZE);

    m_vao.release();
}

void ParticleShaders::paint(const QMatrix4x4& viewProjection)
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

void ParticleShaders::resize(int height)
{
    m_pointSizeScale = (double)height / (double)m_initHeight;
}

void ParticleShaders::reset(const bhs::SimCondition& sim)
{
    if (m_NBodyEngineFloat) {
        delete m_NBodyEngineFloat;
        m_NBodyEngineFloat = nullptr;
    }
    if (m_NBodyEngineDouble) {
        delete m_NBodyEngineDouble;
        m_NBodyEngineDouble = nullptr;
    }
    setNBodyEngine(sim);
}

void ParticleShaders::setModelScale(double val)
{
    if (m_NBodyEngineFloat) {
        m_NBodyEngineFloat->setModelScale(val);
    }
    if (m_NBodyEngineDouble) {
        m_NBodyEngineDouble->setModelScale(val);
    }
}

void ParticleShaders::setModelScaleRatio(double val)
{
    if (m_NBodyEngineFloat) {
        m_NBodyEngineFloat->setModelScaleRatio(val);
    }
    if (m_NBodyEngineDouble) {
        m_NBodyEngineDouble->setModelScaleRatio(val);
    }
}

quint64 ParticleShaders::numberOfParticle() const
{
    quint64 ret = 0;
    if (m_NBodyEngineFloat) {
        ret = m_NBodyEngineFloat->numberOfParticle();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->numberOfParticle();
    }
    return ret;
}

const void* ParticleShaders::coordinates() const
{
    const void* ret = nullptr;
    if (m_NBodyEngineFloat) {
        ret = m_NBodyEngineFloat->coordinates();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->coordinates();
    }
    return ret;
}

double ParticleShaders::modelScale() const
{
    double ret = 0.0;
    if (m_NBodyEngineFloat) {
        ret = m_NBodyEngineFloat->modelScale();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->modelScale();
    }
    return ret;
}
