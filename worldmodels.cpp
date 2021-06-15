#include "worldmodels.h"

WorldModels::WorldModels()
    : m_enableGridLines(true)
    , lines{
          0.0f, 0.0f, 0.0f,
          10.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
          0.0f, 10.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 10.0f,
      }
{
}

WorldModels::~WorldModels()
{
    m_vao.destroy();
}

bool WorldModels::initialize()
{
    if (!initializeOpenGLFunctions()) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/world.vert")) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/world.frag")) {
        return false;
    }
    if (!m_program.link()) {
        return false;
    }
    if (!m_vao.create()) {
        return false;
    }
    initGridLines();
    return true;
}

void WorldModels::initGridLines()
{
    m_vao.bind();

    static const GLfloat color_lines[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(lines, sizeof(lines));
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    QOpenGLBuffer colorBuf;
    colorBuf.create();
    colorBuf.bind();
    colorBuf.allocate(color_lines, sizeof(color_lines));
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

    m_vao.release();
}

void WorldModels::paint(const QMatrix4x4& viewProjection)
{
    m_program.bind();
    m_program.setUniformValue("mvp_matrix", viewProjection);
    m_vao.bind();

    if (m_enableGridLines) {
        //glLineWidth(5.0f);
        glDrawArrays(GL_LINES, 0, 6 * 3 * sizeof(GLfloat));
    }

    m_vao.release();
    m_program.release();
}

void WorldModels::enableGridLines(const bool enabled)
{
    m_enableGridLines = enabled;
}

void WorldModels::changeLineType()
{
    m_vao.bind();

    if (lines[0] == 0.0f) {
        lines[0] = -10.0f;
        lines[7] = -10.0f;
        lines[14] = -10.0f;
    } else {
        lines[0] = 0.0f;
        lines[7] = 0.0f;
        lines[14] = 0.0f;
    }

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(lines, sizeof(lines));
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_vao.release();
}
