
#include "CBowlingGame.hpp"
using namespace bowling_game;
std::shared_ptr<CBowlingGame> CBowlingGame::m_pCBwlngGame = nullptr;

void CBowlingGame::init()
{
    showSpinner("Loading game", 1000, 100);
    showDotAnimation("Preparing score board", 1000, 100);
    setisAnotherRoll(false);
    setisAnotherRollHandled(false);
    setFinalScore(0);
    m_vRolls.clear();
}
void CBowlingGame::roll(int pins)
{
    m_vRolls.push_back(pins);
}

void CBowlingGame::printFirstRaw(int &liRollIndex) const
{
    for (int iFrameNo = 0; iFrameNo < MAX_NO_FRAMES; ++iFrameNo)
    {
        if (iFrameNo < 9) // Frames 1 to 9
        {
            if (liRollIndex >= m_vRolls.size())
            {
                std::cout << "     |";
                continue;
            }

            int first = m_vRolls[liRollIndex];
            if (first == 10)
            {
                std::cout << "  " << BOLDGREEN << "X" << RESET << "  |";
                liRollIndex += 1;
            }
            else if (liRollIndex + 1 < m_vRolls.size())
            {
                int second = m_vRolls[liRollIndex + 1];
                std::string secondDisplay = (first + second == 10) ? "/" : std::to_string(second);
                std::cout << std::setw(2) << first << "|" << GREEN << secondDisplay << RESET << " |";
                liRollIndex += 2;
            }
            else
            {
                std::cout << std::setw(2) << first << "|   |";
                liRollIndex += 1;
            }
        }
        else // 10th frame (can have 3 rolls)
        {
            std::string rolls[3] = {" ", " ", " "};

            for (int i = 0; i < 3 && liRollIndex + i < m_vRolls.size(); ++i)
            {
                int val = m_vRolls[liRollIndex + i];
                if (val == 10)
                {
                    rolls[i] = "X";
                }
                else if (i > 0 && m_vRolls[liRollIndex + i - 1] + val == 10)
                {
                    rolls[i] = "/";
                }
                else
                {
                    rolls[i] = std::to_string(val);
                }
            }

            std::cout << std::setw(2) << rolls[0] << "|" << rolls[1] << "|" << rolls[2] << " |";
            liRollIndex += std::min(3, static_cast<int>(m_vRolls.size()) - liRollIndex);
        }
    }
}

void CBowlingGame::printSecondRaw(int &liRollIndex, int &currentScore) const
{
    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n|";
    for (int iFrameNo = 0; iFrameNo < MAX_NO_FRAMES; ++iFrameNo)
    {
        if (liRollIndex >= m_vRolls.size())
        {
            std::cout << "     |";
            continue;
        }

        if (iFrameNo < 9)
        {
            if (isStrike(liRollIndex))
            {
                if (liRollIndex + 2 < m_vRolls.size())
                {
                    currentScore += 10 + m_vRolls[liRollIndex + 1] + m_vRolls[liRollIndex + 2];
                    std::cout << std::setw(5) << currentScore << "|";
                }
                else
                {

                    if (MAX_NO_FRAMES == getCurrentFrameIndex())
                    {
                        currentScore += 10 + m_vRolls[liRollIndex + 1] + m_vRolls[liRollIndex + 2];
                        std::cout << std::setw(5) << currentScore << "|";
                    }
                    else
                    {
                        std::cout << "     |";
                    }
                }
                liRollIndex += 1;
            }
            else if (liRollIndex + 1 < m_vRolls.size())
            {
                int frameScore = m_vRolls[liRollIndex] + m_vRolls[liRollIndex + 1];
                if (frameScore == 10 && liRollIndex + 2 < m_vRolls.size())
                {
                    currentScore += 10 + m_vRolls[liRollIndex + 2];
                    std::cout << std::setw(5) << currentScore << "|";
                }
                else if (frameScore < 10)
                {
                    currentScore += frameScore;
                    std::cout << std::setw(5) << currentScore << "|";
                }
                else
                {
                    std::cout << "     |";
                }
                liRollIndex += 2;
            }
            else
            {
                std::cout << "     |";
                liRollIndex += 1;
            }
        }
        else // 10th frame
        {
            if (false == m_IsAnotherRoll)
            {
                if (isStrike(liRollIndex) || isSpare(liRollIndex) && !m_IsAnotherRollHandled)
                {
                    setisAnotherRoll(true);
                }
            }
            else
            {
                setisAnotherRoll(false);
                int frameScore = m_vRolls[liRollIndex] + m_vRolls[liRollIndex + 1] + m_vRolls[liRollIndex + 2];
                currentScore += frameScore;
                std::cout << std::setw(5) << BOLDMAGENTA << currentScore << RESET << "     |";
                m_FinalScore = currentScore;
            }
        }
    }
    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n";
}
bool CBowlingGame::isStrike(int liRollIndex) const
{
    return liRollIndex < m_vRolls.size() && m_vRolls[liRollIndex] == 10;
}

