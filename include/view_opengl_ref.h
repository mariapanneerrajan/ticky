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
        std::vector<char> m_board_cells_data;
        //void _process_input(GLFWwindow *window, All_Cell_Vertices all_cells_vertices);
    public:
        View_Opengl();

        void draw_board(std::vector<char> board_cells_data) override;
        
        void welcome_screen() override;
        void show_instructions() override;

        int get_player_X_input() override;
        int get_player_O_input() override;

        void show_end_screen() override;

    };
}