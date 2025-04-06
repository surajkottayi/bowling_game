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
int main(int argc, char *argv[])
{
    std::shared_ptr<CBowlingGame> lpCBwlngGame = CBowlingGame::getInstance();
    if (argc > 1)
    {
        std::string lstrArgFirst(argv[1]);
        if (lstrArgFirst == "--mode=test")
        {
            std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
            std::cout << BOLDRED << "\t\t\t\tTest Mode : Bowling Game Started! Enter pins for each roll (0-10):" << RESET << std::endl;
            std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;

            lpCBwlngGame->init();
            
            // Sample test input
            std::vector<int> lvTestRolls = {
                1, 4, 4, 5, 6, 4, 5, 5, 10, 0, 1, 7, 3, 6, 4, 10, 2, 8, 6};

            for (int pins : lvTestRolls)
            {
                lpCBwlngGame->roll(pins);
            }
            lpCBwlngGame->printScore(lpCBwlngGame->getCurrentFrameIndex() - 1);
            std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
            std::cout << BOLDYELLOW << "\t\t\t\t\t\tGame Over !\n";
            std::cout << BOLDGREEN << "\t\t\t\t\t\tFinal Score is :" << lpCBwlngGame->getFinalScore() << RESET << std::endl;
            std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
        }
    }
    else
    {

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
            std::cout << BOLDYELLOW << "\t\t\t\t\t\tGame Over !\n";
            std::cout << BOLDGREEN << "\t\t\t\t\t\tFinal Score is :" << lpCBwlngGame->getFinalScore() << RESET << std::endl;
            std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;

            // Replay prompt
            if (!lpCBwlngGame->getYes("Would you like to continue?"))
            {
                std::cout << BOLDYELLOW << "\nExiting...Hope u had a good time !\n";
                break;
            }

            lpCBwlngGame->clear();
        }
    }
    return 0;
}

// Function to safely read integer input within range [minVal, maxVal]
int getValidatedInput(const std::string &lstrPrompt, int lMinVal, int lMaxVal)
{
    int liValue;
    while (true)
    {
        std::cout << lstrPrompt;
        std::cin >> liValue;

        if (std::cin.fail())
        {
            std::cin.clear();                                                   // Clear fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
            std::cout << "Invalid input. Please enter a number between " << lMinVal << " and " << lMaxVal << ".\n";
            continue;
        }

        if (liValue < lMinVal || liValue > lMaxVal)
        {
            std::cout << "Invalid pin count. Please enter a number between " << lMinVal << " and " << lMaxVal << ".\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard any remaining input
        return liValue;
    }
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
            int liPin3 = getValidatedInput("Roll 3: ", 0, 10);
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
        int liPin1 = getValidatedInput("Roll 1: ", 0, 10);
        lpCBwlngGame->roll(liPin1);

        // --- Roll 2 (if not a strike) ---
        if (liPin1 != 10)
        {
            int maxRoll2 = 10 - liPin1;
            int liPin2 = getValidatedInput("Roll 2: ", 0, maxRoll2);
            lpCBwlngGame->roll(liPin2);
        }
    }

    // Print score for the current frame
    lpCBwlngGame->printScore(liFrameNo);
}