bool CBowlingGame::isSpare(int rollIndex) const
{
    return rollIndex + 1 < m_vRolls.size() &&
           m_vRolls[rollIndex] + m_vRolls[rollIndex + 1] == 10;
}

bool &CBowlingGame::isAnotherRoll() const
{
    return m_IsAnotherRoll;
}

void CBowlingGame::setisAnotherRoll(bool lbRoll) const
{
    m_IsAnotherRoll = lbRoll;
}
void CBowlingGame::setisAnotherRollHandled(bool lbRoll) const
{
    m_IsAnotherRollHandled = lbRoll;
}

void CBowlingGame::setFinalScore(uint32_t &&lScore)
{
    m_FinalScore = lScore;
}

int CBowlingGame::strikeBonus(int rollIndex) const
{
    return rollIndex + 2 < m_vRolls.size() ? m_vRolls[rollIndex + 1] + m_vRolls[rollIndex + 2] : 0;
}

int CBowlingGame::spareBonus(int rollIndex) const
{
    return rollIndex + 2 <= m_vRolls.size() ? m_vRolls[rollIndex + 2 - 1] : 0;
}

int CBowlingGame::sumOfBallsInFrame(int rollIndex) const
{
    return (rollIndex + 1 < m_vRolls.size() ? m_vRolls[rollIndex] + m_vRolls[rollIndex + 1] : m_vRolls[rollIndex]);
}

void CBowlingGame::printScore(int liFrameNo) const
{

    int liRollIndex = 0;
    std::cout << "Frame Info:\n";
    printFrameInfo(liFrameNo, liRollIndex);

    liRollIndex = 0;
    // Print the first raw :  frame,roll score
    printFirstRaw(liRollIndex);

    liRollIndex = 0;
    int currentScore = 0;
    // Print the second raw :  cumilative score/ total score
    printSecondRaw(liRollIndex, currentScore);
}

void CBowlingGame::printFrameInfo(int liFrameNo, int &liRollIndex) const
{
    for (int iFrameNo = 0; iFrameNo < MAX_NO_FRAMES; ++iFrameNo)
    {
        if (iFrameNo == liFrameNo)
        {
            std::cout << BOLDBLUE << "Frame " << (iFrameNo + 1) << RESET << ": ";
        }
        else
        {

            std::cout << "Frame " << (iFrameNo + 1) << ": ";
        }

        if (iFrameNo < (MAX_NO_FRAMES - 1))
        {
            if (liRollIndex < m_vRolls.size())
            {
                std::cout << "Roll 1 = " << m_vRolls[liRollIndex];

                if (isStrike(liRollIndex))
                {
                    std::cout << " (Strike)";
                    liRollIndex++;
                }
                else if (liRollIndex + 1 < m_vRolls.size())
                {
                    std::cout << ", Roll 2 = " << m_vRolls[liRollIndex + 1];
                    liRollIndex += 2;
                }
                else
                {
                    std::cout << ", Roll 2 = _";
                    liRollIndex += 1;
                }
            }
        }
        else // Frame 10
        {
            for (int iIndex = 0; iIndex < 3 && liRollIndex + iIndex < m_vRolls.size(); ++iIndex)
            {
                std::cout << "Roll " << (iIndex + 1) << " = " << m_vRolls[liRollIndex + iIndex] << ", ";
            }
            liRollIndex += 3;
        }

        std::cout << std::endl;
    }
    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n";

    // Print frame numbers
    std::cout << "|";
    for (int iFrameNo = 1; iFrameNo <= MAX_NO_FRAMES; ++iFrameNo)
    {
        if (iFrameNo == liFrameNo + 1)
        {
            if (iFrameNo < MAX_NO_FRAMES)
                std::cout << BOLDBLUE << "  " << iFrameNo << RESET << "  |";
            else
                std::cout << BOLDBLUE << "  " << std::setw(2) << iFrameNo << RESET << "   |"; // 10th frame needs more spacing
        }
        else
        {
            if (iFrameNo < MAX_NO_FRAMES)
                std::cout << "  " << iFrameNo << "  |";
            else
                std::cout << "  " << std::setw(2) << iFrameNo << "   |"; // 10th fr
        }
    }

    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n|" << RESET;
}
int CBowlingGame::getCurrentFrameIndex() const
{
    int liFrameNo = 0;
    int liRollIndex = 0;
    while (liRollIndex < m_vRolls.size() && liFrameNo < MAX_NO_FRAMES)
    {
        if (isStrike(liRollIndex))
            liRollIndex += 1;
        else
            liRollIndex += 2;
        liFrameNo++;
    }
    return liFrameNo;
}

