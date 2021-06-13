#include "particles.h"

static float randf(){return (float)rand()/(float)RAND_MAX;}

Particles::Particles(int screenHeight)
    : pointSize(30.0f)
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

    for (int i = 0; i < 3000; ++i)
    {
        particlePosition[i] = randf() * 2.0f - 1.0f;
    }

    initParticles();

    return true;
}

void Particles::initParticles()
{
    m_vao.bind();

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(particlePosition, sizeof(particlePosition));
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
    m_program.bind();
    m_program.setUniformValue("mvp_matrix", viewProjection);
    m_program.setUniformValue("size", pointSize * pointSizeScale);
    m_vao.bind();

    glDrawArrays(GL_POINTS, 0, 1000);

    m_vao.release();
    m_program.release();
}

void Particles::resize(int height)
{
    pointSizeScale = (float)height / (float)initHeight;
}
