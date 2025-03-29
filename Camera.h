#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL


#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include"Shaders/ShaderClass.h"


#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include "ImGuizmo.h"



class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(-0.5f, -0.32f, -0.7f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 2.9f;
	float sensitivity =	100.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);

	// Updates and exports the camera matrix to the Vertex Shader
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window, float deltaTime);


	static glm::vec3 ScreenPosToWorldRay(
		float mouseX, float mouseY,
		float screenWidth, float screenHeight,
		const glm::mat4& viewMatrix,
		const glm::mat4& projectionMatrix);

	static bool RayOBBIntersection(
		const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
		const glm::mat4& modelMatrix,
		const glm::vec3& aabbMin, const glm::vec3& aabbMax,
		float& intersectionDistance);

	glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Orientation, Up); }
	glm::mat4 GetProjectionMatrix(float FOVdeg, float nearPlane, float farPlane) const {
		return glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	}

	
	
};
#endif