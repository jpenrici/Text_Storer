#include "controller.hpp"
#include <iostream>

int main()
{
    if (Controller::Instance()->Gui() == EXIT_FAILURE) {
        std::cout << "TextStorer failure!\n";
        return -1;
    }

    return 0;
}
