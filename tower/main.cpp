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
#include <tower/Dragon.h>
#include <tower/Skeleton.h>


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
Stone* stone;
Floor* plane;
Tower* tower;
Dragon* dragon;
Spider* spider;
Skeleton* skeleton;
Mountain* mountain;
GLuint depthFrameBuffer, depthTexture;

Drawable* quad;

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

mat4 stoneModelMatrix = translate(mat4(), vec3(12, 0, 0)) * scale(mat4(), vec3(0.5, 0.5, 0.5));
mat4 planeModelMatrix = mat4();
mat4 towerModelMatrix = scale(mat4(), vec3(0.7, 1, 0.7));
mat4 dragonModelMatrix = translate(mat4(), vec3(-12, 10, 0)) * scale(mat4(), vec3(0.1, 0.1, 0.1));
mat4 mountainModelMatrix = translate(mat4(), vec3(0, 0.01, -45)) * scale(mat4(), vec3(6, 6, 6)) * rotate(mat4(), radians(90.0f), vec3(0, 1, 0));
mat4 spiderModelMatrix = translate(mat4(), vec3(-5, 0, 0)) * scale(mat4(), vec3(0.3, 0.3, 0.3)) * rotate(mat4(), radians(180.0f), vec3(0, 1, 0));
mat4 skeletonModelMatrix = translate(mat4(), vec3(5, 0, 0));

mat4 spiderInstancing[2] = { translate(mat4(), vec3(17, 0, -12)), translate(mat4(), vec3(0, 0, 0)) };
mat4 skeletonInstancing[3] = { translate(mat4(), vec3(0, 0, 2)), translate(mat4(), vec3(-1.5, 0, 0)),  translate(mat4(), vec3(1.5, 0, 0)) };

// locations for depthProgram
GLuint shadowViewProjectionLocation; 
GLuint shadowModelLocation;

// locations for miniMapProgram
GLuint quadTextureSamplerLocation;

// Create two sample materials
const Material polishedSilver{
	vec4{0.23125, 0.23125, 0.23125, 1},
	vec4{0.2775, 0.2775, 0.2775, 1},
	vec4{0.773911, 0.773911, 0.773911, 1},
	89.6f
};

