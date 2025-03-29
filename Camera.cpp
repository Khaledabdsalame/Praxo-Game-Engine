
#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);







void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform, GLFWwindow*  window)
{
	
	glm::mat4 view = GetViewMatrix();
	glm::mat4 projection = GetProjectionMatrix(FOVdeg, nearPlane, farPlane);

	// ????? ?????? ???????? ??????
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE,
		glm::value_ptr(projection * view));
	

	
}








void Camera::Inputs(GLFWwindow* window, float deltaTime)
{
	if (ImGui::GetIO().WantCaptureMouse)
		return;


	
	float adjustedSpeed = speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += adjustedSpeed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += adjustedSpeed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += adjustedSpeed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += adjustedSpeed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += adjustedSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += adjustedSpeed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 3.9f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 2.6f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	  if (!ImGui::IsAnyItemActive() && !ImGui::IsWindowHovered())
	
		  
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

	
}


glm::vec3 Camera::ScreenPosToWorldRay(
	float mouseX, float mouseY,
	float screenWidth, float screenHeight,
	const glm::mat4& viewMatrix,
	const glm::mat4& projectionMatrix)
{
	// ????? ???????? ?????? ??? ???????? NDC
	glm::vec4 rayClip(
		(2.0f * mouseX) / screenWidth - 1.0f,
		1.0f - (2.0f * mouseY) / screenHeight,
		-1.0f,
		1.0f);

	// ????? ??? ???????? ????????
	glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// ????? ??? ???????? ??????
	glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * rayEye);
	return glm::normalize(rayWorld);
}

bool Camera::RayOBBIntersection(
	const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
	const glm::mat4& modelMatrix,
	const glm::vec3& aabbMin, const glm::vec3& aabbMax,
	float& intersectionDistance)
{
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 obbPosition(modelMatrix[3]);
	glm::vec3 delta = obbPosition - rayOrigin;

	// ?????? ??????? ?? ????????? ???????
	for (int i = 0; i < 3; i++) {
		glm::vec3 axis(modelMatrix[i]);
		float e = glm::dot(axis, delta);
		float f = glm::dot(rayDirection, axis);

		if (fabs(f) > 0.001f) {
			float t1 = (e + aabbMin[i]) / f;
			float t2 = (e + aabbMax[i]) / f;

			if (t1 > t2) std::swap(t1, t2);
			if (t2 < tMax) tMax = t2;
			if (t1 > tMin) tMin = t1;

			if (tMax < tMin) return false;
		}
		else {
			if (-e + aabbMin[i] > 0.0f || -e + aabbMax[i] < 0.0f)
				return false;
		}
	}

	intersectionDistance = tMin;
	return true;
}
