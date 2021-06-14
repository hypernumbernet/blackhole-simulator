#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class WorldModels : public QOpenGLFunctions_3_3_Core
{
public:
    WorldModels();
    ~WorldModels();

    bool initialize();
    void paint(QMatrix4x4 viewProjection);
    void enableGridLines(bool enabled);
    void changeLineType();

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;
    void initGridLines();
    bool m_enableGridLines;

    GLfloat lines[18];
};
