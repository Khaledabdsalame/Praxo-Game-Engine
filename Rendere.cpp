

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include"Shaders/ShaderClass.h"
#include"Buffers/EBO.h"
#include"Buffers/VAO.h"
#include"Buffers/VBO.h"
#include "Models/Models.h"
#include<stb/stb_image.h>
#include<Windows.h>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "ImGuiFileDialog.h"
#include "Console.h"
#include"Camera.h"
#include "Textures/Textures.h"





int main()
{
	// Initialize GLFW
	glfwInit();

	PraxoConsole console;
	
	
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	GLFWwindow* window = glfwCreateWindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "Praxo Game Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to Open The Engine" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	
	glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	Shader shaderProgram("Shaders/Glsl/VertexShader.glsl", "Shaders/Glsl/FragmentShader.glsl");

	

	glm::mat4 cubeModel = glm::mat4(1.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	glm::mat4 light = glm::mat4(1.0f);
	
	ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::TRANSLATE;
	
	

	int selectedObject = 0;

	
	Texture Textures;
	

	// 2D SPace

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 3D Space 
	VAO VAO4;
	VAO4.Bind();
	VBO VBO4(Verticies::cubeVertices, sizeof(Verticies::cubeVertices));
	EBO EBO4(Verticies::cubeIndices, sizeof(Verticies::cubeIndices));
	VAO4.LinkAttrib(VBO4, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO4.LinkAttrib(VBO4, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO4.LinkAttrib(VBO4, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	VAO4.Unbind();
	VBO4.Unbind();
	EBO4.Unbind();


	
	VAO VAO5;
	VAO5.Bind();
	VBO VBO5(Verticies::Pyramides, sizeof(Verticies::Pyramides));
	EBO EBO5(Verticies::Pyramides_indices, sizeof(Verticies::Pyramides_indices));
	VAO5.LinkAttrib(VBO5, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO5.LinkAttrib(VBO5, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO5.LinkAttrib(VBO5, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	VAO5.Unbind();
	VBO5.Unbind();
	EBO5.Unbind();

	Shader lightShader("Shaders/Glsl/LightVert.glsl", "Shaders/Glsl/LightFrag.glsl");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(Verticies::lightVertices, sizeof(Verticies::lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(Verticies::lightIndices, sizeof(Verticies::lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;





	

	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// By Default 
	bool drawTriangle = false;
	bool DrawSquare = true;
	bool addlight = false;
	bool Gradinet = true;
	bool showGizmo = false;

	
	float color[4] = { 1.0f, 0.97f, 0.95f, 1.0f };
	float color1[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float color2[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightcolor[4] = { 1.0,1.0f,1.0f,1.0f };
	

	//Shader Prgrame
	shaderProgram.Activate();
	
    glUniform4f(shaderProgram.BindImgui("color"), color[0], color[1], color[2], color[3]);
	
	glUniform4f(shaderProgram.BindImgui("color1"), color1[0], color1[1], color1[2], color1[3]);
	glUniform4f(shaderProgram.BindImgui("color2"), color2[0], color2[1], color2[2], color2[3]);
	
	
	glEnable(GL_DEPTH_TEST);

	
	Camera camera (GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),glm::vec3(2.2f, 1.1f, 2.9f));

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	
	static std::vector<std::string> gameObjects = {"Cube","Pyramid","Light"};
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 555), ImGuiCond_Always);
		console.Draw("Praxo Engine Console");
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
        

		
		
		shaderProgram.Activate();
		
	
	

	
		
		camera.Inputs(window, deltaTime);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		
		Textures.Bind();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		// Window flags to disable moving, resizing, and the title bar
		 ;

			
		
		// ImGUI window creation
		ImGui::SetNextWindowSize(ImVec2(189, 200));
		ImGui::Begin("Shader Engine", nullptr ,window_flags);


		// Text that appears in the window
		ImGui::Text("Shader Changer");
		// Checkbox that appears in the window
		


		

		
		
			
			
			ImGui::Checkbox("Draw Pyramide", &drawTriangle);
			ImGui::Checkbox("Draw Cube", &DrawSquare);
			ImGui::Checkbox("Add Light", &addlight);
			
			
			
			if (DrawSquare)
			{   
				
				
				
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
				VAO4.Bind();
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			}
			 if (drawTriangle)
			{

				
				
				 glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
				 VAO5.Bind();
				 glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}

			 if (addlight)
			 {
				 lightShader.Activate();
				 camera.Matrix(45.0f, 0.1f, 100.0f, lightShader, "camMatrix");
				 glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light));
				 lightVAO.Bind();
				 glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				 ImGui::ColorEdit4("Light", lightcolor);
				 glUniform4f(lightShader.BindImgui("lightColor"), lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3]);

				 shaderProgram.Activate();
				 glUniform4f(shaderProgram.BindImgui("lightColor"), lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3]);
				 glUniform3f(shaderProgram.BindImgui("lightPos"),glm::vec3(light[3]).x, glm::vec3(light[3]).y, glm::vec3(light[3]).z);
				 glUniform3f(shaderProgram.BindImgui("camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

			 }
			 else 
			 {
				 
				 shaderProgram.Activate();
				 glUniform4f(shaderProgram.BindImgui("lightColor"), 0.0f ,0.0f, 0.0f, 0.0f);
				 glUniform4f(shaderProgram.BindImgui("color"), color[0], color[1], color[2], color[3]);
				 glUniform4f(shaderProgram.BindImgui("color1"), color1[0], color1[1], color1[2], color1[3]);
				 glUniform4f(shaderProgram.BindImgui("color2"), color2[0], color2[1], color2[2], color2[3]);

			 }
			 

			 if (ImGui::IsMouseClicked(0) && !ImGui::GetIO().WantCaptureMouse) {
				 
				 ImVec2 mousePos = ImGui::GetMousePos();

				
				 glm::vec3 rayOrigin = camera.Position;
				 glm::vec3 rayDirection = camera.ScreenPosToWorldRay(
					 mousePos.x, mousePos.y,
					 io.DisplaySize.x, io.DisplaySize.y,
					 camera.GetViewMatrix(),
					 camera.GetProjectionMatrix(45.0f, 0.1f, 100.0f)
				 );

				
				 float cubeDistance;
				 
				 if (camera.RayOBBIntersection(
					 rayOrigin, rayDirection,
					 cubeModel,
					 glm::vec3(-0.5f, -0.5f, -0.5f), 
					 glm::vec3(0.5f, 0.5f, 0.5f),
					  
					 cubeDistance))
				 {
					 selectedObject = 0;
					 showGizmo = true;
				 }
				 
				 else if (camera.RayOBBIntersection(
					 rayOrigin, rayDirection,
					 pyramidModel,
					 glm::vec3(-0.5f, -0.5f, -0.5f), 
					 glm::vec3(0.5f, 0.5f, 0.5f),    
					 cubeDistance))
				 {
					 selectedObject = 1;
					 showGizmo = true;
				 }

				 else if (camera.RayOBBIntersection(
					 rayOrigin, rayDirection,
					 light,
					 glm::vec3(-0.5f, -0.5f, -0.5f),
					 glm::vec3(0.5f, 0.5f, 0.5f),
					 cubeDistance))
				 {
					 selectedObject = 2;
					 showGizmo = true;
				 }
				 else {
					 showGizmo = false;
				 }
			 }


			 ImGui::SetNextWindowPos(ImVec2(0, 320), ImGuiCond_Always);
			 ImGui::Begin("ESC Temp", nullptr, window_flags);
			 ImGui::Text("Select a Game Object:");
			 ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);
			 for (int i = 0; i < gameObjects.size(); ++i) {
				 if (ImGui::Selectable(gameObjects[i].c_str(), selectedObject == i)) {
					 selectedObject = i;
					 showGizmo = true;
				 }
			 }

			 ImGui::EndChild();
			 ImGui::End();


			 
			 if (showGizmo) {
				 

				 ImGuizmo::BeginFrame();
				 ImGuizmo::Enable(true); 
				
			     ImGuizmo::SetOrthographic(false);
				

				 ImGuizmo::SetRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

				
				 if (ImGui::IsKeyPressed(ImGuiKey_T)) currentGizmoOperation = ImGuizmo::TRANSLATE;
				 if (ImGui::IsKeyPressed(ImGuiKey_G))currentGizmoOperation = ImGuizmo::ROTATE;
				 if (ImGui::IsKeyPressed(ImGuiKey_H)) currentGizmoOperation = ImGuizmo::SCALE;
				 
				 
				 glm::mat4* modelToManipulate = (selectedObject == 0) ? &cubeModel : (selectedObject == 1) ? &pyramidModel : &light;
				 ImGuizmo::Manipulate(
					 glm::value_ptr(camera.GetViewMatrix()),
					 glm::value_ptr(camera.GetProjectionMatrix(45.0f, 0.1f, 100.0f)),
					 currentGizmoOperation,
					 ImGuizmo::WORLD,
					 glm::value_ptr(*modelToManipulate)
				 );
			 }
			
		
		
		
		
		ImGui::ColorEdit4("Color", color );
		shaderProgram.Activate();
		
	
		glUniform4f(shaderProgram.BindImgui("color"), color[0], color[1], color[2], color[3]);
		
		ImGui::Checkbox("Gradient", &Gradinet);
		if (Gradinet)
		{  
			
			
			ImGui::ColorEdit4("Color1", color1);
			ImGui::ColorEdit4("Color2", color2);
			// Default Alpha
			color1[3] = 1.0f;
			color2[3] = 1.0f;

			
		}
		else
		{
          for (int i = 0; i < 4; i++)
		  {
			color1[i] = 0.0f;
			color2[i] = 0.0f;
		  }

		}
		glUniform4f(shaderProgram.BindImgui("color1"), color1[0], color1[1], color1[2], color1[3]);
	    glUniform4f(shaderProgram.BindImgui("color2"), color2[0], color2[1], color2[2], color2[3]);
	
		
		// Ends the window
		ImGui::End();
		
		ImGui::SetNextWindowPos(ImVec2(0, 200), ImGuiCond_Always);
		ImGui::Begin("Textures",nullptr, window_flags);
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		float squareSize = fminf(availableSize.x, availableSize.y);

		ImGui::Text("Select Textures");

		if (ImGui::Button("Import Texture"))
		{
			IGFD::FileDialogConfig config;
			config.path = ".";
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Select A Texture", ".png,.jpg",config);

		}
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				shaderProgram.Activate();
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				Textures.Setter(filePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
				glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 1);
				Textures.texUnit(shaderProgram, "tex0", 0);
				
				
				
			}
			ImGuiFileDialog::Instance()->Close();
		}
		
		if (ImGui::Button("Delete Texture")) {
			shaderProgram.Activate();
			Textures.Unbind();
			glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 0);
		}

		
        
		ImGui::End();

		
		
		
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		
		glfwPollEvents();
	}
    
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	VAO4.Delete();
	VBO4.Delete();	
	EBO4.Delete();
	VAO5.Delete();
	VBO5.Delete();
	EBO5.Delete();
	lightShader.Delete();
	Textures.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}