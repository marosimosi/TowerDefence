// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <common/animation.h>
#include "common/FireEmitter.h"
#include "common/ExplosionEmitter.h"
#include "common/RuinsEmitter.h"

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>
#include <common/light.h> 
#include <tower/Stone.h>
#include <tower/Floor.h>
#include <tower/Tower.h>
#include <tower/Spider.h>
#include <tower/Snake.h>


using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1800
#define W_HEIGHT 1000
#define TITLE "TOWER DEFENCE"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

int loopNum = 0;




// Creating a structure to store the material parameters of an object
struct Material {
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
	float Ns;
};


// Global Variables
GLFWwindow* window;
Camera* camera;
Light* light;
GLuint shaderProgram, depthProgram, miniMapProgram;
GLuint fireProgram, explosionProgram, ruinsProgram;
Stone* stone;
Floor* plane;
Tower* tower;
Spider* spider;
Snake* snake;
Mountain* mountain;
GLuint depthFrameBuffer, depthTexture;
Drawable* quad;

//To end the game
bool stop = false;



//Initialize dragon variables
bool dragonAttack = false;
bool dragonRun = false;
int dragonRunFirstLoop;
int dragonAttackFirstLoop;
float dragonDamage = 1.5f;

// animation
Animation* first_animation;
GLuint assimp_shader;
GLuint model_mat_location, view_mat_location, proj_mat_location;
GLuint bone_matrices_locations[MAX_BONES];
GLuint textureSampler;



// locations for shaderProgram
GLuint viewMatrixLocation;
GLuint projectionMatrixLocation;
GLuint modelMatrixLocation;
GLuint KaLocation, KdLocation, KsLocation, NsLocation;
GLuint LaLocation, LdLocation, LsLocation;
GLuint lightPositionLocation;
GLuint lightPowerLocation;
GLuint diffuseColorSampler; 
GLuint specularColorSampler;
GLuint useTextureLocation;
GLuint depthMapSampler;
GLuint lightVPLocation;
GLuint lightDirectionLocation;
GLuint lightFarPlaneLocation;
GLuint lightNearPlaneLocation;
GLuint offsets, shadowOffsets;
GLuint diffuseTexture;
GLuint specularTexture;


// locations for depthProgram
GLuint shadowViewProjectionLocation; 
GLuint shadowModelLocation;

//locations for fireProgram
GLuint fireProjAndView;
GLuint fireSampler;
GLuint fireTexture;

//locations for explosionProgram
GLuint explosionProjAndView;
GLuint explosionSampler;
GLuint explosionTexture;

//locations for ruinsProgram
GLuint ruinsProjAndView;
GLuint ruinsSampler;
GLuint ruinsTexture;



// Create material
const Material bone{
	vec4{ 0.227, 0.218, 0.201, 1 },
	vec4{ 0.9, 0.829, 0.829, 1 },
	vec4{ 0.296648, 0.296648, 0.296648, 1 },
	11.264f
};


