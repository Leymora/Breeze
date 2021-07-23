#include "camera.h"

Camera::Camera(glm::fvec3 position, float fov, float sensitivity, float speed, float yaw, float pitch)
{
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);

    this->position = position;
    this->worldUp = this->up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->cameraFOV = fov;
    this->cameraSpeed = speed / 1000;
    this->mouseSensitivity = sensitivity;

    updateCamera();
}

const glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::keyboardInput(Camera_Movement direction, float deltaTime)
{
    float velocity = this->cameraSpeed * deltaTime;
	if (direction == FORWARD)
		this->position += this->front * velocity;
	if (direction == BACKWARD)
		this->position -= this->front * velocity;
	if (direction == LEFT)
		this->position -= glm::normalize(glm::cross(this->front, this->up)) * velocity;
	if (direction == RIGHT)
		this->position += glm::normalize(glm::cross(this->front, this->up)) * velocity;

}

void Camera::mouseInput(float xOffset, float yOffset, bool lockPitch)
{
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw   += xOffset;
    this->pitch += yOffset;

    //Lock pitch so the player can only look 90° up and down.
    if (lockPitch == true)
    {
        //Up
	    if (pitch > 89.0f)
		    pitch = 89.0f;
	    //Down
	    if (pitch < -89.0f)
		    pitch = -89.0f;
    }

    updateCamera();

}

void Camera::scrollInput(float yOffset)
{
    this->cameraFOV -= yOffset * 2;

    if (this->cameraFOV < 20.0f)
		this->cameraFOV = 20.0f;
	if (this->cameraFOV > 120.0f)
		this->cameraFOV = 120.0f;
}

void Camera::updateCamera()
{
    glm::vec3 tempFront;
    tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    tempFront.y = sin(glm::radians(pitch));
    tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->front = glm::normalize(tempFront);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
