#include "particles.h"

static float randf(){return (float)rand()/(float)RAND_MAX;}

Particles::Particles(int screenHeight)
    : timePerFrame(3600.0f)
    , pointSizeScale(1.0f)
    , pointSize(30.0f)
    , initHeight(screenHeight)
    , numberOfParticles(0)
{
    location = new float[0];
}

Particles::~Particles()
{
    m_vao.destroy();
    delete[] location;
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
    numberOfParticles = num;
    delete[] location;
    location = new float[numberOfParticles * 3];
    for (int i = 0; i < numberOfParticles * 3; ++i)
    {
        location[i] = randf() * 2.0f - 1.0f;
    }
    updateParticles();
}

void Particles::updateParticles()
{
    if (numberOfParticles == 0)
        return;
    auto rot = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), 1.0f);
    for (int i = 0; i < numberOfParticles*3; i+=3)
    {
        auto result = rot.rotatedVector(QVector3D(location[i], location[i + 1], location[i + 2]));
        location[i] = result.x();
        location[i + 1] = result.y();
        location[i + 2] = result.z();
    }

    m_vao.bind();

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(location, numberOfParticles * 12);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

//    QOpenGLBuffer colorBuf;
//    colorBuf.create();
//    colorBuf.bind();
//    colorBuf.allocate(particleColor, sizeof(particleColor));
//    m_program.enableAttributeArray(1);
//    m_program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

    m_vao.release();
}

void Particles::paint(QMatrix4x4 viewProjection)
{
    if (numberOfParticles == 0)
        return;
    m_program.bind();
    m_program.setUniformValue("mvp_matrix", viewProjection);
    m_program.setUniformValue("size", pointSize * pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, numberOfParticles);

    m_vao.release();
    m_program.release();
}

void Particles::resize(int height)
{
    pointSizeScale = (float)height / (float)initHeight;
}