const Material turquoise{
	vec4{ 0.1, 0.18725, 0.1745, 0.8 },
	vec4{ 0.396, 0.74151, 0.69102, 0.8 },
	vec4{ 0.297254, 0.30829, 0.306678, 0.8 },
	12.8f
};

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

	shaderProgram = loadShaders("ShadowMapping.vertexshader", "ShadowMapping.fragmentshader");
	depthProgram = loadShaders("Depth.vertexshader", "Depth.fragmentshader");
	miniMapProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");


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


	// --- miniMapProgram ---
	quadTextureSamplerLocation = glGetUniformLocation(miniMapProgram, "textureSampler");
	

	

	// Load models

	stone = new Stone();
	plane = new Floor();
	tower = new Tower();
	dragon = new Dragon();
	spider = new Spider();
	skeleton = new Skeleton();
	mountain = new Mountain();

	//minimap
	vector<vec3> quadVertices = {
	  vec3(0.5, 0.5, 0.0),
	  vec3(1.0, 0.5, 0.0),
	  vec3(1.0, 1.0, 0.0),
	  vec3(1.0, 1.0, 0.0),
	  vec3(0.5, 1.0, 0.0),
	  vec3(0.5, 0.5, 0.0)
	};

	vector<vec2> quadUVs = {
	  vec2(0.0, 0.0),
	  vec2(1.0, 0.0),
	  vec2(1.0, 1.0),
	  vec2(1.0, 1.0),
	  vec2(0.0, 1.0),
	  vec2(0.0, 0.0)
	};

	quad = new Drawable(quadVertices, quadUVs);



	// ---------------------------------------------------------------------------- //
	// -  Task 3.2 Create a depth framebuffer and a texture to store the depthmap - //
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// Task 4.5 Texture wrapping methods
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);							// GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	//*/

	// Task 4.5 Don't shadow area out of light's viewport
	/*/
	// Step 1 : (Don't forget to comment out the respective lines above
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Set color to set out of border 
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Next go to fragment shader and add an iff statement, so if the distance in the z-buffer is equal to 1, 
	// meaning that the fragment is out of the texture border (or further than the far clip plane) 
	// then the shadow value is 0.
	//*/

	//*/ Task 3.2 Continue
	// Attaching the texture to the framebuffer, so that it will monitor the depth component
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
	glDeleteProgram(miniMapProgram);

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
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &stoneModelMatrix[0][0]);
	stone->draw();
	
	//PLANE
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &planeModelMatrix[0][0]);
	plane->draw();

	//TOWER
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &towerModelMatrix[0][0]);
	tower->draw();

	//MOUNTAIN
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &mountainModelMatrix[0][0]);
	mountain->draw();

	//DRAGON
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &dragonModelMatrix[0][0]);
	dragon->draw();

	//SPIDER
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &spiderModelMatrix[0][0]);
	glUniformMatrix4fv(shadowOffsets, 2, GL_FALSE, &spiderInstancing[0][0][0]);
	spider->draw();

	//SKELETON
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &skeletonModelMatrix[0][0]);
	glUniformMatrix4fv(shadowOffsets, 3, GL_FALSE, &skeletonInstancing[0][0][0]);
	skeleton->draw();

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
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &stoneModelMatrix[0][0]);
      
	glActiveTexture(GL_TEXTURE1);								
	glBindTexture(GL_TEXTURE_2D, stone->diffuseTexture);			
	glUniform1i(diffuseColorSampler, 1);						
	
	glActiveTexture(GL_TEXTURE2);								
	glBindTexture(GL_TEXTURE_2D, stone->specularTexture);			
	glUniform1i(specularColorSampler, 2);						

	glUniform1i(useTextureLocation, 1);

	stone->draw();


	//FLOOR
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &planeModelMatrix[0][0]);
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
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &towerModelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tower->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tower->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	tower->draw();

	
	//DRAGON
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &dragonModelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dragon->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dragon->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	dragon->draw();

	
	//MOUNTAIN
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mountainModelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mountain->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mountain->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	mountain->draw();

	//SPIDER
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &spiderModelMatrix[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, spider->diffuseTexture);
	glUniform1i(diffuseColorSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, spider->specularTexture);
	glUniform1i(specularColorSampler, 2);

	glUniform1i(useTextureLocation, 1);

	glUniformMatrix4fv(offsets, 2, GL_FALSE, &spiderInstancing[0][0][0]);

	spider->draw();

	//SKELETON
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &skeletonModelMatrix[0][0]);

	uploadMaterial(bone);

	glUniform1i(useTextureLocation, 0);

	glUniformMatrix4fv(offsets, 3, GL_FALSE, &skeletonInstancing[0][0][0]);

	skeleton->draw();


	//UNDO translations for instancing
	mat4 undoOffsets[3] = { mat4(1), mat4(1), mat4(1) };

	glUniformMatrix4fv(offsets, 1, GL_FALSE, &undoOffsets[0][0][0]);
}


void renderDepthMap() {
	glUseProgram(miniMapProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(quadTextureSamplerLocation, 0);

	quad->bind();
	//quad->draw();
}



void mainLoop() {


	light->update();
	mat4 light_proj = light->projectionMatrix;
	mat4 light_view = light->viewMatrix;

	// Task 3.3
	// Create the depth buffer

	do {

		light->update();
		mat4 light_proj = light->projectionMatrix;
		mat4 light_view = light->viewMatrix;


		// Task 3.5
		// Create the depth buffer
		depth_pass(light_view, light_proj);
		

		// Getting camera information
		camera->update();
		mat4 projectionMatrix = camera->projectionMatrix;
		mat4 viewMatrix = camera->viewMatrix;

		//lighting_pass(viewMatrix, projectionMatrix);
		
		// Task 1.5
		// Rendering the scene from light's perspective when F1 is pressed
		//*/
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
			lighting_pass( light_view, light_proj );
		}
		else {
			// Render the scene from camera's perspective
			lighting_pass( viewMatrix, projectionMatrix );
		}
		//*/

		// Task 2.3:
		renderDepthMap();


		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

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

	// Cull triangles which normal is not towards the camera
	// glEnable(GL_CULL_FACE);

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
		vec3{ 0, 30, 20 },
		800.0f
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