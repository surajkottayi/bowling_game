#include <iostream>
#include "CBowlingGame.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <iostream>
#include <thread>
#include <chrono>

void showSpinner(const std::string &message, int durationMs, int intervalMs)
{
    const char spinner[] = {'|', '/', '-', '\\'};
    int spinnerIndex = 0;
    int totalSteps = durationMs / intervalMs;

    std::cout << "\033[1m"; // Bold on
    for (int i = 0; i < totalSteps; ++i)
    {
        std::cout << "\r" << message << " " << spinner[spinnerIndex] << std::flush;
        spinnerIndex = (spinnerIndex + 1) % 4;
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
    std::cout << "\033[0m" << std::endl; // Reset
}

void showDotAnimation(const std::string &message, int durationMs, int intervalMs)
{
    int totalSteps = durationMs / intervalMs;
    int dotCount = 0;

    std::cout << "\033[1m"; // Bold on
    for (int i = 0; i < totalSteps; ++i)
    {
        dotCount = (dotCount % 3) + 1;
        std::string dots(dotCount, '.');
        std::cout << "\r" << message << dots << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
    std::cout << "\033[0m" << std::endl; // Reset
}

bool getYes(const std::string &prompt)
{
    char lchChoice;
    while (true)
    {
        std::cout << prompt << " (y/n): ";
        std::cin >> lchChoice;
        lchChoice = std::tolower(lchChoice);

        if (lchChoice == 'y')
            return true;
        if (lchChoice == 'n')
            return false;

        std::cout << "Please enter 'y' or 'n'." << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    std::shared_ptr<CBowlingGame> lpCBwlngGame = CBowlingGame::getInstance();

    int lRollCount = 1;
    while (true)
    {
        showSpinner("Loading game", 1000, 150);
        showDotAnimation("Preparing score board", 1000, 400);
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;
        std::cout
            << BOLDYELLOW << "\t\t\t\tBowling Game Started! Enter pins for each roll (0-10):" << RESET << std ::endl;
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;
        while (true)
        {
            int liFrameNo = lpCBwlngGame->getCurrentFrameIndex();
            if (liFrameNo >= MAX_NO_FRAMES)
            {
                break;
            }
            else
            {
                std::cout << "Roll 1: ";
                int liPin1;
                std::cin >> liPin1;

                if (liPin1 < 0 || liPin1 > 10)
                {
                    std::cout << "Invalid pin count. Try again.\n";
                    continue;
                }

                lpCBwlngGame->roll(liPin1);

                if (liPin1 != 10) // Not a strike
                {
                    std::cout << "Roll 2: ";
                    int liPin2;
                    std::cin >> liPin2;

                    if (liPin1 + liPin2 > 10 || liPin2 < 0)
                    {
                        std::cout << "Invalid pin total. Try again.\n";
                        lpCBwlngGame->roll(0); // dummy roll for cleanup
                        continue;
                    }

                    lpCBwlngGame->roll(liPin2);
                }
            }

            lpCBwlngGame->printScore();
        }
        std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;

        std::cout << BOLDYELLOW << "\t\t\t\tGame Over !\n ";
        std::cout
            << BOLDRED << "*********************************************************************************************************************" << RESET << std ::endl;

        if (!getYes("Would you like to continue?"))
        {
            std::cout << BOLDYELLOW << "\nExiting...Hope u had a good time !\n ";
            break;
        }

        lpCBwlngGame->clear();
    }
    return 0;
}
