

#include "init.h"
#include "types.h"
#include <vector>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

InitResult starData;

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
        double facingDegHorizontal = screen->xfacing; // between 0 to 360
        double facingDegVertical = screen->yfacing; // between -90 to 90

        std::cout << "Facing: " << facingDegHorizontal << ", " << facingDegVertical << std::endl;  

        // how many degrees the click is from center
        double ydegreeOffset = FOVht * (ypos / screen->height) - FOVht / 2;
        double xdegreeOffset = screen->FOV * (xpos / screen->width) - screen->FOV / 2;

        // final degree coords of click 
        int xdegree = facingDegHorizontal + xdegreeOffset;
        int ydegree = facingDegVertical - ydegreeOffset;

        std::cout << "Degree coords: " << xdegree << ", " << ydegree << std::endl;

        
        // print stars nearby
        std::vector<DegreeNode> testLoc = starData.starLocs.at(xdegree).at(ydegree+90);
    
        for (DegreeNode star : testLoc) {
            std::cout << star.id << ", " << star.RAdegree << "|" << star.DEdegree << ", " << starData.stars.at(star.id).vMag;
            if (starData.stars.at(star.id).name.length() > 0) {
                std::cout << ", " << starData.stars.at(star.id).name;
            }
            std::cout << std::endl;
        }
        

    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int panAmount = 10;
    Window* screen = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        screen->xfacing = (screen->xfacing + panAmount) % 360;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        if (screen->xfacing < panAmount) {
            screen->xfacing += 360;
        }
        screen->xfacing = (screen->xfacing - panAmount) % 360;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        screen->yfacing = (screen->yfacing - panAmount) % 180;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        screen->yfacing = (screen->yfacing + panAmount) % 180;
    }
    std::cout << screen->xfacing << ", " << screen->yfacing << std::endl;
    //glfwSetWindowUserPointer(window, &newScreen);
}

int main() {
    double FOV = 90;
    
    Window screen = {800, 600, FOV, 180, 0};
  
    
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
    glfwSetKeyCallback(window, keyCallback);


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