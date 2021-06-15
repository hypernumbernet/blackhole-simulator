#include "worldmodels.h"

WorldModels::WorldModels()
    : m_enableGridLines(true)
    , m_lineType(0)
{
    linesXZMeshes();
}

void WorldModels::appendLine(QVector3D start, QVector3D end, QVector3D color)
{
    m_lines.append(start);
    m_lines.append(end);
    m_colors.append(color);
    m_colors.append(color);
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

//    auto len = m_lines.length();
//    auto sizev3d = sizeof(QVector3D);

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(m_lines.constData(), m_lines.length() * sizeof(QVector3D));
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    QOpenGLBuffer colorBuf;
    colorBuf.create();
    colorBuf.bind();
    colorBuf.allocate(m_colors.constData(), m_lines.length() * sizeof(QVector3D));
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

    m_vao.release();
}

void WorldModels::paint(const QMatrix4x4& viewProjection)
{
    if (m_enableGridLines) {
        m_program.bind();
        m_program.setUniformValue("mvp_matrix", viewProjection);
        m_vao.bind();

        //glLineWidth(5.0f);
        glDrawArrays(GL_LINES, 0, m_lines.length() * sizeof(QVector3D));

        m_vao.release();
        m_program.release();
    }
}

void WorldModels::enableGridLines(const bool enabled)
{
    m_enableGridLines = enabled;
}

void WorldModels::changeLineType()
{
    m_lines.clear();
    m_colors.clear();
    ++m_lineType;
    if (m_lineType >= 3)
        m_lineType = 0;
    switch (m_lineType) {
    case 0:
        linesXZMeshes();
        break;
    case 1:
        linesAxis();
        break;
    case 2:
        lines2Meshes();
        break;
    }
    initGridLines();
}

void WorldModels::linesAxis()
{
    appendLine({0, 0, 0}, {1, 0, 0}, RED);
    appendLine({0, 0, 0}, {0, 1, 0}, GREEN);
    appendLine({0, 0, 0}, {0, 0, 1}, BLUE);
}

void WorldModels::lines2Meshes()
{
    for (int i = 0; i < 3; ++i) {
        appendLine({-1, -1, i - 1.0f}, {1, -1, i - 1.0f}, RED);
        appendLine({i - 1.0f, -1, -1}, {i - 1.0f, 1, -1}, GREEN);
        appendLine({-1, i - 1.0f, -1}, {-1, i - 1.0f, 1}, BLUE);
    }
    for (int i = 1; i < 3; ++i) {
        appendLine({i - 1.0f, -1, -1}, {i - 1.0f, -1, 1}, RED);
        appendLine({-1, i - 1.0f, -1}, {1, i - 1.0f, -1}, GREEN);
        appendLine({-1, -1, i - 1.0f}, {-1, 1, i - 1.0f}, BLUE);
    }
}

void WorldModels::linesXZMeshes()
{
    for (int i = -10; i <= 10; ++i) {
        appendLine({-10, 0, (float)i}, {10, 0, (float)i}, RED);
        appendLine({(float)i, 0, -10}, {(float)i, 0, 10}, BLUE);
    }
    appendLine({0, 0, 0}, {0, 1, 0}, GREEN);
}
