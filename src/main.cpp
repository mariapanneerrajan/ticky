#include <memory>
#include "board.h"


int main()
{
    std::shared_ptr<components::Board> board = std::make_shared<components::Board>();
}