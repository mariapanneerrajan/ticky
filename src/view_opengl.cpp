#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "view_opengl.h"

struct Quad
{
    float vertex_data[20];
    float anchor_x;
    float anchor_y;
    float width;
    float height;
};

struct Game_Board_Quads
{
    Quad array[9]
};

Quad create_quad(float anchor_x, float anchor_y, float width, float height)
{
    float cell_vertices[20] =
    {
        anchor_x,          anchor_y,           0.0f, 0.0f, 0.0f,  // bottom left 
        anchor_x + width,  anchor_y,           0.0f, 1.0f, 0.0f,  // bottom right
        anchor_x + width,  anchor_y + height,  0.0f, 1.0f, 1.0f,  // top right
        anchor_x,          anchor_y + height,  0.0f, 0.0f, 1.0f,  // top left
    };
    
    Quad quad;
    for(char j=0; j < 20; j++)    
        quad.vertex_data[j] = cell_vertices[j];

    quad.anchor_x = ((anchor_x * (1.0f)) + 1.0f)/2.0f;
    quad.anchor_y = ((anchor_y * (-1.0f)) + 1.0f)/2.0f;
    quad.width = width/2.0f;
    quad.height = height/2.0f;

    return quad;
} 

Game_Board_Quads create_game_board_quads(float input_anchor_x, float input_anchor_y, float input_width, float input_height, float input_padding_x, float input_padding_y)
{
    // achor is on the bottom left
    // float original_anchor_x = -0.65f;
    // float original_anchor_y = 0.35f;
    float original_anchor_x = input_anchor_x;
    float original_anchor_y = input_anchor_y;
    float anchor_x = original_anchor_x;
    float anchor_y = original_anchor_y;
    float width = input_width;
    float height = input_height;
    // float padding_x = 0.1f;
    // float padding_y = 0.2f;
    float padding_x = padding_x;
    float padding_y = padding_y;
    
    Quad quad;
    Game_Board_Quads game_board_quads;
    for(char i = 0; i < 9; i++)
    {
        quad = create_quad(anchor_x, anchor_y, width, height);
        game_board_quads.array[i] = quad;

        if (i == 2 || i == 5)
        {
            anchor_x = original_anchor_x;
            anchor_y -= height + padding_y;
        }
        else        
            anchor_x += width + padding_x;
    }

    return game_board_quads;
}


void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoord = aTexCoord;"
    "}\0";
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "    FragColor = texture(ourTexture, TexCoord);"
    "}\n\0";


namespace ticky
{
    View_Opengl::View_Opengl()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ticky", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, _framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }        

        // build and compile our shader program
        // ------------------------------------        
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);        
        unsigned int shaderProgram = glCreateProgram();
        
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        glShaderSource(fragmentShader, 1, &fragmentShader1Source, NULL);
        glCompileShader(fragmentShader);

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);        
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);
        

        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)            
                break;
            
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }


    

    void View_Opengl::welcome_screen()
    {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        unsigned int texture_id = _create_texture("./res/welcome_screen.jpg");
        glBindTexture(GL_TEXTURE_2D, texture_id);


        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;

            
            glClear(GL_COLOR_BUFFER_BIT);

            
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

    void View_Opengl::show_instructions()
    {

        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        
    }

    void View_Opengl::draw_board(std::vector<char> board_cells_data)
    {
        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
                
    }

    int View_Opengl::get_player_X_input()
    {
        
    }

    int View_Opengl::get_player_O_input()
    {
        
    }

    void View_Opengl::show_end_screen()
    {
        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

    unsigned int View_Opengl::_create_texture(const char* filename)
    {
        unsigned int texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        return texture_id;
    }

}


void _framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}