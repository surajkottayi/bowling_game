#include <iostream>
#include "CBowlingGame.hpp"

/**
 * @brief Starts a single round of bowling (1 or 2 or 3 rolls).
 *
 * @param lpCBwlngGame Shared pointer to the bowling game instance.
 * @param retFlag Return flag indicating game control:
 * - 1: Continue game
 * - 2: Game over
 * - 3: Retry current frame input
 */
void startBowling(std::shared_ptr<CBowlingGame> &lpCBwlngGame, int &retFlag);

/**
 * @brief Entry point for the Bowling Score Tracker program.
 *
 * Initializes the game and handles main game loop, including replay prompt.
 *
 * @return int Exit status code (0 for success).
 */
int main(int argc, char const *argv[])
{
    std::shared_ptr<CBowlingGame> lpCBwlngGame = CBowlingGame::getInstance();

    while (true)
    {
        lpCBwlngGame->init();

        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
        std::cout << BOLDYELLOW << "\t\t\t\tBowling Game Started! Enter pins for each roll (0-10):" << RESET << std::endl;
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;

        // Frame-by-frame bowling loop
        while (true)
        {
            int retFlag;
            startBowling(lpCBwlngGame, retFlag);
            if (retFlag == 2)
                break; // Game over
            if (retFlag == 3)
                continue; // Retry input
        }

        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
        std::cout << BOLDYELLOW << "\t\t\t\t\t\t\tGame Over !\n";
        std::cout << BOLDGREEN << "\t\t\t\t\t\t\tFinal Score is :" << lpCBwlngGame->getFinalScore() << RESET << std::endl;
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;

        // Replay prompt
        if (!lpCBwlngGame->getYes("Would you like to continue?"))
        {
            std::cout << BOLDYELLOW << "\nExiting...Hope u had a good time !\n";
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
        // Handle potential bonus roll (Frame 10)
        if ((MAX_NO_FRAMES == liFrameNo) && lpCBwlngGame->isAnotherRoll())
        {
            lpCBwlngGame->setisAnotherRollHandled(true);
            std::cout << "Another roll: ";
            std::cout << "Roll 3: ";
            int liPin3;
            std::cin >> liPin3;

            if (liPin3 < 0 || liPin3 > 10)
            {
                std::cout << "Invalid pin count. Try again.\n";
                retFlag = 3;
                return;
            }

            lpCBwlngGame->roll(liPin3);
        }
        else
        {
            retFlag = 2; // Game finished
            return;
        }
    }
    else
    {
        // --- Roll 1 ---
        std::cout << "Roll 1: ";
        int liPin1;
        std::cin >> liPin1;

        if (liPin1 < 0 || liPin1 > 10)
        {
            std::cout << "Invalid pin count. Try again.\n";
            retFlag = 3;
            return;
        }

        lpCBwlngGame->roll(liPin1);

        // --- Roll 2 (if not a strike) ---
        if (liPin1 != 10)
        {
            std::cout << "Roll 2: ";
            int liPin2;
            std::cin >> liPin2;

            if (liPin1 + liPin2 > 10 || liPin2 < 0)
            {
                std::cout << "Invalid pin total. Try again.\n";
                lpCBwlngGame->roll(0); // Dummy roll for clean state
                retFlag = 3;
                return;
            }

            lpCBwlngGame->roll(liPin2);
        }
    }

    // Print score for the current frame
    lpCBwlngGame->printScore(liFrameNo);
}
