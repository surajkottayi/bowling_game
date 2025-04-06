#ifndef CBowlingGame_HPP
#define CBowlingGame_HPP

#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

/// @name Console Color Definitions
/// @{
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
/// @}

/// @brief Maximum number of frames in a bowling game.
inline constexpr uint16_t MAX_NO_FRAMES = 10;

/**
 * @class CBowlingGame
 * @brief Singleton class to manage and calculate a bowling game's score.
 */
namespace bowling_game
{
    enum class GameMode : uint16_t
    {
        GM_UNKNOWN = 0,
        GM_NORMAL = 1,
        GM_TEST = 2
    };
    class CBowlingGame
    {
    public:
        /**
         * @brief Get the singleton instance of CBowlingGame.
         * @return Shared pointer to the CBowlingGame instance.
         */
        [[nodiscard]] static std::shared_ptr<CBowlingGame> getInstance();

        int getFinalScore() const;

        void printGameBegin(const GameMode &lMode);

        void printGameOver(const GameMode &lMode);

        /**
         * @brief Initializes the game.
         */
        void init();

        /**
         * @brief Clears the game's data and resets the state.
         */
        void clear();

        /**
         * @brief Gets the current frame index (0-based).
         * @return Current frame index.
         */
        int getCurrentFrameIndex() const;

        /**
         * @brief Prompts user for a yes/no input.
         * @param lstrPrompt The message to prompt.
         * @return True if user input is 'yes', false otherwise.
         */
        bool getYes(const std::string &lstrPrompt);

        /**
         * @brief Checks if another roll is expected.
         * @return Reference to the mutable boolean flag.
         */
        bool &isAnotherRoll() const;

        /**
         * @brief Prints the score for a specific frame.
         * @param liFrameNo Frame number.
         */
        void printScore(int liFrameNo) const;

        /**
         * @brief Sets the handled state for another roll prompt.
         * @param lbRoll Boolean flag to mark as handled.
         */
        void setisAnotherRollHandled(bool lbRoll) const;

        /**
         * @brief Records a roll in the current game.
         * @param pins Number of pins knocked down.
         */
        void roll(int pins);

    private:
        /**
         * @brief Private constructor for singleton pattern.
         */
        CBowlingGame() = default;

        // Deleted copy and assignment operators
        CBowlingGame(const CBowlingGame &) = delete;
        CBowlingGame &operator=(const CBowlingGame &) = delete;

        /**
         * @brief Sets the final score.
         * @param score Reference to score to set.
         */
        void setFinalScore(uint32_t &&score);

        /**
         * @brief Sets the flag indicating if another roll is needed.
         * @param value True if another roll is needed.
         */
        void setisAnotherRoll(bool value) const;

        /**
         * @brief Shows a spinner animation on the console.
         * @param message Message to display.
         * @param durationMs Duration in milliseconds.
         * @param intervalMs Interval between spinner updates.
         */
        void showSpinner(const std::string &message, int durationMs, int intervalMs);

        /**
         * @brief Shows a dot animation on the console.
         * @param message Message to display.
         * @param durationMs Duration in milliseconds.
         * @param intervalMs Interval between dot prints.
         */
        void showDotAnimation(const std::string &message, int durationMs, int intervalMs);

        /**
         * @brief Prints the first row of the score display.
         * @param liRollIndex Index of the roll.
         */
        void printFirstRaw(int &liRollIndex) const;

        /**
         * @brief Prints the second row of the score display including current score.
         * @param liRollIndex Index of the roll.
         * @param currentScore Current frame's cumulative score.
         */
        void printSecondRaw(int &liRollIndex, int &currentScore) const;

        /**
         * @brief Prints detailed info about a specific frame.
         * @param liFrameNo Frame number.
         * @param liRollIndex Roll index to start from.
         */
        void printFrameInfo(int liFrameNo, int &liRollIndex) const;

        /**
         * @brief Calculates bonus for a strike.
         * @param rollIndex Index of the strike roll.
         * @return Bonus score for strike.
         */
        int strikeBonus(int rollIndex) const;

        /**
         * @brief Calculates bonus for a spare.
         * @param rollIndex Index of the spare roll.
         * @return Bonus score for spare.
         */
        int spareBonus(int rollIndex) const;

        /**
         * @brief Returns the sum of balls in a frame.
         * @param rollIndex Index of the first roll in frame.
         * @return Sum of the two rolls in a frame.
         */
        int sumOfBallsInFrame(int rollIndex) const;

        /**
         * @brief Checks if the roll at index is a strike.
         * @param index Index of roll.
         * @return True if strike.
         */
        bool isStrike(int index) const;

        /**
         * @brief Checks if the rolls at index represent a spare.
         * @param index Index of first roll.
         * @return True if spare.
         */
        bool isSpare(int index) const;

    private:
        /// Singleton instance of CBowlingGame.
        static std::shared_ptr<CBowlingGame> m_pCBwlngGame;

        /// Vector to store the pins knocked down in each roll.
        std::vector<uint16_t> m_vRolls;

        /// Flag to indicate another roll is needed.
        mutable bool m_IsAnotherRoll;

        /// Flag to indicate whether the "another roll" state has been handled.
        mutable bool m_IsAnotherRollHandled;

        /// Final score of the game.
        mutable uint32_t m_FinalScore;
    };
}
#endif // CBowlingGame_HPP
