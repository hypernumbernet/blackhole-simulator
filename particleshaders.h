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
    quint64 numberOfParticle() const;

private:

    struct SSBODataStruct {
        const void* data;
        qint64 total;
        qint64 coordinateOffset;
        qint64 coordinateSize;
        qint64 velocityOffset;
        qint64 velocitySize;
        qint64 massOffset;
        qint64 massSize;
        qint64 paramOffset;
        qint64 paramSize;
        GLenum precision;
        int dataSize;
    };

    const void* ssboData(SSBODataStruct& result, int coordinateVectorSize, int velocityVectorSize) const;
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
    inline const T* masses() const
    {
        const T* ret = nullptr;
        if constexpr (std::is_same_v<T, float>) {
            ret = m_NBodyEngineFloat->masses();
        }
        if constexpr (std::is_same_v<T, double>) {
            ret = m_NBodyEngineDouble->masses();
        }
        return ret;
    }

    template <typename T>
    inline const void* makeSSBOData(SSBODataStruct& result, int coordinateVectorSize, int velocityVectorSize) const
    {
        quint64 num = numberOfParticle();
        quint64 ssboNum = num % 4 ? num / 4 * 4 + 4 : num;

        //qint64 coordinateOffset = 0;
        //qint64 coordinateSize = ssboNum * 4;
        qint64 coordinateSize = ssboNum * 3;
        qint64 velocityOffset = coordinateSize;
        //qint64 velocitySize = ssboNum * 4;
        qint64 velocitySize = ssboNum * 3;
        qint64 massOffset = velocityOffset + velocitySize;
        qint64 massSize = ssboNum;
        qint64 paramOffset = massOffset + massSize;
        qint64 paramSize = 2;
        qint64 total = paramOffset + paramSize;

        T* data = new T[total]();

        const T* coords = coordinates<T>();
//        if (coordinateVectorSize == 3) {
//            for (quint64 i = 0; i < num; ++i) {
//                int i3 = i * 3;
//                int i4 = i * 4;
//                data[i4 + 3] = coords[i3 + 0];
//                data[i4 + 2] = coords[i3 + 1];
//                data[i4 + 1] = coords[i3 + 2];
//                data[i4 + 0] = (T)0.0;
//            }
//        } else {
//            for (quint64 i = 0; i < num * 4; ++i) {
//                data[i] = coords[i];
//            }
//        }
        for (quint64 i = 0; i < num * 3; ++i) {
            data[i] = coords[i];
        }

        const T* vels = velocities<T>();
//        if (velocityVectorSize == 3) {
//            for (quint64 i = 0; i < num; ++i) {
//                int i3 = i * 3;
//                int i4 = velocityOffset + i * 4;
//                data[i4++] = vels[i3++];
//                data[i4++] = vels[i3++];
//                data[i4++] = vels[i3++];
//                data[i4] = (T)0.0;
//            }
//        } else {
//            for (quint64 i = 0; i < num * 4; ++i) {
//                data[velocityOffset + i] = vels[i];
//            }
//        }
        for (quint64 i = 0; i < num * 3; ++i) {
            data[velocityOffset + i] = vels[i];
        }

        const T* mss = masses<T>();
        for (quint64 i = 0; i < num; ++i) {
            data[massOffset + i] = mss[i];
        }

        data[paramOffset] = timePerFrame<T>();
        data[paramOffset + 1] = (T)numberOfParticle();

        result.coordinateSize = coordinateSize;
        result.velocityOffset = velocityOffset;
        result.velocitySize = velocitySize;
        result.massOffset = massOffset;
        result.massSize = massSize;
        result.paramOffset = paramOffset;
        result.paramSize = paramSize;
        result.total = total;

        return data;
    }

    template <typename T>
    inline T timePerFrame() const
    {
        if constexpr (std::is_same_v<T, float>) {
            return m_NBodyEngineFloat->timePerFrame();
        }
        if constexpr (std::is_same_v<T, double>) {
            return m_NBodyEngineDouble->timePerFrame();
        }
    }
};
