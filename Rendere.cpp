


#include"Shaders/ShaderClass.h"

#include"Buffers/EBO.h"
#include"Buffers/VAO.h"
#include"Buffers/VBO.h"
#include "Models/Models.h"
#include<stb/stb_image.h>


#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include<Windows.h>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "ImGuiFileDialog.h"
#include "Console.h"
#include"Camera.h"






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

	Shader shaderProgram("VertexShader.glsl", "FragmentShader.glsl");



	int width, hight, colorch;
	stbi_set_flip_vertically_on_load(true);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



	
	

	// 2D SPace

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 3D Space 
	VAO VAO4;
	VAO4.Bind();
	VBO VBO4(Verticies::cubeVertices, sizeof(Verticies::cubeVertices));
	EBO EBO4(Verticies::cubeIndices, sizeof(Verticies::cubeIndices));
	VAO4.LinkAttrib(VBO4, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO4.LinkAttrib(VBO4, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO4.Unbind();
	VBO4.Unbind();
	EBO4.Unbind();


	
	VAO VAO5;
	VAO5.Bind();
	VBO VBO5(Verticies::Pyramides, sizeof(Verticies::Pyramides));
	EBO EBO5(Verticies::Pyramides_indices, sizeof(Verticies::Pyramides_indices));
	VAO5.LinkAttrib(VBO5, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO5.LinkAttrib(VBO5, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO5.Unbind();
	VBO5.Unbind();
	EBO5.Unbind();

	






	

	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// By Default 
	bool drawTriangle = false;
	bool DrawSquare = true;
	bool Gradinet = true;
	float size = 1.0f;
	float color[4] = { 1.0f, 0.97f, 0.95f, 1.0f };
	float color1[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float color2[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	

	//Shader Prgrame
	shaderProgram.Activate();
	glUniform1f(shaderProgram.BindImgui("size"), size);
    glUniform4f(shaderProgram.BindImgui("color"), color[0], color[1], color[2], color[3]);
	
	glUniform4f(shaderProgram.BindImgui("color1"), color1[0], color1[1], color1[2], color1[3]);
	glUniform4f(shaderProgram.BindImgui("color2"), color2[0], color2[1], color2[2], color2[3]);
	
	
	glEnable(GL_DEPTH_TEST);

	
	Camera camera (GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),glm::vec3(2.2f, 1.1f, 2.9f));

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	
	
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
		
	
	

	

		

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		// Window flags to disable moving, resizing, and the title bar
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse ;

			
		
		// ImGUI window creation
		ImGui::SetNextWindowSize(ImVec2(189, 200));
		ImGui::Begin("Shader Engine", nullptr ,window_flags);


		// Text that appears in the window
		ImGui::Text("Shader Changer");
		// Checkbox that appears in the window
		


		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("ScreenClickPopup");
		}

		// Center the popup at mouse position
		if (ImGui::BeginPopup("ScreenClickPopup")) {
			ImGui::SetNextWindowPos(io.MousePos, ImGuiCond_Appearing);  // Open at click location

			if (ImGui::BeginMenu("Add")) {
				if (ImGui::MenuItem("Cube"))
				{

				}
				if (ImGui::MenuItem("Pyramid"))
				{

				}

				if (ImGui::MenuItem("Circle"))
				{

				}
			ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Option 2")) {
				// Handle option 2
			}
			if (ImGui::MenuItem("Close")) {
				ImGui::CloseCurrentPopup();
			}

			

			ImGui::EndPopup();
		}
		
		
			
			
			ImGui::Checkbox("Draw Pyramide", &drawTriangle);
			ImGui::Checkbox("Draw Cube", &DrawSquare);
			
			VAO4.Bind();
			if (DrawSquare)
			{   
				
				
				
				
                camera.Inputs(window, deltaTime);
				camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix",window);
				
					
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			}
			else if (drawTriangle)
			{


				camera.Inputs(window, deltaTime);
				camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix",window);
				VAO5.Bind();
			    glDrawElements(GL_TRIANGLES, sizeof(Verticies::Pyramides_indices) / sizeof(int), GL_UNSIGNED_INT, 0);

				
				
				
			}
         
		
		
		
		ImGui::SliderFloat("Size", &size, 0.0f, 2.0f);
		// Fancy color editor that appears in the window
		ImGui::ColorEdit4("Color", color );
		shaderProgram.Activate();
		glUniform1f(shaderProgram.BindImgui("size"), size);

		glUniform4f(shaderProgram.BindImgui("color"), color[0], color[1], color[2], color[3]);
		
		ImGui::Checkbox("Gradient", &Gradinet);
		if (Gradinet)
		{  
			//Reset Values 
			
			
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
			// Open file diaog
			IGFD::FileDialogConfig config;
			config.path = ".";
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Select A Texture", ".png,.jpg",config);

		}
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::cout << ImGuiFileDialog::Instance()->GetCurrentFilter();
				unsigned char* bytes = stbi_load(filePath.c_str(), &width, &hight, &colorch, 0);
				if (ImGuiFileDialog::Instance()->GetCurrentFilter()==".png")
				{
                   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, hight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
				}
				else 
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
				}
				
				
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(bytes);
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 1);
				GLuint tex0uni = glGetUniformLocation(shaderProgram.ID,"tex0");
				shaderProgram.Activate();
				glUniform1i(tex0uni,0);
				glBindTexture(GL_TEXTURE_2D, texture);
				
				
			}
			ImGuiFileDialog::Instance()->Close();
		}
		
		if (ImGui::Button("Delete Texture")) {
			glBindTexture(GL_TEXTURE_2D, 0);
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
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	
	glfwTerminate();
	return 0;
}