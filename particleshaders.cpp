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

    if (!m_programFloat.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particle_float.vert")) {
        return false;
    }
    if (!m_programFloat.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particle.frag")) {
        return false;
    }
    if (!m_programFloat.link()) {
        return false;
    }

    if (!m_programDouble.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particle_float.vert")) {
        return false;
    }
    if (!m_programDouble.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particle.frag")) {
        return false;
    }
    if (!m_programDouble.link()) {
        return false;
    }

    if (!m_vao.create()) {
        return false;
    }

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    //glPointSize(pointSize);

    //m_computeShaders->initialize();

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
        //m_computeShaders->bindDouble(m_NBodyEngineDouble);
    }
    emit UpdateUi::it().displayEngineName(sim.engine);
    emit UpdateUi::it().displayPrecision(sim.precision);
    emit UpdateUi::it().displayPresetName(sim.preset);

    m_precision = sim.precision;

    const int VECTOR_SIZE = 3;
    quint64 size = sizeof(double);
    GLenum precision = GL_DOUBLE;
    if (m_precision == bhs::Precision::Float) {
        size = sizeof(float);
        precision = GL_FLOAT;
    }

    //program()->bind();
    //uint32_t ssbo = 0;
    glGenBuffers(1, &m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    //glBufferStorage(GL_SHADER_STORAGE_BUFFER, numberOfParticle() * VECTOR_SIZE * size, coordinates(), GL_DYNAMIC_STORAGE_BIT);

//    double* data = new double[numberOfParticle() * 4];
//    const double* const org = static_cast<const double*>(coordinates());
//    for (quint64 i = 0; i < numberOfParticle(); ++i) {
//        quint64 i3 = i * 3;
//        quint64 i4 = i * 4;
//        data[i4] = org[i3];
//        data[i4+1] = org[i3+1];
//        data[i4+2] = org[i3+2];
//        data[i4+3] = 1.0;
//    }

    glBufferStorage(GL_SHADER_STORAGE_BUFFER, numberOfParticle() * VECTOR_SIZE * size, coordinates(), GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfParticle() * VECTOR_SIZE * size, data, GL_DYNAMIC_DRAW);

    m_vao.bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo);
    glVertexAttribPointer(0, VECTOR_SIZE, precision, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //m_vao.release();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    m_vao.release();

    const int LAYOUT_BINDING = 0;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, LAYOUT_BINDING, m_ssbo);

    //program()->release();
}

// This function must be called from the GUI thread.
void ParticleShaders::updateGL()
{
    //m_computeShaders->run();
    if (numberOfParticle() == 0)
        return;
    const void* const coord = coordinates();
    if (coord == nullptr)
        return;

    const int VECTOR_SIZE = 3;
    quint64 size = sizeof(double);
    if (m_precision == bhs::Precision::Float) {
        size = sizeof(float);
    }

//    double* data = new double[numberOfParticle() * 4];
//    const double* const org = static_cast<const double*>(coordinates());
//    for (quint64 i = 0; i < numberOfParticle(); ++i) {
//        quint64 i3 = i * 3;
//        quint64 i4 = i * 4;
//        data[i4] = org[i3];
//        data[i4+1] = org[i3+1];
//        data[i4+2] = org[i3+2];
//        data[i4+3] = 1.0;
//    }

//    const double* const org = static_cast<const double*>(coordinates());
//    for (int i = 0; i < 6; ++i)
//        qDebug() << i << org[i];

    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, numberOfParticle() * VECTOR_SIZE * size, coordinates());
}

void ParticleShaders::paint(const QMatrix4x4& viewProjection)
{
    if (numberOfParticle() == 0)
        return;

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(modelScale());

    program()->bind();
    program()->setUniformValue("mvp_matrix", viewProjection * modelMatrix);
    program()->setUniformValue("size", m_pointSize * m_pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, numberOfParticle());

    m_vao.release();
    program()->release();
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

QOpenGLShaderProgram* ParticleShaders::program()
{
    if (m_precision == bhs::Precision::Float)
        return &m_programFloat;
    else
        return &m_programDouble;
}
