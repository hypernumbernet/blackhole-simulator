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

    struct SSBODataStruct {
        const void* data;
        qint64 total;
        qint64 coordinateOffset;
        qint64 coordinateSize;
        qint64 velocityOffset;
        qint64 velocitySize;
        GLenum precision;
        int dataSize;
    };

    quint64 numberOfParticle() const;
    const void* ssboData(const SSBODataStruct&) const;
    double modelScale() const;
    void GetSSBOStruct(SSBODataStruct&, int, int) const;

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
    inline const void* makeSSBOData(const SSBODataStruct& ssboStruct) const
    {
        T* data = new T[ssboStruct.total];

        const T* coords = coordinates<T>();
        for (qint64 i = 0; i < ssboStruct.coordinateSize; ++i) {
            data[i] = coords[i];
        }

        const T* vels = velocities<T>();
        for (qint64 i = 0; i < ssboStruct.velocitySize; ++i) {
            data[ssboStruct.coordinateSize + i] = vels[i];
        }
        return data;
    }
};
