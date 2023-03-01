#include "ExplosionEmitter.h"
#include <iostream>
#include <algorithm>

ExplosionEmitter::ExplosionEmitter(Drawable *_model, int number) : IntParticleEmitter(_model, number) {}

void ExplosionEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos) {

    //shoot all the particles at once
    if (active_particles < number_of_particles) {
        int batch = 1600;
        for (int i = 0; i < batch; i++) {
            createNewParticle(active_particles);
            active_particles++;
        }
    }
    else {
        active_particles = number_of_particles; //In case we resized our ermitter to a smaller particle number
    }

    for(int i = 0; i < active_particles; i++){
        particleAttributes & particle = p_attributes[i];

        if(particle.position.y > height_threshold || particle.life <= 0.0f || checkForCollision(particle)){
            particle.turn = 4;
            createNewParticle(i);
        }

        // MUSHROOM
        if (particle.life > 0.5f && particle.life <= 1.0f) {
            particle.accel = glm::vec3(0, 20 * RAND, 0);
            particle.velocity = particle.velocity + particle.accel * dt;
        }
        else if (particle.life <= 0.5f && particle.life > 0.1f) {
            particle.accel = glm::vec3(0, 5 + 2 * RAND, 0);
            particle.velocity = glm::vec3(0, RAND, 0);
            particle.velocity = particle.velocity + particle.accel * dt;
        }
        else if (particle.life <= 0.1f && particle.life > 0.0f) {
            particle.accel = glm::vec3(0, 0, 0);
            particle.velocity = glm::vec3(0, RAND, 0);
        }

        if (particle.position.y > height_threshold - 2) {
            particle.accel = glm::vec3(0, 0, 0);
            particle.velocity += glm::vec3(-8.0f * (RAND - RAND),0, 8.0f * (RAND - RAND));
            particle.mass = 0.6f;
        }

        particle.position = particle.position + particle.velocity*dt + particle.accel*(dt*dt)*0.5f;

        //*
        auto bill_rot = calculateBillboardRotationMatrix(particle.position, camera_pos);
        particle.rot_axis = glm::vec3(bill_rot.x, bill_rot.y, bill_rot.z);
        particle.rot_angle = glm::degrees(bill_rot.w);
        //*/
        particle.dist_from_camera = length(particle.position - camera_pos);
        particle.life = (height_threshold - particle.position.y) / (height_threshold - emitter_pos.y);


        if (particle.life < 0.0f) {
            particle.mass = 0;
        }
    }
}

bool ExplosionEmitter::checkForCollision(particleAttributes& particle)
{
    return particle.position.y < 0.0f;
}

void ExplosionEmitter::createNewParticle(int index){
    particleAttributes & particle = p_attributes[index];

    // 4 different explosions
    if (index >= 0 && index < 400 && particle.turn != 4) particle.turn = 0;
    else if (index >= 400 && index < 800 && particle.turn != 4) particle.turn = 1;
    else if (index >= 800 && index < 1200 && particle.turn != 4) particle.turn = 2;
    else if (index >= 1200 && index < 1600 && particle.turn != 4) particle.turn = 3;

    particle.rot_angle = RAND * 360;
    particle.mass = 0.6f;
    particle.velocity = glm::vec3(0, 5 + 150 * RAND, 0);

    if (particle.turn == 0) {
        particle.position = emitter_pos + glm::vec3((RAND - RAND) - 7, 0, (RAND - RAND))+ glm::vec3(0, 0, 7);
    }
    else if (particle.turn == 1) {
        particle.position = emitter_pos + glm::vec3((RAND - RAND), 0, (RAND - RAND) + 7) + glm::vec3(7, 0, 0);
    }
    else if (particle.turn == 2) {
        particle.position = emitter_pos + glm::vec3((RAND - RAND) + 7, 0, (RAND - RAND)) + glm::vec3(0, 0, -7);
    }
    else if (particle.turn == 3) {
        particle.position = emitter_pos + glm::vec3((RAND - RAND), 0, (RAND - RAND) - 7) + glm::vec3(-7, 0, 0);
    }
    else if (particle.turn == 4) {
        particle.mass = 0;
    }

    particle.rot_axis = glm::normalize(glm::vec3(1 - 2*RAND, 1 - 2*RAND, 1 - 2*RAND));
    particle.accel = glm::vec3(0.0f, 1.0f, 0.0f); 
    particle.life = 1.0f; //mark it alive
}
