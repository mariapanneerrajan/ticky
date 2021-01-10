#include <iostream>
#include <memory>
#include "assert.h"
#include "board.h"
#include "view_terminal.h"
#include "view_opengl.h"
#include "controller.h"

#define log(value) std::cout << value << std::endl;

void test_board_num_of_cells()
{
    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();
    assert(board->get_num_of_cells() == 9);
}

void test_view_terminal_draw_board()
{
    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();
    board->place_X_at(2);
    board->place_O_at(4);
    board->place_X_at(0);
    board->place_O_at(1);
    
    std::shared_ptr<ticky::View_Terminal> view_terminal = std::make_shared<ticky::View_Terminal>();    
    view_terminal->draw_board(board->get_cells());
}

void test_controller_draw_board()
{
    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();
    std::shared_ptr<ticky::View_Terminal> view_terminal = std::make_shared<ticky::View_Terminal>();        

    std::shared_ptr<ticky::Controller> controller = std::make_shared<ticky::Controller>(board, view_terminal);

    controller->m_view->draw_board(
        controller->m_board->get_cells()
    );
}

void test_controller_game_play()
{
    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();
    std::shared_ptr<ticky::View_Terminal> view_terminal = std::make_shared<ticky::View_Terminal>();        
    std::shared_ptr<ticky::Controller> controller = std::make_shared<ticky::Controller>(board, view_terminal);
    
    controller->start_game();
}

void test_view_opengl_draw_board()
{

    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();
    std::shared_ptr<ticky::View_Opengl> view_opengl = std::make_shared<ticky::View_Opengl>();
    std::shared_ptr<ticky::Controller> controller = std::make_shared<ticky::Controller>(board, view_opengl);
    
    controller->m_view->welcome_screen();
    controller->m_view->show_instructions();
}

int main()
{
    // test_board_num_of_cells();
    // test_view_terminal_draw_board();
    // test_controller_draw_board();
    // test_controller_game_play();
    
    test_view_opengl_draw_board();
}