//upload the light parameters to the shader program
void uploadLight(const Light& light) {
	glUniform4f(LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
	glUniform4f(LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
	glUniform4f(LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
	glUniform3f(lightPositionLocation, light.lightPosition_worldspace.x,
		light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
	glUniform1f(lightPowerLocation, light.power);
}


//upload the material parameters of a model to the shader program
void uploadMaterial(const Material& mtl) {
	glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
	glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
	glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
	glUniform1f(NsLocation, mtl.Ns);
}





void createContext() {

	shaderProgram = loadShaders("shaders/ShadowMapping.vertexshader", "shaders/ShadowMapping.fragmentshader");
	depthProgram = loadShaders("shaders/Depth.vertexshader", "shaders/Depth.fragmentshader");
	fireProgram = loadShaders("shaders/Fire.vertexshader", "shaders/Fire.fragmentshader");
	explosionProgram = loadShaders("shaders/Explosion.vertexshader", "shaders/Explosion.fragmentshader");
	ruinsProgram = loadShaders("shaders/Ruins.vertexshader", "shaders/Ruins.fragmentshader");

	// Get pointers to uniforms

	// --- shaderProgram ---
	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
	viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
	// for phong lighting
	KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
	KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
	KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
	NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");
	LaLocation = glGetUniformLocation(shaderProgram, "light.La");
	LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
	LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
	lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
	lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
	diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
	specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");
	useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture"); 
	// locations for shadow rendering
	depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
	lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");
	//instancing
	offsets = glGetUniformLocation(shaderProgram, "offsets");
	
	// --- depthProgram ---
	shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
	shadowModelLocation = glGetUniformLocation(depthProgram, "M");
	shadowOffsets = glGetUniformLocation(depthProgram, "offsets");

	// ---fireProgram ---
	fireProjAndView = glGetUniformLocation(fireProgram, "PV");
	fireSampler = glGetUniformLocation(fireProgram, "texture0");
	fireTexture = loadSOIL("fire3.png");

	// ---explosionProgram
	explosionProjAndView = glGetUniformLocation(explosionProgram, "PV");
	explosionSampler = glGetUniformLocation(explosionProgram, "texture0");
	explosionTexture = loadSOIL("smoke.jpg");

	// ---ruinsProgram ---
	ruinsProjAndView = glGetUniformLocation(ruinsProgram, "PV");
	ruinsSampler = glGetUniformLocation(ruinsProgram, "texture0");
	ruinsTexture = loadSOIL("ruins.jpg");



	// Load models
	stone = new Stone();
	plane = new Floor();
	tower = new Tower();
	spider = new Spider();
	snake = new Snake();
	mountain = new Mountain();
	


	//  ANIMATION
	first_animation = new Animation("models/dragon2.dae");
	first_animation->loadTexture("dragondif.bmp");

	assimp_shader = loadShaders("shaders/assimp.vertexshader", "shaders/assimp.fragmentshader");
	model_mat_location = glGetUniformLocation(assimp_shader, "model");
	view_mat_location = glGetUniformLocation(assimp_shader, "view");
	proj_mat_location = glGetUniformLocation(assimp_shader, "proj");

	char name[64];
	for (int i = 0; i < MAX_BONES; i++) {
		sprintf(name, "bone_matrices[%i]", i);
		bone_matrices_locations[i] = glGetUniformLocation(assimp_shader, name);
		glUniformMatrix4fv(bone_matrices_locations[i], 1, GL_FALSE, identity_mat4().m);
	}


	// ---------------------------------------------------------------------------- //
	// -  Create a depth framebuffer and a texture to store the depthmap - //
	// ---------------------------------------------------------------------------- //
	//*/
	// Tell opengl to generate a framebuffer
	glGenFramebuffers(1, &depthFrameBuffer);
	// Binding the framebuffer, all changes bellow will affect the binded framebuffer
	// **Don't forget to bind the default framebuffer at the end of initialization
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);


	// We need a texture to store the depth image
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	// Telling opengl the required information about the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);							
	

	//*/Continue attaching the texture to the framebuffer, so that it will monitor the depth component
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	

	// Since the depth buffer is only for the generation of the depth texture, 
	// there is no need to have a color output
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	// Finally, we have to always check that our frame buffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glfwTerminate();
		throw runtime_error("Frame buffer not initialized correctly");
	}

	// Binding the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//*/

}


void free() {
	// Delete Shader Programs
	glDeleteProgram(shaderProgram);
	glDeleteProgram(depthProgram);
	glDeleteProgram(fireProgram);
	glDeleteProgram(explosionProgram);
	glDeleteProgram(ruinsProgram);
	glDeleteProgram(assimp_shader);


	glfwTerminate();
}


void depth_pass(mat4 viewMatrix, mat4 projectionMatrix) {


	// Setting viewport to shadow map size
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// Binding the depth framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);
	// Cleaning the framebuffer depth information (stored from the last render)
	glClear(GL_DEPTH_BUFFER_BIT);
	// Selecting the new shader program that will output the depth component
	glUseProgram(depthProgram);
	// sending the view and projection matrix to the shader
	mat4 view_projection = projectionMatrix * viewMatrix;
	glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);


	// ---- rendering the scene ---- //
	//STONE
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &stone->modelMatrix[0][0]);
	stone->draw();
	
	//PLANE
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &plane->modelMatrix[0][0]);
	plane->draw();

	//TOWER
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &tower->modelMatrix[0][0]);
	tower->draw();

	//MOUNTAIN
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &mountain->modelMatrix[0][0]);
	mountain->draw();

	//SPIDER
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &spider->modelMatrix[0][0]);
	glUniformMatrix4fv(shadowOffsets, 2, GL_FALSE, &spider->instancing[0][0][0]);
	spider->draw();

	//SNAKE
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &snake->modelMatrix[0][0]);
	snake->draw();


	//UNDO translations for instancing
	mat4 undoOffsets[3] = { mat4(1), mat4(1), mat4(1) };
	glUniformMatrix4fv(shadowOffsets, 1, GL_FALSE, &undoOffsets[0][0][0]);

	// binding the default framebuffer again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix) {

	// Binding a frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, W_WIDTH, W_HEIGHT);
	// Clearing color and depth info
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Selecting shader program
	glUseProgram(shaderProgram);

	// Making view and projection matrices uniform to the shader program
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// uploading the light parameters to the shader program
	uploadLight(*light);

	// Sending the shadow texture to the shaderProgram
	glActiveTexture(GL_TEXTURE0);								
	glBindTexture(GL_TEXTURE_2D, depthTexture);			
	glUniform1i(depthMapSampler, 0);

	// Sending the light View-Projection matrix to the shader program
	mat4 light_VP = light->lightVP();
	glUniformMatrix4fv(lightVPLocation, 1, GL_FALSE, &light_VP[0][0]);





	// ----------------------------------------------------------------- //
	// --------------------- Drawing scene objects --------------------- //	
	// ----------------------------------------------------------------- //

	//STONE
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &stone->modelMatrix[0][0]);
      
	glActiveTexture(GL_TEXTURE1);								
	glBindTexture(GL_TEXTURE_2D, stone->diffuseTexture);			
	glUniform1i(diffuseColorSampler, 1);						
	
	glActiveTexture(GL_TEXTURE2);								
	glBindTexture(GL_TEXTURE_2D, stone->specularTexture);			
	glUniform1i(specularColorSampler, 2);						

	glUniform1i(useTextureLocation, 1);

	stone->draw();



	//FLOOR
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &plane->modelMatrix[0][0]);
	glActiveTexture(GL_TEXTURE1);								
	glBindTexture(GL_TEXTURE_2D, plane->diffuseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glUniform1i(diffuseColorSampler, 1);						

	glActiveTexture(GL_TEXTURE2);								
	glBindTexture(GL_TEXTURE_2D, plane->specularTexture);			
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	plane->draw();

	//TOWER
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &tower->modelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tower->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tower->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	tower->draw();


	
	//MOUNTAIN
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mountain->modelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mountain->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mountain->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	mountain->draw();


	//SPIDER
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &spider->modelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, spider->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, spider->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	glUniformMatrix4fv(offsets, 2, GL_FALSE, &spider->instancing[0][0][0]);

	spider->draw();

	//SNAKE
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &snake->modelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, snake->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, snake->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	snake->draw();


	//UNDO translations for instancing
	mat4 undoOffsets[3] = { mat4(1), mat4(1), mat4(1) };

	glUniformMatrix4fv(offsets, 1, GL_FALSE, &undoOffsets[0][0][0]);
}