void CBowlingGame::clear()
{
    std::system("clear");
    m_vRolls.clear();
    setisAnotherRoll(false);
    setisAnotherRollHandled(false);
    setFinalScore(0);
}

void CBowlingGame::showSpinner(const std::string &message, int durationMs, int intervalMs)
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

void CBowlingGame::showDotAnimation(const std::string &message, int durationMs, int intervalMs)
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

bool CBowlingGame::getYes(const std::string &lstrPrompt)
{
    char lchChoice;
    while (true)
    {
        std::cout << lstrPrompt << " (y/n): ";
        std::cin >> lchChoice;
        lchChoice = std::tolower(lchChoice);
        if ('y' == lchChoice)
            return true;
        if ('n' == lchChoice)
            return false;

        std::cout << "Please enter 'y' or 'n'." << std::endl;
    }
}

std::shared_ptr<CBowlingGame> CBowlingGame::getInstance()
{
    if (!m_pCBwlngGame)
    {
        m_pCBwlngGame = std::shared_ptr<CBowlingGame>(new CBowlingGame());
    }
    return m_pCBwlngGame;
}

int CBowlingGame::getFinalScore() const
{
    int score = 0;
    int rollIndex = 0;

    for (int frame = 0; frame < MAX_NO_FRAMES; ++frame)
    {
        if (rollIndex >= m_vRolls.size())
            break;

        if (m_vRolls[rollIndex] == 10)
        {
            // Strike
            int bonus1 = (rollIndex + 1 < m_vRolls.size()) ? m_vRolls[rollIndex + 1] : 0;
            int bonus2 = (rollIndex + 2 < m_vRolls.size()) ? m_vRolls[rollIndex + 2] : 0;
            score += 10 + bonus1 + bonus2;
            rollIndex += 1;
        }
        else if (rollIndex + 1 < m_vRolls.size() && m_vRolls[rollIndex] + m_vRolls[rollIndex + 1] == 10)
        {
            // Spare
            int bonus = (rollIndex + 2 < m_vRolls.size()) ? m_vRolls[rollIndex + 2] : 0;
            score += 10 + bonus;
            rollIndex += 2;
        }
        else
        {
            // Open frame
            int roll1 = m_vRolls[rollIndex];
            int roll2 = (rollIndex + 1 < m_vRolls.size()) ? m_vRolls[rollIndex + 1] : 0;
            score += roll1 + roll2;
            rollIndex += 2;
        }
    }

    return score;
}

void CBowlingGame::printGameBegin(const GameMode &lMode)
{
    std::string lstrMode = "";
    std::string lstrColor;
    if (lMode == GameMode::GM_TEST)
    {
        lstrColor = BOLDRED;
        lstrMode = "Test Mode";
    }
    else if (lMode == GameMode::GM_NORMAL)
    {
        lstrColor = BOLDYELLOW;
        lstrMode = "";
    }

    std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
    std::cout << lstrColor << "\t\t\t\t " << lstrMode << " Bowling Game Started! Enter pins for each roll (0-10):" << RESET << std::endl;
    std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
}

void CBowlingGame::printGameOver(const GameMode &lMode)
{
    std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
    std::cout << BOLDYELLOW << "\t\t\t\t\t\tGame Over !\n";
    std::cout << BOLDGREEN << "\t\t\t\t\t\tFinal Score is :" << getFinalScore() << RESET << std::endl;
    std::cout << BOLDRED << "*********************************************************************************************************************" << RESET << std::endl;
}
