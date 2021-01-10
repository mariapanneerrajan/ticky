#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "view.h"
#include <vector>

struct All_Cell_Vertices;
namespace ticky
{
    class View_Opengl: public View
    {
    private:
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;

        GLFWwindow* window;

        unsigned int _create_texture(const char*);

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