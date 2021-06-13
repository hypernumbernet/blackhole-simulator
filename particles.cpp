#include "particles.h"

static float randf(){return (float)rand()/(float)RAND_MAX;}

Particles::Particles(int screenHeight)
    : pointSizeScale(1.0f)
    , pointSize(30.0f)
    , initHeight(screenHeight)
    , numberOfParticles(0)
    , timePerFrame(3600.0f)
{
    mass = new float[0];
    coordinates = new float[0];
    velocities = new float[0];
    inversedDistances = new float[0];
}

Particles::~Particles()
{
    m_vao.destroy();
    delete[] mass;
    delete[] coordinates;
    delete[] velocities;
    delete[] inversedDistances;
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

    newParticles();

    //initParticlesRandam();
    initSunEarth();

    updateParticles();
}

void Particles::updateParticles()
{
    if (numberOfParticles == 0)
        return;

    m_vao.bind();

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(coordinates, numberOfParticles * 12);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);

//    QOpenGLBuffer colorBuf;
//    colorBuf.create();
//    colorBuf.bind();
//    colorBuf.allocate(particleColor, sizeof(particleColor));
//    m_program.enableAttributeArray(1);
//    m_program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

    m_vao.release();

    timeProgress();
    calculateInteraction();
}

void Particles::paint(QMatrix4x4 viewProjection)
{
    if (numberOfParticles == 0)
        return;
    m_program.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(1.0e-11f);

    m_program.setUniformValue("mvp_matrix", viewProjection * modelMatrix);
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

//↓別クラスに分離予定=============================================

void Particles::initParticlesRandam()
{
    for (uint64_t i = 0; i < numberOfParticles; ++i)
    {
        mass[i] = randf() * 20000.0f - 10000.0f;
    }
    for (uint64_t i = 0; i < numberOfParticles * 3; ++i)
    {
        coordinates[i] = randf() * 2.0f - 1.0f;
    }
    for (uint64_t i = 0; i < numberOfParticles * 3; ++i)
    {
        velocities[i] = randf() * 0.0000002f - 0.0000001f;
    }
    calculateDistances();
}

void Particles::initSunEarth()
{
    mass[0] = 1.9891e+30f;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0f;
    velocities[0] = velocities[1] = velocities[2] = 0.0f;

    mass[1] = 5.972e+24f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 29780.0f;
    velocities[5] = 0.0f;
}

//↓別クラスに分離予定=============================================

void Particles::calculateDistances()
{
    uint64_t k = 0;
    for (uint64_t i = 0; i < numberOfParticles - 1; ++i)
    {
        for (uint64_t j = i + 1; j < numberOfParticles; ++j)
        {
            inversedDistances[k] = 1.0f / getDistance(i, j);
            ++k;
        }
    }
}

void Particles::newParticles()
{
    delete[] mass;
    delete[] coordinates;
    delete[] velocities;
    delete[] inversedDistances;

    mass = new float[numberOfParticles];
    coordinates = new float[numberOfParticles * 3];
    velocities = new float[numberOfParticles * 3];

    numberOfInteractions = numberOfParticles * (numberOfParticles - 1) / 2;
    inversedDistances = new float[numberOfInteractions];
}

float Particles::getDistance(uint64_t a, uint64_t b)
{
    uint64_t ai = a * 3;
    uint64_t bi = b * 3;
    float d1 = coordinates[ai] - coordinates[bi]; ++ai; ++bi;
    float d2 = coordinates[ai] - coordinates[bi]; ++ai; ++bi;
    float d3 = coordinates[ai] - coordinates[bi];
    return sqrt(d1 * d1 + d2 * d2 + d3 * d3);
}

void Particles::timeProgress()
{
    //debug();
    for (uint64_t i = 0; i < numberOfParticles; ++i)
    {
        uint64_t j = i * 3;
        coordinates[j] += velocities[j] * timePerFrame; ++j;
        coordinates[j] += velocities[j] * timePerFrame; ++j;
        coordinates[j] += velocities[j] * timePerFrame;
    }
}

void Particles::calculateInteraction()
{
    float theta;
    uint64_t k = 0;
    for (uint64_t i = 0; i < numberOfParticles - 1; ++i)
    {
        for (uint64_t j = i + 1; j < numberOfParticles; ++j)
        {
//            theta = inversedDistances[k];
//            inversedDistances[k] = 1.0f / getDistance(i, j);
//            theta -= inversedDistances[k];

            //お互いの距離がほとんど変化しなかった場合、微分で計算し、通常は積分計算をする。
            //すべてを積分計算にすると、計算精度以下になった時動きがなくなる。
            //これを回避したいのなら微分計算で分岐させる。（初期速度をゼロにしたい時）
            //	微分計算の場合
            //	theta = get_distance(i, j);
            //	theta = 1.0 / (theta * theta);
            theta = 1.0f / getDistance(i, j);
            theta *= theta * timePerFrame;

            if (theta == theta)
            {
                theta *= GRAVITATIONAL_CONSTANT;

                uint64_t a = i * 3;
                uint64_t b = j * 3;
                float d1 = coordinates[a] - coordinates[b];
                float d2 = coordinates[a + 1] - coordinates[b + 1];
                float d3 = coordinates[a + 2] - coordinates[b + 2];
                float norm = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                d1 *= theta / norm;
                d2 *= theta / norm;
                d3 *= theta / norm;

                velocities[a] -= d1 * mass[j];
                velocities[a + 1] -= d2 * mass[j];
                velocities[a + 2] -= d3 * mass[j];
                velocities[b] += d1 * mass[i];
                velocities[b + 1] += d2 * mass[i];
                velocities[b + 2] += d3 * mass[i];

            } else {
                qDebug("[warning] invalid number detected");
            }

            ++k;
        }
    }
}

void Particles::debug()
{
    for (uint64_t i = 0; i < numberOfParticles * 3; ++i)
    {
        qDebug("coordinates: %lu: %f", i, coordinates[i]);
    }
    for (uint64_t i = 0; i < numberOfParticles * 3; ++i)
    {
        qDebug("velocities: %lu: %f", i, velocities[i]);
    }
}
