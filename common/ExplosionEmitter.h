//
// Created by stagakis on 9/12/20.
//

#ifndef VVR_OGL_LABORATORY_EXPLOSIONEMITTER_H
#define VVR_OGL_LABORATORY_EXPLOSIONEMITTER_H
#include "IntParticleEmitter.h"

class ExplosionEmitter : public IntParticleEmitter {
    public:
        ExplosionEmitter(Drawable* _model, int number);
        

        //data member for collision checking
        float height_threshold = 10.0f;

        float t = 0;

        bool checkForCollision(particleAttributes& particle);

        int active_particles = 0; //number of particles that have been instantiated
        void createNewParticle(int index) override;
        void updateParticles(float time, float dt, glm::vec3 camera_pos = glm::vec3(0, 0, 0)) override;
};


#endif //VVR_OGL_LABORATORY_EXPLOSIONEMITTER_H
