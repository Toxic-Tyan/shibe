#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

class Hangman {
private:
    std::vector<std::string> wordList;   // Список слів (розшифровується)
    std::string secretWord;              // Загадане слово
    std::string displayWord;             // Поточний стан слова з "_"
    std::vector<char> guessedLetters;    // Вгадані літери
    int maxErrors;                        // Максимальна кількість помилок
    int currentErrors;                    // Лічильник помилок
    time_t startTime;                    // Час початку гри

    // Лiмiт помилок
public:
    Hangman(int maxErrors = 6) {
        this->maxErrors = maxErrors;
        this->currentErrors = 0;
    }

    // Завантаження англійських слів з файлу та розшифровка (+1)
    void LoadWords(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                for (int i = 0; i < line.size(); i++) line[i] -= 1; // Розшифровка
                wordList.push_back(line);
            }
        }
    }

    // Вибір випадкового слова
    void ChooseWord() {
        srand(static_cast<unsigned int>(time(0)));
        secretWord = wordList[rand() % wordList.size()];
        displayWord = std::string(secretWord.size(), '_');
    }

    // Перевірка, чи літера вже була вгадана
    bool AlreadyGuessed(char letter) const {
        return std::find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end();
    }

    // Відображення поточного стану гри
    void ShowState() const {
        std::cout << "Word: " << displayWord << "\n";
        std::cout << "Guessed letters: ";
        for (char c : guessedLetters) std::cout << c << " ";
        std::cout << "\n";
        std::cout << "Errors left: " << (maxErrors - currentErrors) << "\n";
    }

    // Обробка введеної літери
    void Guess(char letter) {
        guessedLetters.push_back(letter);

        bool correct = false;
        for (int i = 0; i < secretWord.size(); i++) {
            if (secretWord[i] == letter) {
                displayWord[i] = letter;
                correct = true;
            }
        }

        if (!correct) {
            currentErrors++;
            std::cout << "Incorrect!\n";
        }
    }

    // Основний цикл гри
    void Play() {
        startTime = time(0);
        while (displayWord != secretWord && currentErrors < maxErrors) {
            ShowState();
            std::cout << "Enter a letter: ";
            char guess;
            std::cin >> guess;

            // Приведення букви до нижнього регістру
            if (guess >= 'A' && guess <= 'Z') guess += 32;

            if (AlreadyGuessed(guess)) {
                std::cout << "Letter already guessed!\n";
                continue;
            }

            Guess(guess);
        }
        EndGame();
    }

    // Завершення гри та вивід статистики
    void EndGame() const {
        time_t endTime = time(0);
        if (displayWord == secretWord) std::cout << "You won! Word: " << secretWord << "\n";
        else std::cout << "You lost! Word was: " << secretWord << "\n";

        std::cout << "Game statistics:\n";
        std::cout << "Time played: " << difftime(endTime, startTime) << " seconds\n";
        std::cout << "Number of errors: " << currentErrors << "\n";
        std::cout << "Guessed letters: ";
        for (char c : guessedLetters) std::cout << c << " ";
        std::cout << "\n";
    }
};

int main() {
    Hangman game;                // Створення об'єкта гри
    game.LoadWords("words.txt"); // Завантаження англійських зашифрованих слів
    game.ChooseWord();           // Вибір випадкового слова
    game.Play();                 // Початок гри
    return 0;
}
