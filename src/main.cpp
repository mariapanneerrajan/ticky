#include <iostream>
#include <memory>
#include "board.h"
#include "view_terminal.h"
#include "view_opengl.h"
#include "controller.h"


int main(int argc, char** argv)
{
    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();    

    std::shared_ptr<ticky::Controller> controller;
    

    if(argc > 1)
    {
        for(char i = 0; i < argc; i++)
        {
            std::cout << argv[i] << std::endl;
        }
    }

    if(
        argc > 1 &&
        argv[1][0] == '-' &&
        argv[1][1] == 't'
    )
    {
        std::shared_ptr<ticky::View_Terminal> view_terminal = std::make_shared<ticky::View_Terminal>();        
        std::shared_ptr<ticky::Controller> controller = std::make_shared<ticky::Controller>(board, view_terminal);
        controller->start_game();
    }
    else
    {
        std::shared_ptr<ticky::View_Opengl> view_opengl = std::make_shared<ticky::View_Opengl>();
        std::shared_ptr<ticky::Controller> controller = std::make_shared<ticky::Controller>(board, view_opengl);
        controller->start_game();
    }
    
}