void mainLoop() {

	light->update();
	mat4 light_proj = light->projectionMatrix;
	mat4 light_view = light->viewMatrix;

	double anim_time = 0.0;

	// initialize emitters to avoid errors, will override later
	auto* quad = new Drawable("models/quad.obj");
	FireEmitter* f_emitter = new FireEmitter(quad, 300);
	f_emitter->emitter_pos = vec3(-3.7, 10.2, -3.7);

	ExplosionEmitter* e_emitter = new ExplosionEmitter(quad, 1600);
	e_emitter->emitter_pos = vec3(0.0, 0.0, 0.0);

	// initialize emitter
	auto* rock = new Drawable("models/rock3.obj");
	RuinsEmitter* r_emitter = new RuinsEmitter(rock, 1000);
	r_emitter->emitter_pos = vec3(0.0, 3.0, 0.0);

	float t = glfwGetTime();
	float shake = 0.1f;

	do {
		loopNum += 1;


		// ANIMATION TIMER
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		anim_time += elapsed_seconds * 600.0;
		if (anim_time >= first_animation->anim_duration) {
			anim_time = first_animation->anim_duration - anim_time;
		}

		light->update();
		mat4 light_proj = light->projectionMatrix;
		mat4 light_view = light->viewMatrix;

		// Create the depth buffer
		depth_pass(light_view, light_proj);

		// Getting camera information
		camera->update();
		mat4 projectionMatrix = camera->projectionMatrix;
		mat4 viewMatrix = camera->viewMatrix;

		float currentTime = glfwGetTime();
		float dt = currentTime - t;

		lighting_pass(viewMatrix, projectionMatrix);


		// ANIMATION
		glEnable(GL_DEPTH_TEST);
		glUseProgram(assimp_shader);
		glActiveTexture(GL_TEXTURE0);
		first_animation->bindTexture();
		glUniform1i(textureSampler, 0);
		first_animation->bind();
		first_animation->skeleton_animate(first_animation->root_node, anim_time,
			identity_mat4(), first_animation->bone_offset_matrices, first_animation->bone_animation_mats);
		first_animation->update();
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &first_animation->modelMatrix[0][0]);
		glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &camera->viewMatrix[0][0]);
		glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &camera->projectionMatrix[0][0]);
		glUniformMatrix4fv(bone_matrices_locations[0], first_animation->bone_count,
			GL_FALSE, first_animation->bone_animation_mats[0].m);
		first_animation->draw();


		// ----------POLL KEYS--------------------------------------------------------------------------------------// 
		if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_RELEASE) {
			if (!spider->Run && !spider->Attack) {
				spider->runFirstLoop = loopNum;
				spider->Run = true;
				spider->modelMatrix = spider->modelMatrix * rotate(mat4(), radians(-5.0f), vec3(1, 0, 1));
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_RELEASE) {
			if (!snake->Run && !snake->Attack) {
				snake->runFirstLoop = loopNum;
				snake->Run = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_RELEASE) {
			if (!stone->Run && !stone->Attack) {
				stone->runFirstLoop = loopNum;
				stone->Run = true;
				stone->modelMatrix = stone->modelMatrix * rotate(mat4(), radians(-20.0f), vec3(0, 1, 0));
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_RELEASE) {
			if (!dragonAttack) {
				dragonAttack = true;
				dragonAttackFirstLoop = loopNum;
				//override emitter
				f_emitter = new FireEmitter(quad, 1600);
				f_emitter->emitter_pos = vec3(-8.7, 10.2, -8.7);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_RELEASE) {
			if (!tower->Attack) {
				tower->Attack = true;
				tower->attackFirstLoop = loopNum;
				//override emitter
				e_emitter = new ExplosionEmitter(quad, 1600);
				e_emitter->emitter_pos = vec3(0.0, 0.0, 0.0);
			}
		}


		// ----------CHECK RUN--------------------------------------------------------------------------------------// 
		if (spider->Run == true) {
			spider->run(loopNum);
		}
		if (snake->Run == true) {
			snake->run(loopNum);
		}
		if (stone->Run == true) {
			stone->run(loopNum);
		}


		// ----------CHECK ATTACK--------------------------------------------------------------------------------------// 
		//snake
		if (snake->Attack == true) {
			snake->attack(loopNum);
			tower->snakeAttack(loopNum, snake->attackFirstLoop);
			tower->hp -= snake->damage;
		}
		//stone
		if (stone->Attack == true) {
			stone->attack(loopNum);
			tower->stoneAttack(loopNum);
			tower->hp -= stone->damage;
		}
		//spider
		if (spider->Attack == true) {
			spider->attack(loopNum);
			tower->spiderAttack(loopNum, spider->attackFirstLoop);
			tower->hp -= spider->damage;
		}
		//dragon
		auto PV = projectionMatrix * viewMatrix;
		glUseProgram(fireProgram);
		glUniformMatrix4fv(fireProjAndView, 1, GL_FALSE, &PV[0][0]);
		if (dragonAttack) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fireTexture);
			glUniform1i(fireSampler, 0);
			f_emitter->updateParticles(currentTime, dt, camera->pos);
			f_emitter->renderParticles();
			if (loopNum - dragonAttackFirstLoop > 100) {
				dragonAttack = false;
				delete f_emitter;
			}
			tower->hp -= dragonDamage;
		}

		//tower
		glUseProgram(explosionProgram);
		glUniformMatrix4fv(explosionProjAndView, 1, GL_FALSE, &PV[0][0]);
		if (tower->Attack) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, explosionTexture);
			glUniform1i(explosionSampler, 0);
			e_emitter->updateParticles(currentTime, dt, camera->pos);
			e_emitter->renderParticles();
			if (loopNum - tower->attackFirstLoop > 25) {
				tower->Attack = false;
				delete e_emitter;
			}
			if (stone->Run || stone->Attack) {
				stone->hp -= 0.1;
				if (stone->hp <= 0) {
					stone->dead = true;
					stone->reviveFirstLoop = loopNum;
				}
			}
			if (spider->Run || spider->Attack) {
				spider->hp -= 0.1;
				if (spider->hp <= 0) {
					spider->dead = true;
					spider->reviveFirstLoop = loopNum;
				}
			}
			if (snake->Run) {
				snake->hp -= 0.1;
				if (snake->hp <= 0) {
					snake->dead = true;
					snake->reviveFirstLoop = loopNum;
				}
			}
		}


		// ----------CHECK REVIVE--------------------------------------------------------------------------------------// 
		if (stone->dead == true) {
			stone->revive(loopNum);
		}
		if (snake->dead == true) {
			snake->revive(loopNum);
		}
		if (spider->dead == true) {
			spider->revive(loopNum);
		}


		// ----------CHECK TOWER HP--------------------------------------------------------------------------------------//
		if (tower->hp <= 0) {
			static int start = loopNum;
			glUseProgram(ruinsProgram);
			glUniformMatrix4fv(ruinsProjAndView, 1, GL_FALSE, &PV[0][0]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ruinsTexture);
			glUniform1i(ruinsSampler, 0);
			r_emitter->updateParticles(currentTime, dt, camera->pos);
			r_emitter->renderParticles();
			if (loopNum - start == 150) {
				stop = true; // TIME TO STOP THE GAME
			}
			tower->modelMatrix = translate(mat4(), vec3(shake, -0.1, shake)) * tower->modelMatrix;
			shake = -shake;
		}



		t = currentTime;
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 && stop == false);

}


void initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW\n");
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		throw runtime_error(string(string("Failed to open GLFW window.") +
			" If you have an Intel GPU, they are not 3.3 compatible." +
			"Try the 2.1 version.\n"));
	}
	glfwMakeContextCurrent(window);

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

	// Gray background color
	glClearColor(0.7f, 0.8f, 1.0f, 1.0f);//0.255f, 0.655f, 0.955f, 0.5f

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable texturing and bind the depth texture
	glEnable(GL_TEXTURE_2D);

	// Log
	logGLParameters();

	// Create camera
	camera = new Camera(window);

	// Createlight 
	light = new Light(window,
		vec4{ 0.99, 0.98, 0.83, 1 },
		vec4{ 0.99, 0.98, 0.83, 1 },
		vec4{ 0.99, 0.98, 0.83, 1 },
		vec3{ 10, 50, 15 },
		2000.0f
	);

}

int main(void) {
	try {
		initialize();
		createContext();
		mainLoop();
		free();
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
		getchar();
		free();
		return -1;
	}

	return 0;
}