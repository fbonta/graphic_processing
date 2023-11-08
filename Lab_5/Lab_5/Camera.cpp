#include "Camera.hpp"

/*
 * https://learnopengl.com/Getting-started/Camera
 */

static glm::vec3 world_up;

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) : cameraPosition(cameraPosition), cameraTarget(cameraTarget) {
        world_up = cameraUp;
    	cameraFrontDirection = normalize(cameraPosition - cameraTarget);
        cameraRightDirection = normalize(cross(cameraUp, cameraFrontDirection));
        cameraUpDirection = normalize(cross(cameraFrontDirection, cameraRightDirection));
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix()
    {
        return lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
	    switch (direction)
	    {
	    case MOVE_LEFT:
            cameraPosition -= speed * normalize(cross(cameraFrontDirection, cameraUpDirection));
            break;
	    case MOVE_RIGHT:
            cameraPosition += speed * normalize(cross(cameraFrontDirection, cameraUpDirection));
            break;
	    case MOVE_BACKWARD:
            cameraPosition -= speed * cameraFrontDirection;
            break;
	    case MOVE_FORWARD:
            cameraPosition += speed * cameraFrontDirection;
            break;
	    }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
	    const glm::vec3 direction(glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)), 
	                              glm::sin(glm::radians(pitch)), 
	                              glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));

        cameraFrontDirection = normalize(direction);
        cameraRightDirection = normalize(cross(world_up, cameraFrontDirection));
        cameraUpDirection = normalize(cross(cameraFrontDirection, cameraRightDirection));
    }
}