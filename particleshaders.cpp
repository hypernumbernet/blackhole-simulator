#include "particleshaders.h"

ParticleShaders::ParticleShaders(ThreadAdmin* const threadAdmin)
    : m_threadAdmin(threadAdmin)
    , m_pointSizeScale(1.0f)
    , m_pointSize(30.0f)
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

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/particle.vert")) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/particle.frag")) {
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

void ParticleShaders::setNBodyEngine(const bhs::SimCondition& sim)
{
    if (sim.precision == bhs::Precision::Float) {
        switch (sim.engine) {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineFloat = new G3DMassDiffNBE<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, G3DMassDiffCoreFloat::factory);
            break;
        case bhs::Engine::G3DMassIntegral:
            m_NBodyEngineFloat = new G3DMassIntegralNBE<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, G3DMassIntegralCoreFloat::factory);
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineFloat = new G3D4DMassDiffNBE<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, G3D4DMassDiffCoreFloat::factory);
            break;
        }
    } else {
        switch (sim.engine) {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineDouble = new G3DMassDiffNBE<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, G3DMassDiffCoreDouble::factory);
            break;
        case bhs::Engine::G3DMassIntegral:
            m_NBodyEngineDouble = new G3DMassIntegralNBE<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, G3DMassIntegralCoreDouble::factory);
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineDouble = new G3D4DMassDiffNBE<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, G3D4DMassDiffCoreDouble::factory);
            break;
        }
    }
    emit UpdateUi::it().displayEngineName(sim.engine);
    emit UpdateUi::it().displayPrecision(sim.precision);
    emit UpdateUi::it().displayPresetName(sim.preset);

    m_precision = sim.precision;

    const int VECTOR_SIZE = 3;

    int size = numberOfParticle() * VECTOR_SIZE * sizeof(double);
    GLenum precision = GL_DOUBLE;
    if (m_precision == bhs::Precision::Float) {
        size = numberOfParticle() * VECTOR_SIZE * sizeof(float);
        precision = GL_FLOAT;
    }

    uint32_t ssboCoord = 0;
    glGenBuffers(1, &ssboCoord);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboCoord);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, size, coordinates(), GL_DYNAMIC_STORAGE_BIT);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    m_vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, ssboCoord);
    glVertexAttribPointer(0, VECTOR_SIZE, precision, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_vao.release();

    const int LAYOUT_BINDING = 0;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, LAYOUT_BINDING, ssboCoord);

//    uint32_t ssboVels = 0;
//    glGenBuffers(1, &ssboVels);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboVels);
//    glBufferStorage(GL_SHADER_STORAGE_BUFFER, size, velocities(), GL_DYNAMIC_STORAGE_BIT);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboVels);

}

// This function must be called from the GUI thread.
void ParticleShaders::updateGL()
{
    if (numberOfParticle() == 0)
        return;
    if (coordinates() == nullptr)
        return;

    const int VECTOR_SIZE = 3;
    quint64 size = sizeof(double);
    if (m_precision == bhs::Precision::Float) {
        size = sizeof(float);
    }

    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, numberOfParticle() * VECTOR_SIZE * size, coordinates());
}

void ParticleShaders::paint(const QMatrix4x4& viewProjection)
{
    if (numberOfParticle() == 0)
        return;

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(modelScale());

    m_program.bind();
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

const void* ParticleShaders::velocities() const
{
    const void* ret = nullptr;
    if (m_NBodyEngineFloat) {
        ret = m_NBodyEngineFloat->velocities();
    }
    if (m_NBodyEngineDouble) {
        ret = m_NBodyEngineDouble->velocities();
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
