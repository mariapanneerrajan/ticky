#pragma once

#include "view.h"
#include <vector>

namespace ticky
{
    class View_Terminal: public View
    {
    private:
        int _get_input();
    public:
        void draw_board(std::vector<char> board_cells_data) override;
        
        void welcome_screen() override;
        void show_instructions() override;

        int get_player_X_input() override;
        int get_player_O_input() override;

        void show_end_screen() override;

    };
}