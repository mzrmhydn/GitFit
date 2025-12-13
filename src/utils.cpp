// utils.cpp
#include "gitfit.hpp"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << COL_OK << "\nPress Enter to return to the menu..." << COL_RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printMainBanner() {
    cout << COL_TITLE;
    cout << "=====================================================\n";
    cout << "  ____    ___    _____   ______    ___    _____ \n";
    cout << " / ___|  |_ _|  |_   _|  |  ___|  |_ _|  |_   _|\n";
    cout << "| |  _    | |     | |    | |_      | |     | |  \n";
    cout << "| |_| |   | |     | |    |  _|     | |     | |  \n";
    cout << " \\____|  |___|    |_|    |_|      |___|    |_|  \n";
    cout << "\n";
    cout << "               GitFit - Your Fitness Buddy\n";
    cout << "=====================================================\n\n";
    cout << COL_RESET;
}

int getIntInRange(const string &prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << COL_WARN << "Invalid input. Please enter a number.\n" << COL_RESET;
            continue;
        }
        if (value < minVal || value > maxVal) {
            cout << COL_WARN << "Please enter a value between "
                 << minVal << " and " << maxVal << ".\n" << COL_RESET;
            continue;
        }
        return value;
    }
}

float getFloatInRange(const string &prompt, float minVal, float maxVal) {
    float value;
    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << COL_WARN << "Invalid input. Please enter a number.\n" << COL_RESET;
            continue;
        }
        if (value < minVal || value > maxVal) {
            cout << COL_WARN << "Please enter a value between "
                 << minVal << " and " << maxVal << ".\n" << COL_RESET;
            continue;
        }
        return value;
    }
}

char getCharFromOptions(const string &prompt, const string &options) {
    char c;
    while (true) {
        cout << prompt;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << COL_WARN << "Invalid input. Try again.\n" << COL_RESET;
            continue;
        }
        bool ok = false;
        for (char o : options) {
            if (c == o) {
                ok = true;
                break;
            }
        }
        if (!ok) {
            cout << COL_WARN << "Please enter one of [" << options << "].\n" << COL_RESET;
            continue;
        }
        return c;
    }
}

char getYesNo(const string &prompt) {
    char c;
    while (true) {
        cout << prompt;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << COL_WARN << "Invalid input. Enter y or n.\n" << COL_RESET;
            continue;
        }
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        if (c == 'y' || c == 'n') return c;
        cout << COL_WARN << "Please enter 'y' or 'n'.\n" << COL_RESET;
    }
}

string toLowerString(string s) {
    for (char &c : s) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return s;
}
