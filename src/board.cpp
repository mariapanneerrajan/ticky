#include <iostream>
#include "board.h"

namespace components
{
    Board::Board()
    {
        //std::cout << "Board Created" << std::endl;
        num_of_cells = 9;    
        cells.reserve(num_of_cells);
        cells = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        
    }

    Board::~Board()
    {
        //std::cout << "Board Destroyed" << std::endl;
    }

    char Board::get_num_of_cells()
    {
        return num_of_cells;
    }

    std::vector<char> Board::get_cells()
    {
        return cells;
    }

    void Board::place_X_at(char position)
    {
        //cells.insert(cells.begin() + position, 1);
        cells[position] = 1;
        num_of_cells_occupied++;
    }

    void Board::place_O_at(char position)
    {
        //cells.insert(cells.begin() + position, 2);
        cells[position] = 2;
        num_of_cells_occupied++;
    }

    bool Board::is_cell_available(char position)
    {
        if(cells[position] == 0)
        {
            return true;
        }
        return false;
    }

    bool Board::are_all_cells_occupied()
    {
        if(num_of_cells_occupied == num_of_cells)
            return true;
        
        return false;
    }
}
