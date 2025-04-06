#ifndef CBowlingGame_HPP
#define CBowlingGame_HPP

#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <iostream>
#include <thread>
#include <chrono>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDYELLOW "\033[1m\033[33m"
#define BOLDBLUE "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN "\033[1m\033[36m"
#define BOLDWHITE "\033[1m\033[37m"

inline constexpr uint16_t MAX_NO_FRAMES = 10;
class CBowlingGame
{
public:
    [[nodiscard]] static std::shared_ptr<CBowlingGame> getInstance();
    void init();
    void start(int &retFlag);
    ~CBowlingGame() = default;

    void clear();
    int getCurrentFrameIndex() const;
    uint32_t &getFinalScore();
    bool getYes(const std::string &lstrPrompt);
    bool &isAnotherRoll() const;
    void printScore(int liFrameNo) const;
    void setisAnotherRollHandled(bool lbRoll) const;
    void roll(int pins);

private:
    void setFinalScore(uint32_t &);
    void setisAnotherRoll(bool) const;
    void showSpinner(const std::string &message, int durationMs, int intervalMs);
    void showDotAnimation(const std::string &message, int durationMs, int intervalMs);
    void printFirstRaw(int &liRollIndex) const;
    void printSecondRaw(int &liRollIndex, int &currentScore) const;
    void printFrameInfo(int liFrameNo, int &liRollIndex) const;
    int strikeBonus(int rollIndex) const;
    int spareBonus(int rollIndex) const;
    int sumOfBallsInFrame(int rollIndex) const;

private:
    bool isStrike(int index) const;
    bool isSpare(int index) const;

private:
    CBowlingGame() = default;
    CBowlingGame(const CBowlingGame &) = delete;
    CBowlingGame &operator=(const CBowlingGame &) = delete;
    static std::shared_ptr<CBowlingGame> m_pCBwlngGame;

private:
    std::vector<uint16_t> m_vRolls;
    mutable bool m_IsAnotherRoll = false;
    mutable bool m_IsAnotherRollHandled = false;
    mutable uint32_t m_FinalScore;
};

#endif // CBowlingGame_HPP