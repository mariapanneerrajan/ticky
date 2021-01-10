#include <iostream>
#include <vector>

#include "view_opengl.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

const char num_of_cells = 9;

struct All_Cell_Vertices
{
    float array_2d[9][20];
    float anchor_x[9];
    float anchor_y[9];
    float width[9];
    float height[9];
};

struct Cell
{
    float position_x;
    float position_y;
    float width;
    float height;
};

All_Cell_Vertices create_all_cell_vertices()
{
    // achor is on the bottom left
    float original_anchor_x = -0.65f;
    float original_anchor_y = 0.35f;
    float anchor_x = original_anchor_x;
    float anchor_y = original_anchor_y;
    float width = 0.35f;
    float height = 0.35f;
    float padding_x = 0.1f;
    float padding_y = 0.2f;
    
    All_Cell_Vertices all_cell_vertices;

    for(char i = 0; i < num_of_cells; i++)
    {
        float cell_vertices[20] = {
            anchor_x,          anchor_y,           0.0f, 0.0f, 0.0f,  // bottom left 
            anchor_x + width,  anchor_y,           0.0f, 1.0f, 0.0f,  // bottom right
            anchor_x + width,  anchor_y + height,  0.0f, 1.0f, 1.0f,  // top right
            anchor_x,          anchor_y + height,  0.0f, 0.0f, 1.0f,  // top left
        };
        
        all_cell_vertices.anchor_x[i] = ((anchor_x * (1.0f)) + 1.0f)/2.0f;
        all_cell_vertices.anchor_y[i] = ((anchor_y * (-1.0f)) + 1.0f)/2.0f;
        all_cell_vertices.width[i] = width/2.0f;
        all_cell_vertices.height[i] = height/2.0f;
        for(char j=0; j < 20; j++)
        {
            all_cell_vertices.array_2d[i][j] = cell_vertices[j];            
        }
        

        if (i == 2 || i == 5)
        {
            anchor_x = original_anchor_x;
            anchor_y -= height + padding_y;
        }
        else
        {
            anchor_x += width + padding_x;
        }
        
    }

    return all_cell_vertices;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window, All_Cell_Vertices all_cell_vertices);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";


namespace ticky
{
    void View_Opengl::draw_board(std::vector<char> board_cells_data)
    {
        m_board_cells_data = board_cells_data;

        for(char value : m_board_cells_data)
        {
            std::cout << value << std::endl;
        }
    }


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
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ticky", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        // build and compile our shader program
        // ------------------------------------        
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
        unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
        unsigned int shaderProgramOrange = glCreateProgram();
        unsigned int shaderProgramYellow = glCreateProgram();
        
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
        glCompileShader(fragmentShaderOrange);
        
