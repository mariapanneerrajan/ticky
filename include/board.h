#pragma once
#include <vector>

namespace components
{
    class Board
    {
    private:
        char num_of_cells;
        char num_of_cells_occupied = 0;

    public:
        std::vector<char> cells;

        Board();
        ~Board();

        char get_num_of_cells();

        std::vector<char> get_cells();

        void place_X_at(char position);
        void place_O_at(char position);

        bool is_cell_available(char position);
        bool are_all_cells_occupied();
    };
    
}


