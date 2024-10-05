#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>  // For sine wave animation

// FORWARD DECLARATIONS
// --------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float& xOffset, float& yOffset);

// SHADER SOURCE CODE
// ------------------
// Vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"      // Position attribute
"layout(location = 1) in vec3 aColor;\n"    // Color attribute
"out vec3 vertexColor;\n"                   // Output color to fragment shader
"uniform vec2 translation;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + translation.x, aPos.y + translation.y, aPos.z, 1.0);\n"
"   vertexColor = aColor;\n"  // Pass the color to the fragment shader
"}\0";

// Fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"       // Interpolated color from vertex shader
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f);\n"  // Use the interpolated color
"}\n\0";

int main()
{
    // GLFW initialization and window creation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "RGB Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window failed to create" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL function pointers" << std::endl;
        return -1;
    }

    // Initialize vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    // Initialize fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Failed to link shader program: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data: positions and colors
    GLfloat vertices[] = {
        // Positions         // Colors (RGB)
        -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom left, Red
         0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Top, Green
         0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f   // Bottom right, Blue
    };

    // Vertex buffer object (VBO), Vertex array object (VAO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and set up vertex buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Translation variables
    float xOffset = 0.0f;
    float yOffset = 0.0f;

    // Time variable for color animation
    float timeValue = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window, xOffset, yOffset);

        // Clear background
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update colors in real-time using a sine wave for animation
        timeValue = glfwGetTime();
        vertices[3] = 1.0f;  // Red value for vertex 1
        vertices[9] = (sin(timeValue + 2.0f) + 1.0f) / 2.0f;  // Green value for vertex 2
        vertices[15] = (sin(timeValue + 4.0f) + 1.0f) / 2.0f;  // Blue value for vertex 3

        // Update VBO with new color data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Use shader program and update translation
        glUseProgram(shaderProgram);
        int translationLoc = glGetUniformLocation(shaderProgram, "translation");
        glUniform2f(translationLoc, xOffset, yOffset);

        // Draw the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}

// Process input for translation
void processInput(GLFWwindow* window, float& xOffset, float& yOffset)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        xOffset -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        xOffset += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        yOffset += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        yOffset -= 0.01f;
}

// Callback for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
