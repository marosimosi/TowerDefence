#include "ExplosionEmitter.h"
#include <iostream>
#include <algorithm>

ExplosionEmitter::ExplosionEmitter(Drawable *_model, int number) : IntParticleEmitter(_model, number) {}

void ExplosionEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos) {

    //This is for the fountain to slowly increase the number of its particles to the max amount
    //instead of shooting all the particles at once
    if (active_particles < number_of_particles) {
        int batch = 800;
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

        if(particle.position.y < emitter_pos.y - 10.0f || particle.life == 0.0f || checkForCollision(particle)){
            createNewParticle(i);
        }

        if (particle.position.y > height_threshold)
            createNewParticle(i);

        //particle.accel = glm::vec3(-particle.position.x, 0.0f, -particle.position.z); //gravity force

        //particle.rot_angle += 90*dt; 

        particle.position = particle.position + particle.velocity*dt + particle.accel*(dt*dt)*0.5f;
        particle.velocity = particle.velocity + particle.accel*dt;

        //*
        auto bill_rot = calculateBillboardRotationMatrix(particle.position, camera_pos);
        particle.rot_axis = glm::vec3(bill_rot.x, bill_rot.y, bill_rot.z);
        particle.rot_angle = glm::degrees(bill_rot.w);
        //*/
        particle.dist_from_camera = length(particle.position - camera_pos);
        particle.life = (height_threshold - particle.position.y) / (height_threshold - emitter_pos.y);


        if (particle.life < 0.5f) { particle.accel = glm::vec3(-2.0f * (RAND - RAND), -70.0f, +2.0f * (RAND - RAND)); }

    }
}

bool ExplosionEmitter::checkForCollision(particleAttributes& particle)
{
    return particle.position.y < 0.0f;
}

void ExplosionEmitter::createNewParticle(int index){
    particleAttributes & particle = p_attributes[index];

    particle.rot_angle = RAND * 360;
    particle.position = emitter_pos + glm::vec3((4 + 6*RAND) * sin(particle.rot_angle), 0, (4 + 6 * RAND) * cos(particle.rot_angle));
    particle.velocity = glm::vec3(0, 7 * RAND, 0) * 10.0f; // *10.0f; 1 - RAND * 2

    particle.mass = 0.6f;
    particle.rot_axis = glm::normalize(glm::vec3(1 - 2*RAND, 1 - 2*RAND, 1 - 2*RAND));
    particle.accel = glm::vec3(0.0f, 0.0f, 0.0f); //gravity force
    particle.life = 1.0f; //mark it alive
}
