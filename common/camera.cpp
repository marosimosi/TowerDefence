#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

using namespace glm;
vec3 direction(0, 5, 0);
float angle = 0;

Camera::Camera(GLFWwindow* window) : window(window) {
    position = vec3(0, 20, 43);
    horizontalAngle = 3.14f;
    verticalAngle = -0.5f;
    FoV = 45.0f;
    speed = 2.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 1.5f;
   
}

void Camera::update() {
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, width / 2, height / 2);

    

    // Task 5.3: Compute new horizontal and vertical angles, given windows size
    //*/
    // and cursor position
    horizontalAngle += mouseSpeed * float(width / 2 - xPos);
    verticalAngle += mouseSpeed * float(height / 2 - yPos);

    //// Task 5.4: right and up vectors of the camera coordinate system
    //// use spherical coordinates
    //vec3 direction(
    //    cos(verticalAngle) * sin(horizontalAngle),
    //    sin(verticalAngle),
    //    cos(verticalAngle) * cos(horizontalAngle)
    //);

    //// Right vector
    //vec3 right(
    //    sin(horizontalAngle - 3.14f / 2.0f),
    //    0,
    //    cos(horizontalAngle - 3.14f / 2.0f)
    //);

    //// Up vector
    //vec3 up = cross(right, direction);

    // Task 5.5: update camera position using the direction/right vectors
    // zoom in
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (radians(FoV) > 0.1 + radians(fovSpeed))
        FoV -= fovSpeed;
    }
    // zoom out
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (radians(FoV) < 3.14/4 - radians(fovSpeed))
            FoV += fovSpeed;
    }
    // rotate right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        angle += deltaTime * speed;
    }
    // rotate left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        angle -= deltaTime * speed;
    }

    //// Task 5.6: handle zoom in/out effects
    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //    if (radians(FoV) > 0.1 + radians(fovSpeed))
    //    FoV -= fovSpeed;
    //}
    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //    if (radians(FoV) < 3.14 - radians(fovSpeed))
    //    FoV += fovSpeed;
    //}

    //construct projection and view matrices
    mat3 rot = mat3( rotate(mat4(), angle, vec3(0, 1, 0)) );
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 200.0f);
    viewMatrix = lookAt(
        rot * position,
        direction,
        vec3(0.0, 1.0, 0.0)
    );
 

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}