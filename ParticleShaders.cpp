#include "ParticleShaders.h"
#include "engine/CoreTrapezoid.h"
#include "engine/CoreEvenlyDivided.h"
#include "engine/3D/Engine3D.h"
#include "engine/3D/CoreDouble3D.h"
#include "engine/Universe1/engine3d4d.h"
#include "engine/Universe1/coredouble3d4d.h"
#include "engine/Universe2/engine4d3d.h"
#include "engine/Universe2/coredouble4d3d.h"
#include "engine/GravityCollision/EngineGravityCollision.h"
#include "engine/GravityCollision/CoreDoubleGravityCollision.h"
#include "engine/Relativity1/EngineRelativity1.h"
#include "engine/Relativity1/CoreDoubleRelativity1.h"
#include "engine/Relativity2/EngineRelativity2.h"
#include "engine/Relativity2/CoreDoubleRelativity2.h"
#include "engine/Relativity3/EngineRelativity3.h"
#include "engine/Relativity3/CoreDoubleRelativity3.h"

using namespace bhs;

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

void ParticleShaders::setNBodyEngine(const SimCondition& sim)
{
    m_precision = sim.precision;
    int coordinateVectorSize = 3;
    int velocityVectorSize = 3;

    if (sim.precision == Precision::Float)
    {
        switch (sim.engine)
        {
        case Engine::Gravity3D:
            m_NBodyEngineFloat = new Engine3D<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreTrapezoid::factory);
            break;
        case Engine::Universe1:
            m_NBodyEngineFloat = new Engine3D4D<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreEvenlyDivided::factory);
            velocityVectorSize = 4;
            break;
        case Engine::Relativity1:
            m_NBodyEngineFloat = new EngineRelativity1<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreEvenlyDivided::factory);
            velocityVectorSize = 4;
            break;
        case Engine::Universe2:
            m_NBodyEngineFloat = new Engine4D3D<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreTrapezoid::factory);
            break;
        case Engine::GravityCollision:
            m_NBodyEngineFloat = new EngineGravityCollision<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreTrapezoid::factory);
            break;
        case Engine::Relativity2:
            m_NBodyEngineFloat = new EngineRelativity2<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreEvenlyDivided::factory);
            break;
        case Engine::Relativity3:
            m_NBodyEngineFloat = new EngineRelativity3<float>(sim);
            m_threadAdmin->initialize(m_NBodyEngineFloat, CoreTrapezoid::factory);
            break;
        }
    } else {
        switch (sim.engine)
        {
        case Engine::Gravity3D:
            m_NBodyEngineDouble = new Engine3D<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDouble3D::factory);
            break;
        case Engine::Universe1:
            m_NBodyEngineDouble = new Engine3D4D<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDouble3D4D::factory);
            velocityVectorSize = 4;
            break;
        case Engine::Relativity1:
            m_NBodyEngineDouble = new EngineRelativity1<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDoubleRelativity1::factory);
            velocityVectorSize = 4;
            break;
        case Engine::Universe2:
            m_NBodyEngineDouble = new Engine4D3D<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDouble4D3D::factory);
            break;
        case Engine::GravityCollision:
            m_NBodyEngineDouble = new EngineGravityCollision<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDoubleGravityCollision::factory);
            break;
        case Engine::Relativity2:
            m_NBodyEngineDouble = new EngineRelativity2<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDoubleRelativity2::factory);
            break;
        case Engine::Relativity3:
            m_NBodyEngineDouble = new EngineRelativity3<double>(sim);
            m_threadAdmin->initialize(m_NBodyEngineDouble, CoreDoubleRelativity3::factory);
            break;
        }
    }
    emit UpdateUi::it().displayEngineName(sim.engine);
    emit UpdateUi::it().displayPrecision(sim.precision);
    emit UpdateUi::it().displayPresetName(sim.preset, sim.custom.name);
    emit UpdateUi::it().displayCompute(sim.compute);

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

    const quint64 VECTOR_SIZE = 3;
    if (m_precision == Precision::Float)
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

void ParticleShaders::reset(const SimCondition& sim)
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
    if (m_precision == Precision::Float)
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

    if (m_precision == Precision::Float)
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

QString ParticleShaders::particleData()
{
    if (m_precision == Precision::Float)
        return particleDataToString<float>();
    else
        return particleDataToString<double>();
}
