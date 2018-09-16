//
// Created by zepheon on 16/09/18.
//

#include "display.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool glfwSetWindowCenter( GLFWwindow * window )
{
    if( !window )
        return false;

    int sx = 0, sy = 0;
    int px = 0, py = 0;
    int mx = 0, my = 0;
    int monitor_count = 0;
    int best_area = 0;
    int final_x = 0, final_y = 0;

    glfwGetWindowSize( window , &sx, &sy );
    glfwGetWindowPos( window , &px, &py );

    // Iterate throug all monitors
    GLFWmonitor ** m = glfwGetMonitors( &monitor_count );
    if( !m )
        return false;

    for( int j = 0; j < monitor_count ; ++j )
    {

        glfwGetMonitorPos( m[j] , &mx, &my );
        const GLFWvidmode * mode = glfwGetVideoMode( m[j] );
        if( !mode )
            continue;

        // Get intersection of two rectangles - screen and window
        int minX = std::max( mx , px );
        int minY = std::max( my , py );

        int maxX = std::min( mx+mode->width , px+sx );
        int maxY = std::min( my+mode->height , py+sy );

        // Calculate area of the intersection
        int area = std::max( maxX - minX , 0 ) * std::max( maxY - minY , 0 );

        // If its bigger than actual (window covers more space on this monitor)
        if( area > best_area )
        {
            // Calculate proper position in this monitor
            final_x = mx + (mode->width-sx)/2;
            final_y = my + (mode->height-sy)/2;

            best_area = area;
        }

    }

    // We found something
    if( best_area )
        glfwSetWindowPos( window , final_x , final_y );

        // Something is wrong - current window has NOT any intersection with any monitors. Move it to the default one.
    else
    {
        GLFWmonitor * primary = glfwGetPrimaryMonitor( );
        if( primary )
        {
            const GLFWvidmode * desktop = glfwGetVideoMode( primary );

            if( desktop )
                glfwSetWindowPos( window , (desktop->width-sx)/2 , (desktop->height-sy)/2 );
            else
                return false;
        }
        else
            return false;
    }

    return true;
}

void Display::init() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "BoyBoy", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowCenter(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    GLenum err = glewInit();
    std::string errstr;
    if (err != GLEW_OK)
    {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    }

//    auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());



    glViewport(0, 0, WIDTH, HEIGHT);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
}