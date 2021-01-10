#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "view.h"
#include <vector>

struct Game_Board_Quads;

namespace ticky
{
    class View_Opengl: public View
    {
    private:
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;

        char user_input = -1;

        GLFWwindow* window;

        unsigned int _create_texture(const char*);
        unsigned int _create_vertex_array_object(float vertex_data[20]);

        unsigned int quad_vertex_indices[6] = {
            0, 1, 2, // first triangle
            3, 2, 0 // first box
        };

        bool _cell_pressed(Game_Board_Quads game_board_quads);

    public:        
        View_Opengl();

        

        //void View_Opengl::is_canvas_open();

        void draw_board(std::vector<char> board_cells_data) override;
        
        void welcome_screen() override;
        void show_instructions() override;

        int get_player_X_input() override;
        int get_player_O_input() override;

        void show_end_screen() override;

    };
}