#pragma once

#include "board.h"
#include "view.h"
#include <memory>

namespace ticky
{
    class Controller
    {
    private:
        bool m_game_over = false;

    public:
        std::shared_ptr<components::Board> m_board;
        std::shared_ptr<ticky::View> m_view;

        Controller(std::shared_ptr<components::Board> board,  std::shared_ptr<ticky::View> view);
        void start_game();
        ~Controller();
    };
}