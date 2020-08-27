#ifndef PARTICLEENGINE2D_H
#define PARTICLEENGINE2D_H

#include <vector>

namespace Bengine{

class ParticleBatch2D;
class SpriteBatch;

class ParticleEngine2D
{
    public:
        ParticleEngine2D();
        virtual ~ParticleEngine2D();

        ///After adding a particle batch, the ParticleEngine2D
        ///becomes responsible for its deallocation.
        void addParticleBatch(ParticleBatch2D* particleBatch);
        void update(float deltaTime);
        void draw(SpriteBatch* spriteBatch);

    protected:

    private:
        std::vector<ParticleBatch2D*> m_batches;
};

}

#endif // PARTICLEENGINE2D_H
