
#include "CBowlingGame.hpp"

std::shared_ptr<CBowlingGame> CBowlingGame::m_pCBwlngGame = nullptr;

std::shared_ptr<CBowlingGame> CBowlingGame::getInstance()
{
    if (!m_pCBwlngGame)
    {
        m_pCBwlngGame = std::shared_ptr<CBowlingGame>(new CBowlingGame());
    }
    return m_pCBwlngGame;
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

uint32_t& CBowlingGame::getFinalScore() 
{
    return m_FinalScore;
}

void CBowlingGame::setFinalScore(uint32_t &lScore)
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

void CBowlingGame::printScore() const
{

    int liRollIndex = 0;
    std::cout << "Frame Info:\n";
    printFrameInfo(liRollIndex);

    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n|";
    liRollIndex = 0;
    // Print the first raw :  frame,roll score
    printFirstRaw(liRollIndex);

    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n|";

    liRollIndex = 0;
    int currentScore = 0;
    // Print the second raw :  cumilative score/ total score
    printSecondRaw(liRollIndex, currentScore);

    std::cout << "\n+-----+-----+-----+-----+-----+-----+-----+-----+-----+-------+-------+\n";
}

void CBowlingGame::printFrameInfo(int &liRollIndex) const
{
    for (int iFrameNo = 0; iFrameNo < MAX_NO_FRAMES; ++iFrameNo)
    {
        std::cout << "Frame " << (iFrameNo + 1) << ": ";

        if (iFrameNo < 9)
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
            for (int i = 0; i < 3 && liRollIndex + i < m_vRolls.size(); ++i)
            {
                std::cout << "Roll " << (i + 1) << " = " << m_vRolls[liRollIndex + i] << ", ";
            }
            liRollIndex += 3;
        }

        std::cout << std::endl;
    }
}
int CBowlingGame::getCurrentFrameIndex() const
{
    int frame = 0;
    int rollIndex = 0;
    while (rollIndex < m_vRolls.size() && frame < 10)
    {
        if (isStrike(rollIndex))
            rollIndex += 1;
        else
            rollIndex += 2;
        frame++;
    }
    return frame;
}

void CBowlingGame::clear()
{
    m_vRolls.clear();
    std::system("clear");
    m_IsAnotherRoll = false;
    m_IsAnotherRollHandled = false;
}