        int compile_status;
        glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &compile_status);
        // if(compile_status != GL_TRUE)
        // {
        //     int info_log_length;
        //     glGetShaderiv(fragmentShaderOrange, GL_INFO_LOG_LENGTH, &info_log_length);
        //     char* buffer = new char[info_log_length];

        //     int buffer_size;
        //     glGetShaderInfoLog(fragmentShaderOrange, info_log_length, &buffer_size, buffer);
        //     std::cout << buffer << std::endl;

        //     delete []buffer;
        // }

        glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
        glCompileShader(fragmentShaderYellow);

        glAttachShader(shaderProgramOrange, vertexShader);
        glAttachShader(shaderProgramOrange, fragmentShaderOrange);        
        glLinkProgram(shaderProgramOrange);
        
        glAttachShader(shaderProgramYellow, vertexShader);
        glAttachShader(shaderProgramYellow, fragmentShaderYellow);
        glLinkProgram(shaderProgramYellow);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float first_box[] = {
            -0.9f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left 
            -0.0f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
             0.0f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
            -0.9f,  0.5f, 0.0f, 0.0f, 1.0f  // top left
        };
        All_Cell_Vertices all_cell_vertices = create_all_cell_vertices();

        unsigned int cell_vertex_indices[] = {
            0, 1, 2, // first triangle
            3, 2, 0 // first box
        };

        float secondTriangle[] = {
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f   // top 
        };
        unsigned int VBOs[num_of_cells], VAOs[num_of_cells], EBO;
        glGenVertexArrays(num_of_cells, VAOs); // we can also generate multiple VAOs or buffers at the same time
        glGenBuffers(num_of_cells, VBOs);
        glGenBuffers(1, &EBO);
        
        // Draw Cells
        // --------------------        
        for(char i=0; i < num_of_cells; i++ )
        {
            glBindVertexArray(VAOs[i]);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(all_cell_vertices.array_2d[i]), all_cell_vertices.array_2d[i], GL_STATIC_DRAW);

            // Positoin Attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Texture Attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	
            glEnableVertexAttribArray(1);
            //glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cell_vertex_indices), cell_vertex_indices, GL_STATIC_DRAW);
        }



        // Load Textures 
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load("./res/empty_cell.jpg", &width, &height, &nrChannels, 0);
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


        unsigned int texture1;
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        width, height, nrChannels;
        data = stbi_load("./res/symbol_X.jpg", &width, &height, &nrChannels, 0);
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


        unsigned int texture2;
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        width, height, nrChannels;
        data = stbi_load("./res/symbol_X.jpg", &width, &height, &nrChannels, 0);
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

        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            //_process_input(window, all_cell_vertices);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glUseProgram(shaderProgramOrange);
            for(char i=0; i < num_of_cells; i++)
            {
                // if (m_board_cells_data[i] == 0)
                //     glBindTexture(GL_TEXTURE_2D, texture);
                // else if(m_board_cells_data[i] == 1)
                //     glBindTexture(GL_TEXTURE_2D, texture1);
                // else if(m_board_cells_data[i] == 2)
                //     glBindTexture(GL_TEXTURE_2D, texture2);
                    
                glBindTexture(GL_TEXTURE_2D, texture);
                glBindVertexArray(VAOs[i]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            // then we draw the second triangle using the data from the second VAO
            // when we draw the second triangle we want to use a different shader program so we switch to the shader program with our yellow fragment shader.
            // glUseProgram(shaderProgramYellow);
            // glBindVertexArray(VAOs[1]);
            // glDrawArrays(GL_TRIANGLES, 0, 3);	// this call should output a yellow triangle

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(2, VAOs);
        glDeleteBuffers(2, VBOs);
        glDeleteBuffers(1, &EBO);
        glDeleteProgram(shaderProgramOrange);
        glDeleteProgram(shaderProgramYellow);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }

    void View_Opengl::welcome_screen()
    {
        
    }

    void View_Opengl::show_instructions()
    {
        
    }

    int View_Opengl::get_player_X_input()
    {
        
    }

    int View_Opengl::get_player_O_input()
    {
        
    }

    void View_Opengl::show_end_screen()
    {
        
    }

    void View_Opengl::_process_input(GLFWwindow *window, All_Cell_Vertices all_cells_vertices)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            double xpos, ypos;        
            glfwGetCursorPos(window, &xpos, &ypos);

            float x_pos = (float)xpos;
            float y_pos = (float)ypos;

            int m_viewport[4];
            glGetIntegerv( GL_VIEWPORT, m_viewport);

            float anchor_x = 0;
            float anchor_y = 0;
            float width = 0;
            float height = 0;

            for(char i = 0; i < 9; i++)
            {
                anchor_x = all_cells_vertices.anchor_x[i] * m_viewport[2];
                anchor_y = all_cells_vertices.anchor_y[i] * m_viewport[3];
                width = all_cells_vertices.width[0]*m_viewport[2];
                height = all_cells_vertices.height[0]*m_viewport[3];

                if(
                    x_pos > anchor_x &&
                    x_pos < (anchor_x + width) &&
                    y_pos < anchor_y &&
                    y_pos > (anchor_y - height)
                )
                {
                    std::cout << "Cell: " << (int)i << std::endl;
                    m_board_cells_data[i] = 1;
                }
            }
        }
    }
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

