#pragma once

#include <vector>

namespace ticky
{
    class View
    {
    public:
        void virtual draw_board(std::vector<char> board_cells_data) = 0;
        void virtual welcome_screen() = 0;

        void virtual show_instructions() = 0;

        int virtual get_player_X_input() = 0;
        int virtual get_player_O_input() = 0;

        void virtual show_end_screen() = 0;

    };
}