#ifndef CAMERA_H
#define CAMERA_H

//External Library for Matrix Math (GLM)
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Created H files

namespace Renderer
{
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		RELEASE_FORWARD,
		RELEASE_BACKWARD,
		RELEASE_LEFT,
		RELEASE_RIGHT
	};

	// Default camera values
	const float YAW = 0.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.0f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;
	const float MIN_ZOOM = 1.0f;
	const float MAX_ZOOM = 45.0f;
	const float FOLLOW_RADIUS = 3.0f;
	const float FOLLOW_HEIGHT = 1.5f;


	// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
	class Camera
	{
	public:
		// Camera options
		float Zoom;

		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

		// Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 getViewMatrix() const;

		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void processKeyboard(Camera_Movement direction, float deltaTime);

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void processMouseScroll(float yoffset);

		void follow(const glm::vec3& point);

		float getYaw() const;

		float getPitch() const;

		glm::vec3 getFront() const;

		glm::vec3 getUp() const;

	private:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// Euler Angles
		float Yaw;
		float Pitch;
		// Camera options
		float MovementSpeed;
		float MouseSensitivity;
		// Following options;
		float followRadius;
		float followHeight;

		// Calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
	};
}
#endif