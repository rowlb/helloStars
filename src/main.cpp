

#include "init.h"
#include "types.h"
#include <vector>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// find stars where user clicks
void MouseClickCallback (GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        Window* screen = static_cast<Window*>(glfwGetWindowUserPointer(window)); // getting screen size

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        std::cout << xpos << ", " << ypos << std::endl;



        
    }
}

int main() {
    double FOV = 90;
    Window screen = {640, 480, FOV};
    
    // double FOVht = screenHeight * (FOV/screenWidth);
    // double RAdegreesInPxl = FOVht / screenHeight;
    // double DEdegreesInPxl = FOV / screenWidth;

    // std::cout << "FOVht: " << FOVht << std::endl;
    // std::cout << "RAdegreesInPxl: " << RAdegreesInPxl << std::endl;
    // std::cout << "DEdegreesInPxl: " << DEdegreesInPxl << std::endl;

    std::cout << "Initialising from csv..." << std::endl; 
    InitResult lists = init("stars.csv");
    std::cout << "Initialisation complete." << std::endl; 

    
    
    // random testing of stars in a location
    int randRA = rand() % 360;
    int randDE = rand() % 180;
    std::vector<DegreeNode> testLoc = lists.starLocs.at(randRA).at(randDE);

    std::cout << "Testing location " << randRA << " | " << randDE-90 << std::endl;
    
    for (DegreeNode star : testLoc) {
        std::cout << star.id << ", " << star.RAdegree << "|" << star.DEdegree << ", " << lists.stars.at(star.id).vMag << std::endl;
    }

    
    GLFWwindow* window;

    glfwSetWindowUserPointer(window, &screen);  // store size

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


    glfwSetMouseButtonCallback(window, MouseClickCallback);


    if (glewInit() != GLEW_OK)
        std::cout << "GLEW initialisation failure" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEvents();


    }

    glfwTerminate();
    return 0;
}