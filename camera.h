#include <glad/glad.h> // Include GLAD to get all the OpenGL headers that are needed

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const float FOV = 90.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float YAW = -90.0f;
const float PITCH = 0.0f;


enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{

public:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler angles
    float yaw;
    float pitch;

    float cameraSpeed;
    float mouseSensitivity;
    float cameraFOV;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float fov = FOV, float sensitivity = SENSITIVITY, float speed = SPEED, float yaw = YAW, float pitch = PITCH);

    const glm::mat4 getViewMatrix();
    
    void keyboardInput(Camera_Movement direction, float deltaTime);
    void mouseInput(float xOffset, float yOffset, bool lockPitch = true);
    void scrollInput(float yOffset);

private:
    // Calculates the camera's 'front' value based on the Euler Angles
    void updateCamera();
};