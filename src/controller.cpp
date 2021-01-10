#include "controller.h"
#include "board.h"
#include "view.h"
#include <iostream>

namespace ticky
{

    Controller::Controller(std::shared_ptr<components::Board> board,  std::shared_ptr<ticky::View> view)
    {
        m_board = board;
        m_view = view;
    }

    void Controller::start_game()
    {
        int player_input = -1;
        char player_X_turn = 1; 
        
        m_view->welcome_screen();
        m_view->show_instructions();

        while(m_game_over == false)
        {
            m_view->draw_board(
                m_board->get_cells()
            );

            if(m_board->are_all_cells_occupied())
                break;

            if(player_X_turn)            
                player_input = m_view->get_player_X_input();
            else            
                player_input = m_view->get_player_O_input();
            
            if(player_input == -1)
                continue;
            else if(player_input == 0)
                break;

            char index_board_position = player_input - 1;
            
            if(m_board->is_cell_available(index_board_position) == false)            
                continue;

            if(player_X_turn)
            {
                m_board->place_X_at(index_board_position);
                player_X_turn = 0;
            }
            else
            {
                m_board->place_O_at(index_board_position);
                player_X_turn = 1;
            }
            
        }
        m_view->show_end_screen();
    }

    Controller::~Controller(){}
}
