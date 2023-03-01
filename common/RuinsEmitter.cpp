#include "RuinsEmitter.h"
#include <iostream>
#include <algorithm>


RuinsEmitter::RuinsEmitter(Drawable *_model, int number) : IntParticleEmitter(_model, number) {}

void RuinsEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos) {

    //This is for the fountain to slowly increase the number of its particles to the max amount
    //instead of shooting all the particles at once
    if (active_particles < number_of_particles) {
        int batch = 5;
        int limit = std::min(number_of_particles - active_particles, batch);
        for (int i = 0; i < limit; i++) {
            createNewParticle(active_particles);
            active_particles++;
        }
    }
    else {
        active_particles = number_of_particles; //In case we resized our ermitter to a smaller particle number
    }

    for(int i = 0; i < active_particles; i++){
        particleAttributes & particle = p_attributes[i];

        particle.position = particle.position + particle.velocity*dt + particle.accel*(dt*dt)*0.5f;
        particle.velocity = particle.velocity + particle.accel*dt;


        if (particle.position.y < 0.5f && ( abs(particle.position.x) > 0.2f || abs(particle.position.z) > 0.2f) ) {
            particle.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            particle.accel = glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }
}

bool RuinsEmitter::checkForCollision(particleAttributes& particle)
{
    return particle.position.y < 0.0f;
}

void RuinsEmitter::createNewParticle(int index){
    particleAttributes & particle = p_attributes[index];

    particle.position = emitter_pos - glm::vec3(0, 3 * RAND, 0);
    particle.velocity = glm::vec3(3 - RAND * 6, 1 - RAND * 2 , 3 - RAND * 6) * 4.0f;

    particle.mass = 0.6 * RAND;
    particle.rot_axis = glm::normalize(glm::vec3(1 - 2*RAND, 1 - 2*RAND, 1 - 2*RAND));
    particle.accel = glm::vec3(0.0f, -9.8f, 0.0f); //gravity force
    particle.rot_angle = RAND*360;
    particle.life = 1.0f; //mark it alive
}
