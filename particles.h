#pragma once

#include <math.h>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Particles : public QOpenGLFunctions_3_3_Core
{
public:
    Particles(int screenHeight);
    ~Particles();

    bool initialize();
    void paint(QMatrix4x4 viewProjection);
    void resize(int height);
    void updateParticles();
    void setNumberOfParticles(int num);

    static constexpr float GRAVITATIONAL_CONSTANT = 6.6743e-11f; // 2018 CODATA
    static constexpr float SPEED_OF_LIGHT = 2.99792458e+8f;

private:

    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    float pointSizeScale;
    float pointSize;
    int initHeight;
    uint64_t numberOfParticles;
    uint64_t numberOfInteractions;

    //↓別クラスに分離予定=============================================
    void initParticlesRandam();
    void initSunEarth();

    //↓別クラスに分離予定=============================================
    void newParticles();
    float getDistance(uint64_t a, uint64_t b);
    void calculateDistances();
    void timeProgress();
    void calculateInteraction();
    void debug();

    // 1フレームあたりの物理計算上の時間 (second)
    float timePerFrame;

    // 質量 (kg)
    float* mass;

    // 粒子の座標 (m)
    float* coordinates;

    // 粒子の速度
    float* velocities;

    // 前回フレームの距離の逆数
    float* inversedDistances;
};
