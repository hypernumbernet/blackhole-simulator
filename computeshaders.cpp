#include "computeshaders.h"

#include <QDebug>
#include <iterator>

#ifdef QT_DEBUG
void APIENTRY gl_debug_callback(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei/* length*/, const GLchar* msg, const void* /*data*/)
{
    QString source_str;
    QString type_str;
    QString severity_str;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        source_str = "SOURCE_API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_str = "SOURCE_WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_str = "SOURCE_SHADER_COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_str = "SOURCE_THIRD_PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_str = "SOURCE_APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_str = "SOURCE_OTHER";
        break;
    default:
        source_str = "?";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        type_str = "TYPE_ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_str = "TYPE_DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_str = "TYPE_UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_str = "TYPE_PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_str = "TYPE_PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_str = "TYPE_OTHER";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_str = "TYPE_MARKER";
        break;
    default:
        type_str = "?";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_str = "SEVERITY_HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_str = "SEVERITY_MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_str = "SEVERITY_LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_str = "SEVERITY_NOTIFICATION";
        break;
    default:
        severity_str = "?";
        break;
    }
    qDebug() << "[" << source_str << "][" << type_str << "][" << id << "][" << severity_str << "] " << msg;
}
#endif

ComputeShaders::ComputeShaders()
{
}

ComputeShaders::~ComputeShaders()
{
    glDeleteBuffers(1, &m_bufferHandle);
}

bool ComputeShaders::initialize()
{
    if (!initializeOpenGLFunctions()) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Compute, ":/shader/timeprogress.comp")) {
        return false;
    }
    if (!m_program.link()) {
        return false;
    }

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &m_maxComputeWorkSizeX);

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &m_maxComputeWorkCountX);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &m_maxComputeWorkCountY);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &m_maxComputeWorkCountZ);

    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &m_maxConputeWorkInvocations);

    m_maxComputeWorkCount = m_maxComputeWorkCountX * m_maxComputeWorkCountY * m_maxComputeWorkCountZ;

#ifdef QT_DEBUG
//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(gl_debug_callback, nullptr);
//    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    return true;
}

void ComputeShaders::bindDouble(AbstractNBodyEngine<double>* engine)
{
    glGenBuffers(1, &m_bufferHandle);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferHandle);

    GLsizeiptr num = engine->numberOfParticle();
    //m_maxComputeWorkCount = ((size + 128 - 1) / 128);
    m_maxComputeWorkCount = num;

    const double* coords = engine->coordinates();
    GLsizeiptr coordsSize = num * 3;
    const double* vels = engine->velocities();
    GLsizeiptr velsSize = num * 3;
    GLsizeiptr total = coordsSize + velsSize;
    double* glData = new double[total];

    for (GLsizeiptr i = 0; i < coordsSize; ++i) {
        glData[i] = coords[i];
    }
    for (GLsizeiptr i = 0; i < velsSize; ++i) {
        glData[coordsSize + i] = vels[i];
    }

//    glBufferStorage(GL_SHADER_STORAGE_BUFFER, total * sizeof(double), glData, GL_DYNAMIC_STORAGE_BIT);

//    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_bufferHandle, 0, coordsSize * sizeof(double));
//    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, m_bufferHandle, coordsSize * sizeof(double), velsSize * sizeof(double));

    glBufferStorage(GL_SHADER_STORAGE_BUFFER, coordsSize * sizeof(double), engine->coordinates(), GL_DYNAMIC_STORAGE_BIT);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_bufferHandle, 0, coordsSize * sizeof(double));
    //glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, m_bufferHandle, coordsSize * sizeof(double), velsSize * sizeof(double));


}

void ComputeShaders::update(const quint64 numberOfParticles)
{
    GLuint num = ((numberOfParticles + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE);
    m_program.bind();
    glDispatchCompute(num, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //m_program.release();
}

//GLuint ComputeShaders::numberOfWorkGroups()
//{
//    return
//}
