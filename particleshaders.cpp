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
    if (!initializeOpenGLFunctions())
        return false;

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/particle.vert"))
        return false;

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/particle.frag"))
        return false;

    if (!m_program.link())
        return false;

    if (!m_vao.create())
        return false;

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    //glPointSize(pointSize);

    return true;
}

void ParticleShaders::setNBodyEngine(const bhs::SimCondition& sim)
{
    m_precision = sim.precision;
    int coordinateVectorSize = 3;
    int velocityVectorSize = 3;

    if (sim.precision == bhs::Precision::Float)
    {
        switch (sim.engine)
        {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineFloat = new G3DMassDiffNBE<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, G3DMassDiffCoreFloat::factory);
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineFloat = new G3D4DMassDiffNBE<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, G3D4DMassDiffCoreFloat::factory);
            velocityVectorSize = 4;
            break;
        }
    } else {
        switch (sim.engine)
        {
        case bhs::Engine::G3DMassDiff:
        default:
            m_NBodyEngineDouble = new G3DMassDiffNBE<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, G3DMassDiffCoreDouble::factory);
            break;
        case bhs::Engine::G3D4DMassDiff:
            m_NBodyEngineDouble = new G3D4DMassDiffNBE<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, G3D4DMassDiffCoreDouble::factory);
            velocityVectorSize = 4;
            break;
        }
    }
    emit UpdateUi::it().displayEngineName(sim.engine);
    emit UpdateUi::it().displayPrecision(sim.precision);
    emit UpdateUi::it().displayPresetName(sim.preset);

    SSBODataStruct ssboStruct;
    GetSSBOStruct(ssboStruct, coordinateVectorSize, velocityVectorSize);

    quint32 ssbo = 0;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, ssboStruct.total, ssboStruct.data, GL_DYNAMIC_STORAGE_BIT);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, ssbo, ssboStruct.coordinateOffset, ssboStruct.coordinateSize);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, ssbo, ssboStruct.velocityOffset, ssboStruct.velocitySize);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, ssbo, ssboStruct.massOffset, ssboStruct.massSize);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, ssbo, ssboStruct.paramOffset, ssboStruct.paramSize);

    m_vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, ssbo);
    glVertexAttribPointer(0, coordinateVectorSize, ssboStruct.precision, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_vao.release();
}

// This function must be called from the GUI thread.
void ParticleShaders::updateGL()
{
    if (numberOfParticle() == 0)
        return;

    const int VECTOR_SIZE = 3;
    if (m_precision == bhs::Precision::Float)
    {
        quint64 size = numberOfParticle() * VECTOR_SIZE * sizeof(float);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, coordinates<float>());
    } else {
        quint64 size = numberOfParticle() * VECTOR_SIZE * sizeof(double);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, coordinates<double>());
    }
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
    if (m_NBodyEngineFloat)
    {
        delete m_NBodyEngineFloat;
        m_NBodyEngineFloat = nullptr;
    }
    if (m_NBodyEngineDouble)
    {
        delete m_NBodyEngineDouble;
        m_NBodyEngineDouble = nullptr;
    }
    setNBodyEngine(sim);
}

void ParticleShaders::setModelScale(double val)
{
    if (m_NBodyEngineFloat)
        m_NBodyEngineFloat->setModelScale(val);

    if (m_NBodyEngineDouble)
        m_NBodyEngineDouble->setModelScale(val);
}

void ParticleShaders::setModelScaleRatio(double val)
{
    if (m_NBodyEngineFloat)
        m_NBodyEngineFloat->setModelScaleRatio(val);

    if (m_NBodyEngineDouble)
        m_NBodyEngineDouble->setModelScaleRatio(val);
}

quint64 ParticleShaders::numberOfParticle() const
{
    quint64 ret = 0;
    if (m_NBodyEngineFloat)
        ret = m_NBodyEngineFloat->numberOfParticle();

    if (m_NBodyEngineDouble)
        ret = m_NBodyEngineDouble->numberOfParticle();

    return ret;
}

const void* ParticleShaders::ssboData(SSBODataStruct& result, int coordinateVectorSize, int velocityVectorSize) const
{
    const void* ret = nullptr;
    if (m_precision == bhs::Precision::Float)
        ret = makeSSBOData<float>(result, coordinateVectorSize, velocityVectorSize);
    else
        ret = makeSSBOData<double>(result, coordinateVectorSize, velocityVectorSize);

    return ret;
}

double ParticleShaders::modelScale() const
{
    double ret = 0.0;
    if (m_NBodyEngineFloat)
        ret = m_NBodyEngineFloat->modelScale();

    if (m_NBodyEngineDouble)
        ret = m_NBodyEngineDouble->modelScale();

    return ret;
}

void ParticleShaders::GetSSBOStruct(SSBODataStruct& result, int coordinateVectorSize, int velocityVectorSize) const
{
    SSBODataStruct ssboDataStruct;
    result.data = ssboData(ssboDataStruct, coordinateVectorSize, velocityVectorSize);

    if (m_precision == bhs::Precision::Float)
    {
        result.dataSize = sizeof(float);
        result.precision = GL_FLOAT;
    } else {
        result.dataSize = sizeof(double);
        result.precision = GL_DOUBLE;
    }
    result.coordinateOffset = 0;
    result.coordinateSize = ssboDataStruct.coordinateSize * result.dataSize;
    result.velocityOffset = ssboDataStruct.velocityOffset * result.dataSize;
    result.velocitySize = ssboDataStruct.velocitySize * result.dataSize;
    result.massOffset = ssboDataStruct.massOffset * result.dataSize;
    result.massSize = ssboDataStruct.massSize * result.dataSize;
    result.paramOffset = ssboDataStruct.paramOffset * result.dataSize;
    result.paramSize = ssboDataStruct.paramSize * result.dataSize;
    result.total = ssboDataStruct.total * result.dataSize;
}
