

#include "init.h"
#include "types.h"
#include <vector>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

InitResult starData;
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\n\0";

// find stars where user clicks
void MouseClickCallback (GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        Window* screen = static_cast<Window*>(glfwGetWindowUserPointer(window)); // getting screen size

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        std::cout << xpos << ", " << ypos << std::endl;

        double FOVht = screen->height * (screen->FOV/screen->width);

        // where camera faces
        double facingDegHorizontal = 180; // between 0 to 360
        double facingDegVertical = 0; // between -90 to 90

        // how many degrees the click is from center
        double ydegreeOffset = FOVht * (ypos / screen->height) - FOVht / 2;
        double xdegreeOffset = screen->FOV * (xpos / screen->width) - screen->FOV / 2;

        // final degree coords of click 
        int xdegree = facingDegHorizontal + xdegreeOffset;
        int ydegree = facingDegVertical - ydegreeOffset;

        std::cout << "Degree coords: " << xdegree << ", " << ydegree << std::endl;

        
        // print stars nearby
        std::vector<DegreeNode> coord = starData.starLocs.at(xdegree).at(ydegree+90);
    
        for (DegreeNode star : coord) {
            std::cout << star.id << ", " << star.RAdegree << "|" << star.DEdegree << ", " << starData.stars.at(star.id).vMag;
            if (starData.stars.at(star.id).name.length() > 0) {
                std::cout << ", " << starData.stars.at(star.id).name;
            }
            std::cout << std::endl;
        }
        

    }
}

int main() {
    double FOV = 90;
    Window screen = {800, 600, FOV};
    //Angle direction = {180, 0};
    
    // double FOVht = screenHeight * (FOV/screenWidth);
    // double RAdegreesInPxl = FOVht / screenHeight;
    // double DEdegreesInPxl = FOV / screenWidth;

    // std::cout << "FOVht: " << FOVht << std::endl;
    // std::cout << "RAdegreesInPxl: " << RAdegreesInPxl << std::endl;
    // std::cout << "DEdegreesInPxl: " << DEdegreesInPxl << std::endl;

    std::cout << "Initialising from csv..." << std::endl; 
    InitResult lists = init("starsN.csv");
    std::cout << "Initialisation complete." << std::endl; 

    starData = lists;
    
    
    // random testing of stars in a location
    int randRA = rand() % 360;
    int randDE = rand() % 180;
    std::vector<DegreeNode> testLoc = lists.starLocs.at(randRA).at(randDE);

    std::cout << "Testing location " << randRA << " | " << randDE-90 << std::endl;
    
    for (DegreeNode star : testLoc) {
        std::cout << star.id << ", " << star.RAdegree << "|" << star.DEdegree << ", " << lists.stars.at(star.id).vMag << std::endl;
    }

    
    GLFWwindow* window;

    //glfwSetWindowUserPointer(window, &direction);  // store direction

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screen.width, screen.height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, &screen);  // store size

    glfwSetMouseButtonCallback(window, MouseClickCallback);


    if (glewInit() != GLEW_OK)
        std::cout << "GLEW initialisation failure" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    //vertex buffer stuff
    float positions[6] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);





    //shader stuff
    GLint success;
    GLchar infoLog[512];
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader stuff
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

    }
    
    

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShader);
    


    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEvents();

        glUseProgram(shaderProgram);

    }

    glfwTerminate();
    return 0;
}