#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "threadadmin.h"
#include "engine/3d/engine3d.h"
#include "engine/3d/corefloat3d.h"
#include "engine/3d/coredouble3d.h"
#include "engine/3d4d/engine3d4d.h"
#include "engine/3d4d/corefloat3d4d.h"
#include "engine/3d4d/coredouble3d4d.h"
#include "engine/3d4dr1/engine3d4dr1.h"
#include "engine/3d4dr1/corefloat3d4dr1.h"
#include "engine/3d4dr1/coredouble3d4dr1.h"
#include "engine/4d3d/engine4d3d.h"
#include "engine/4d3d/corefloat4d3d.h"
#include "engine/4d3d/coredouble4d3d.h"
#include "computeshaders.h"

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
    QString particleData();

private:

    struct SSBODataStruct
    {
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
    const T* coordinates() const
    {
        const T* ret = nullptr;
        if constexpr (std::is_same_v<T, float>)
            ret = m_NBodyEngineFloat->coordinates();

        if constexpr (std::is_same_v<T, double>)
            ret = m_NBodyEngineDouble->coordinates();

        return ret;
    }

    template <typename T>
    const T* velocities() const
    {
        const T* ret = nullptr;
        if constexpr (std::is_same_v<T, float>)
            ret = m_NBodyEngineFloat->velocities();

        if constexpr (std::is_same_v<T, double>)
            ret = m_NBodyEngineDouble->velocities();

        return ret;
    }

    template <typename T>
    const T* masses() const
    {
        const T* ret = nullptr;
        if constexpr (std::is_same_v<T, float>)
            ret = m_NBodyEngineFloat->masses();

        if constexpr (std::is_same_v<T, double>)
            ret = m_NBodyEngineDouble->masses();

        return ret;
    }

    template <typename T>
    T timePerFrame() const
    {
        if constexpr (std::is_same_v<T, float>)
            return m_NBodyEngineFloat->timePerFrame();

        if constexpr (std::is_same_v<T, double>)
            return m_NBodyEngineDouble->timePerFrame();
    }

    template <typename T>
    T gravitationalConstant() const
    {
        if constexpr (std::is_same_v<T, float>)
            return m_NBodyEngineFloat->gravitationalConstant();

        if constexpr (std::is_same_v<T, double>)
            return m_NBodyEngineDouble->gravitationalConstant();
    }

    template <typename T>
    double scaleCenterValue() const
    {
        if constexpr (std::is_same_v<T, float>)
            return m_NBodyEngineFloat->scaleInv();

        if constexpr (std::is_same_v<T, double>)
            return m_NBodyEngineDouble->scaleInv();
    }

    template <typename T>
    const void* makeSSBOData(SSBODataStruct& result, int coordinateVectorSize, int velocityVectorSize) const
    {
        quint64 num = numberOfParticle();
        quint64 ssboNum = num % 4 ? num / 4 * 4 + 4 : num;

        qint64 coordinateSize = ssboNum * coordinateVectorSize;
        qint64 velocityOffset = coordinateSize;
        qint64 velocitySize = ssboNum * velocityVectorSize;
        qint64 massOffset = velocityOffset + velocitySize;
        qint64 massSize = ssboNum;
        qint64 paramOffset = massOffset + massSize;
        qint64 paramSize = 8;
        qint64 total = paramOffset + paramSize;

        T* data = new T[total]();

        const T* coords = coordinates<T>();
        for (quint64 i = 0; i < num * coordinateVectorSize; ++i)
        {
            data[i] = coords[i];
        }

        const T* vels = velocities<T>();
        for (quint64 i = 0; i < num * velocityVectorSize; ++i)
        {
            data[velocityOffset + i] = vels[i];
        }

        const T* mss = masses<T>();
        for (quint64 i = 0; i < num; ++i)
        {
            data[massOffset + i] = mss[i];
        }

        double scale = scaleCenterValue<T>();

        data[paramOffset] = timePerFrame<T>();
        data[paramOffset + 1] = (T)numberOfParticle();
        data[paramOffset + 2] = gravitationalConstant<T>();
        data[paramOffset + 3] = T(scale / AbstractNBodyEngine<double>::VANGLE);
        data[paramOffset + 4] = T(0.5 * AbstractNBodyEngine<double>::VANGLE / scale);
        data[paramOffset + 5] = AbstractNBodyEngine<T>::BOUNDARY_TO_INVALIDATE;

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
    const QString particleDataToString() const
    {
        QString ret;
        quint64 num = numberOfParticle();
        const T* cs = nullptr;
        if constexpr (std::is_same_v<T, float>)
        {
            cs = m_NBodyEngineFloat->coordinates();
        }
        if constexpr (std::is_same_v<T, double>)
        {
            cs = m_NBodyEngineDouble->coordinates();
        }
        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            ret.append(QString::number(i));
            ret.append("\t");
            ret.append(QString::number(cs[i3], 'E', 15));
            ret.append("\t");
            ret.append(QString::number(cs[i3 + 1], 'E', 15));
            ret.append("\t");
            ret.append(QString::number(cs[i3 + 2], 'E', 15));
            ret.append("\n");
        }
        return ret;
    }
};
