// main.cpp
#include "gitfit.hpp"

void showMenu() {
    cout << COL_MENU;
    cout << "==================== MAIN MENU ====================\n";
    cout << "Type the number of an option and press Enter.\n";
    cout << " 1) Create new profile      (set up your details)\n";
    cout << " 2) Switch active profile   (change user)\n";
    cout << " 3) View active profile     (see your info)\n";
    cout << " 4) Update active profile   (edit age/weight/goal)\n";
    cout << " 5) Calculate BMI           (see your BMI category)\n";
    cout << " 6) Generate workout plan   (weekly exercise guide)\n";
    cout << " 7) Generate diet plan      (daily sample meals)\n";
    cout << " 8) Add daily workout log   (record what you did)\n";
    cout << " 9) View weekly summary     (see progress & badges)\n";
    cout << "10) Delete active profile   (remove profile + logs)\n";
    cout << "11) Achievements            (badges, XP & progress)\n";
    cout << " 0) Exit                    (close GitFit)\n";
    cout << "===================================================\n";
    cout << COL_RESET;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    UserProfile profiles[MAX_PROFILES];
    int profileCount        = loadAllProfiles(profiles, MAX_PROFILES);
    int currentProfileIndex = -1;

    if (profileCount > 0) {
        clearScreen();
        printMainBanner();
        cout << "Found " << profileCount << " saved profile(s).\n\n";
        cout << "You can pick one now, or press 0 to go back and create a new one later.\n\n";
        int idx = chooseProfileIndex(profiles, profileCount);
        if (idx >= 0) {
            currentProfileIndex = idx;
        } else {
            cout << "\nNo profile selected. You can create one from the menu (option 1).\n";
            pauseScreen();
        }
    } else {
        clearScreen();
        printMainBanner();
        cout << COL_WARN << "No profiles found yet.\n\n" << COL_RESET;
        cout << "Use option 1 in the main menu to create your first profile.\n\n";
        pauseScreen();
    }

    int choice;
    do {
        clearScreen();
        printMainBanner();

        if (currentProfileIndex >= 0) {
            cout << COL_OK << "Active profile: " << profiles[currentProfileIndex].name
                 << "  [" << profiles[currentProfileIndex].goal
                 << ", " << profiles[currentProfileIndex].dietPreference << "]\n\n"
                 << COL_RESET;
        } else {
            cout << COL_WARN << "No active profile selected.\n\n" << COL_RESET;
        }

        showMenu();
        choice = getIntInRange("Enter your choice (0-11): ", 0, 11);

        switch (choice) {
            case 1:
                createNewProfile(profiles, profileCount, currentProfileIndex);
                break;
            case 2: {
                int idx = chooseProfileIndex(profiles, profileCount);
                if (idx >= 0) {
                    currentProfileIndex = idx;
                    cout << COL_OK << "\nActive profile changed to: "
                         << profiles[currentProfileIndex].name << '\n' << COL_RESET;
                    pauseScreen();
                } else {
                    cout << "\nNo profile changed.\n";
                    pauseScreen();
                }
                break;
            }
            case 3:
                if (currentProfileIndex >= 0)
                    viewProfile(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 4:
                if (currentProfileIndex >= 0)
                    updateCurrentProfile(profiles, profileCount, currentProfileIndex);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 5:
                if (currentProfileIndex >= 0)
                    showBMI(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 6:
                if (currentProfileIndex >= 0)
                    generateWorkoutPlan(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 7:
                if (currentProfileIndex >= 0)
                    generateDietPlan(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 8:
                if (currentProfileIndex >= 0)
                    addWorkoutLog(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 9:
                if (currentProfileIndex >= 0)
                    showWeeklySummary(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 10:
                if (currentProfileIndex >= 0)
                    deleteCurrentProfile(profiles, profileCount, currentProfileIndex);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 11:
                if (currentProfileIndex >= 0)
                    showAchievementsScreen(profiles[currentProfileIndex]);
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 0:
                clearScreen();
                cout << "Thanks for using GitFit. Stay strong!\n";
                break;
        }

    } while (choice != 0);

    return 0;
}
