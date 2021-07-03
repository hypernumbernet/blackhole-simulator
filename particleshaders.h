#pragma once

#include "g3dmassdiffnbe.h"
#include "g3dmassdiffcorefloat.h"
#include "g3dmassdiffcoredouble.h"
#include "g3dmassintegralnbe.h"
#include "g3dmassintegralcorefloat.h"
#include "g3dmassintegralcoredouble.h"
#include "g3d4dmassdiffnbe.h"
#include "g3d4dmassdiffcorefloat.h"
#include "g3d4dmassdiffcoredouble.h"
#include "computeshaders.h"

#include <math.h>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class ParticleShaders : private QOpenGLFunctions_4_5_Core
{
public:
    explicit ParticleShaders(ThreadAdmin*);
    ~ParticleShaders();

    bool initialize(int screenHeight);
    void paint(const QMatrix4x4& viewProjection);
    void resize(int height);
    void updateGL();
    void setNBodyEngine(const bhs::SimCondition&);
    void setModelScale(double);
    void setModelScaleRatio(double);
    void reset(const bhs::SimCondition&);

private:
    quint64 numberOfParticle() const;
    const void* ssboData() const;
    double modelScale() const;

    ThreadAdmin* const m_threadAdmin;

    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    AbstractNBodyEngine<float>* m_NBodyEngineFloat = nullptr;
    AbstractNBodyEngine<double>* m_NBodyEngineDouble = nullptr;

    // It varies depending on the size of the window.
    float m_pointSizeScale;

    // Display size of particles
    float m_pointSize;

    int m_initHeight;
    bhs::Precision m_precision;

    template <typename T>
    inline const T* coordinates() const
    {
        const T* ret = nullptr;
        if constexpr (std::is_same_v<T, float>) {
            ret = m_NBodyEngineFloat->coordinates();
        }
        if constexpr (std::is_same_v<T, double>) {
            ret = m_NBodyEngineDouble->coordinates();
        }
        return ret;
    }

    template <typename T>
    inline const T* velocities() const
    {
        const T* ret = nullptr;
        if constexpr (std::is_same_v<T, float>) {
            ret = m_NBodyEngineFloat->velocities();
        }
        if constexpr (std::is_same_v<T, double>) {
            ret = m_NBodyEngineDouble->velocities();
        }
        return ret;
    }

    template <typename T>
    inline const void* makeSSBOData() const
    {
        const T* coords = coordinates<T>();
        const T* vels = velocities<T>();

        quint64 num = numberOfParticle();
        quint64 coordsSize = num * 3;
        quint64 velsSize = num * 3;
        quint64 total = coordsSize + velsSize;
        T* data = new T[total];
        for (quint64 i = 0; i < coordsSize; ++i) {
            data[i] = coords[i];
        }
        for (quint64 i = 0; i < velsSize; ++i) {
            data[coordsSize + i] = vels[i];
        }
        return data;
    }
};
