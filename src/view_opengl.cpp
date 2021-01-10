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
    Quad quads[9];
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
    
    // quad.anchor_x = anchor_x;
    // quad.anchor_y = anchor_y;
    // quad.width = width;
    // quad.height = height;

    quad.anchor_x = ((anchor_x * (1.0f)) + 1.0f)/2.0f;
    quad.anchor_y = ((anchor_y * (-1.0f)) + 1.0f)/2.0f;
    quad.width = width/2.0f;
    quad.height = height/2.0f;

    return quad;
}




Game_Board_Quads create_game_board_quads(float input_anchor_x, float input_anchor_y, float input_width, float input_height, float input_padding_x, float input_padding_y)
{
    float original_anchor_x = input_anchor_x;
    float original_anchor_y = input_anchor_y;
    float anchor_x = original_anchor_x;
    float anchor_y = original_anchor_y;
    float width = input_width;
    float height = input_height;
    float padding_x = input_padding_x;
    float padding_y = input_padding_y;
    
    Quad quad;
    Game_Board_Quads game_board_quads;
    for(char i = 0; i < 9; i++)
    {
        quad = create_quad(anchor_x, anchor_y, width, height);
        game_board_quads.quads[i] = quad;
        
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

        Quad welcome_screen_quad = create_quad(-1.0f, -1.0f, 2.0f,2.0f);
        unsigned int VAO = _create_vertex_array_object(welcome_screen_quad.vertex_data);
        unsigned int texture_id = _create_texture("./res/welcome_screen.jpg");

        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {   
            glClear(GL_COLOR_BUFFER_BIT);

            glBindTexture(GL_TEXTURE_2D, texture_id);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            

            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

    void View_Opengl::show_instructions()
    {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

        Quad instruction_screen_quad = create_quad(-1.0f, -1.0f, 2.0f,2.0f);
        unsigned int VAO = _create_vertex_array_object(instruction_screen_quad.vertex_data);
        unsigned int texture_id = _create_texture("./res/instruction_screen.jpg");

        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void View_Opengl::draw_board(std::vector<char> board_cells_data)
    {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        char num_of_cells  = 9;
        Game_Board_Quads game_board_quads = create_game_board_quads(-0.65f, 0.35f, 0.35f, 0.35f, 0.1f, 0.2f);

        unsigned int VAOs[num_of_cells];
        for(char i = 0; i < num_of_cells; i++)
            VAOs[i] = _create_vertex_array_object(game_board_quads.quads[i].vertex_data);

        // unsigned int VBOs[num_of_cells], VAOs[num_of_cells], EBO;
        // glGenVertexArrays(num_of_cells, VAOs);
        // glGenBuffers(num_of_cells, VBOs);
        // glGenBuffers(1, &EBO);
        
        // // Draw Cells
        // // --------------------        
        // for(char i=0; i < num_of_cells; i++ )
        // {
        //     glBindVertexArray(VAOs[i]);
        //     glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        //     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, game_board_quads.quads[i].vertex_data, GL_STATIC_DRAW);

        //     // Positoin Attribute
        //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        //     glEnableVertexAttribArray(0);
        //     // Texture Attribute
        //     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	
        //     glEnableVertexAttribArray(1);
        //     //glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines

        //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_vertex_indices), quad_vertex_indices, GL_STATIC_DRAW);
        // }

        unsigned int texture_id_empty = _create_texture("./res/empty_cell.jpg");
        unsigned int texture_id_symbol_X = _create_texture("./res/symbol_X.jpg");
        unsigned int texture_id_symbol_O = _create_texture("./res/symbol_O.jpg");

        glBindTexture(GL_TEXTURE_2D, texture_id_symbol_X);

        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            
            if (_cell_pressed(game_board_quads))
                break;

            glClear(GL_COLOR_BUFFER_BIT);

            for(char i=0; i < num_of_cells; i++)
            {
                if(board_cells_data[i] == 0)
                    glBindTexture(GL_TEXTURE_2D, texture_id_empty);
                else if(board_cells_data[i] == 1)
                    glBindTexture(GL_TEXTURE_2D, texture_id_symbol_X);
                else if(board_cells_data[i] == 2)
                    glBindTexture(GL_TEXTURE_2D, texture_id_symbol_O);
                
                glBindVertexArray(VAOs[i]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    bool View_Opengl::_cell_pressed(Game_Board_Quads game_board_quads)
    {
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
                anchor_x = game_board_quads.quads[i].anchor_x * m_viewport[2];
                anchor_y = game_board_quads.quads[i].anchor_y * m_viewport[3];
                width = game_board_quads.quads[i].width *m_viewport[2];
                height = game_board_quads.quads[i].height *m_viewport[3];

                if(
                    x_pos > anchor_x &&
                    x_pos < (anchor_x + width) &&
                    y_pos < anchor_y &&
                    y_pos > (anchor_y - height)
                )
                {
                    user_input = i;
                    return true;
                }
            }
        }
        return false;
    }

    int View_Opengl::get_player_X_input()
    {
        return user_input + 1;
    }

    int View_Opengl::get_player_O_input()
    {
        return user_input + 1;
    }

    void View_Opengl::show_end_screen()
    {
        Quad end_screen_quad = create_quad(-1.0f, -1.0f, 2.0f,2.0f);
        unsigned int VAO = _create_vertex_array_object(end_screen_quad.vertex_data);
        unsigned int texture_id = _create_texture("./res/end_screen.jpg");

        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            if (
                glfwGetTime() > 1.0 &&
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            )
                break;

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
        stbi_set_flip_vertically_on_load(true);
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


    unsigned int View_Opengl::_create_vertex_array_object(float vertex_data[20])
    {
        unsigned int VBO, VAO, EBO;
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);        
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertex_data, GL_STATIC_DRAW);

        // Positoin Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture Attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	
        glEnableVertexAttribArray(1);
        //glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_vertex_indices), quad_vertex_indices, GL_STATIC_DRAW);

        return VAO;
    }

}


void _framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
