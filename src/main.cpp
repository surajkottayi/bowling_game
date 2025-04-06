#include <iostream>
#include "CBowlingGame.hpp"
void startBowling(std::shared_ptr<CBowlingGame> &lpCBwlngGame, int &retFlag);
int main(int argc, char const *argv[])
{
    /* code */
    std::shared_ptr<CBowlingGame> lpCBwlngGame = CBowlingGame::getInstance();

    int lRollCount = 1;
    while (true)
    {
        lpCBwlngGame->init();
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;
        std::cout
            << BOLDYELLOW << "\t\t\t\tBowling Game Started! Enter pins for each roll (0-10):" << RESET << std ::endl;
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;
        while (true)
        {
            int retFlag;
            startBowling(lpCBwlngGame, retFlag);
            if (retFlag == 2)
                break;
            if (retFlag == 3)
                continue;
        }
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;

        std::cout << BOLDYELLOW << "\t\t\t\t\t\t\tGame Over !\n ";
        std::cout << BOLDGREEN << "\t\t\t\t\t\t\tFinal Score is :" << lpCBwlngGame->getFinalScore() << RESET << std::endl;
        std::cout
            << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;

        if (not lpCBwlngGame->getYes("Would you like to continue?"))
        {
            std::cout << BOLDYELLOW << "\nExiting...Hope u had a good time !\n ";
            break;
        }

        lpCBwlngGame->clear();
    }
    return 0;
}

void startBowling(std::shared_ptr<CBowlingGame> &lpCBwlngGame, int &retFlag)
{
    retFlag = 1;

    int liFrameNo = lpCBwlngGame->getCurrentFrameIndex();
    if (liFrameNo >= MAX_NO_FRAMES)
    {
        if ((MAX_NO_FRAMES == liFrameNo) && lpCBwlngGame->isAnotherRoll())
        {
            // Roll 3 : Special Case
            lpCBwlngGame->setisAnotherRollHandled(true);
            std::cout << "Another roll: ";
            std::cout << "Roll 3: ";
            int liPin3;
            std::cin >> liPin3;
            if (liPin3 < 0 || liPin3 > 10)
            {
                std::cout << "Invalid pin count. Try again.\n";
                {
                    retFlag = 3;
                    return;
                };
            }

            lpCBwlngGame->roll(liPin3);
        }
        else
        {
            {
                retFlag = 2;
                return;
            };
        }
    }
    else
    {
        // Roll 1
        std::cout << "Roll 1: ";
        int liPin1;
        std::cin >> liPin1;

        if (liPin1 < 0 || liPin1 > 10)
        {
            std::cout << "Invalid pin count. Try again.\n";
            {
                retFlag = 3;
                return;
            };
        }

        lpCBwlngGame->roll(liPin1);

        if (liPin1 != 10) // Not a strike
        {
            // Roll 2
            std::cout << "Roll 2: ";
            int liPin2;
            std::cin >> liPin2;

            if (liPin1 + liPin2 > 10 || liPin2 < 0)
            {
                std::cout << "Invalid pin total. Try again.\n";
                lpCBwlngGame->roll(0); // dummy roll for cleanup
                {
                    retFlag = 3;
                    return;
                };
            }

            lpCBwlngGame->roll(liPin2);
        }
    }

    lpCBwlngGame->printScore(liFrameNo);
}
