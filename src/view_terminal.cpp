#include <iostream>
#include "view_terminal.h"

namespace ticky
{
    void View_Terminal::draw_board(std::vector<char> board_cells_data)
    {
        char index = 0;

        for(char value : board_cells_data)
        {
            // 0 1 2
            // 3 4 5
            // 6 7 8
            if((index % 3) == 0)
            {
                std::cout << "\n";
            }
            
            std::cout << (int)value << " ";
            
            index ++;
        }
        std::cout << "\n\n";
    }

    void View_Terminal::welcome_screen()
    {
        std::cout << "\n++++++++++++++++++" << std::endl;
        std::cout << "Welcome to Ticky!!" << std::endl;
        std::cout << "++++++++++++++++++\n" << std::endl;
    }

    void View_Terminal::show_instructions()
    {
        std::cout << "Enter any of the following numers to capture a cell:" << std::endl;
        std::cout << "1 - Top Left" << std::endl;
        std::cout << "2 - Top Middle" << std::endl;
        std::cout << "3 - Top Right" << std::endl;
        std::cout << "4 - Middle Left" << std::endl;
        std::cout << "5 - Middle Middle" << std::endl;
        std::cout << "6 - Middle Right" << std::endl;
        std::cout << "7 - Bottom Left" << std::endl;
        std::cout << "8 - Bottom Middle" << std::endl;
        std::cout << "9 - Bottom Right" << std::endl;
        std::cout << "0 - Exit" << std::endl;
    }

    int View_Terminal::_get_input()
    {
        int input = -1;
        std::cin >> input;
        if ((input > 9) || input < 0)
        {
            return -1;
        }
        return input;
    }

    int View_Terminal::get_player_X_input()
    {
        std::cout << "\nPlayer 'X' turn:" << std::endl;
        return _get_input();
    }

    int View_Terminal::get_player_O_input()
    {
        std::cout << "\nPlayer 'O' turn:" << std::endl;
        return _get_input();
    }

    void View_Terminal::show_end_screen()
    {
        std::cout << "\nThanks for Playing Ticky! :)" << std::endl;
        std::cout << "Have a Great Day!!\n" << std::endl;
    }
}