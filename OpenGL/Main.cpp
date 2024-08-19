#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// FORWARD DECLARATIONS
// --------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// SHADER SOURCE CODE
// ------------------
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
"}\n\0";
// MAIN
// ----
int main()
{
	// GLFW INITIALIZATION AND WINDOW CREATION
	// ---------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window{ glfwCreateWindow(800,800, "This is going to be an epic demo :D", NULL, NULL) };
	if (window == NULL)
	{
		std::cout << "Window failed to create" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// LOAD OPENGL FUNCITON POINTERS
	// -----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load OpenGL function pointers" << std::endl;
		return -1;
	}
	// INITIALIZE VERTEX SHADER
	// ------------------------
	unsigned int vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed" << infoLog << std::endl;
	}
	// INITIALIZE FRAGMENT SHADER
	// --------------------------
	unsigned int fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed" << infoLog << std::endl;
	}
	// LINK SHADERS
	// ------------
	unsigned int shaderProgram{ glCreateProgram() };
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// CHECK PROGRAM LINK SUCCESS
	// --------------------------
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// INITIALIZE VERTEX DATA
	// ----------------------
	GLfloat vertices[]
	{
	-0.5f,0.0f,0.0f,
	0.0f,0.5f,0.0f,
	0.5f,0.0f,0.0f,
	};
	// INITIALIZE VERTEX BUFFER OBJECT
	// -------------------------------
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// BIND VAO AND VBO TO PASS VERTEX DATA TO BUFFER OBJECT
	// -----------------------------------------------------
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// VERTEX ATTRIBUTE POINTER
	// ------------------------
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	// UNBIND ARRAY BUFFER
	// -------------------
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Binding array and buffer obects to 0 is to "unbind" them
	// RENDER LOOP
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// INPUT
		// -----
		processInput(window);
		// RENDER COLORED BACKGROUND
		// -------------------------
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// DRAW SOME TRIANGLES MF
		// -------------------------
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// GLFW SWAP BUFFERS AND POLL EVENTS (MOUSE MOVEMENT, KEYBOARD, ETC.)
		// ------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// DE-ALLOCATE RESOURCES
	// ---------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}
// PROCESSINPUT() AND FRAMEBUFFER_SIZE_CALLBACK IMPLEMENTATIONS
// ------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
