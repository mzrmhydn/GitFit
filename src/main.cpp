#include <iostream>
#include <fstream>
#include <sstream>
#include <string>   
#include <cstdlib>   // system("cls")
#include <ctime>
#include <limits>    // numeric_limits
#include <algorithm> // std::max
#include <cctype>    // tolower

using namespace std;

// ===================== DATA STRUCTURES =====================

struct UserProfile {
    string name;
    int age;
    char gender;           // 'M' / 'F'
    float heightCm;
    float weightKg;
    string goal;           // lose / maintain / gain / strength / stamina
    string dietPreference; // veg / non-veg
};

struct WorkoutLog {
    int weekNumber;
    string date;
    string exercise;
    int durationMinutes;
    int caloriesBurned;
};

struct WeeklySummary {
    int totalMinutes;
    int totalCalories;
    int daysLogged;
};

// ===================== CONSTANTS =====================

const string PROFILES_FILE    = "profiles.txt";      // multiple profiles
const string WORKOUT_LOG_FILE = "workout_log.txt";
const int    MAX_PROFILES     = 100;

// Simple ANSI color codes (VS Code terminal / Git Bash etc.)
const string COL_RESET  = "\033[0m";
const string COL_TITLE  = "\033[1;36m";  // bright cyan
const string COL_MENU   = "\033[1;33m";  // yellow
const string COL_OK     = "\033[1;32m";  // green
const string COL_WARN   = "\033[1;31m";  // red
const string COL_MUTED  = "\033[2;37m";  // grey

// ===================== UTILS / UI =====================

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

void playBeep() {
    cout << '\a'; // simple console beep
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

// ----------- INPUT VALIDATION HELPERS -----------

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

// Make string lowercase (for classifying exercise names)
string toLowerString(string s) {
    for (char &c : s) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

// ===================== PROFILE HANDLING =====================

// Each profile is stored as:
// name|age|gender|heightCm|weightKg|goal|dietPreference
int loadAllProfiles(UserProfile profiles[], int maxProfiles) {
    ifstream in(PROFILES_FILE.c_str());
    if (!in) return 0;

    string line;
    int count = 0;

    while (getline(in, line) && count < maxProfiles) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        UserProfile u;

        getline(ss, u.name, '|');

        getline(ss, token, '|');
        u.age = atoi(token.c_str());

        getline(ss, token, '|');
        u.gender = token.empty() ? 'M' : token[0];

        getline(ss, token, '|');
        u.heightCm = static_cast<float>(atof(token.c_str()));

        getline(ss, token, '|');
        u.weightKg = static_cast<float>(atof(token.c_str()));

        getline(ss, u.goal, '|');
        getline(ss, u.dietPreference, '|');

        profiles[count++] = u;
    }

    in.close();
    return count;
}

void saveAllProfiles(UserProfile profiles[], int count) {
    ofstream out(PROFILES_FILE.c_str());
    if (!out) {
        cout << COL_WARN << "Error: Could not save profiles file.\n" << COL_RESET;
        return;
    }

    for (int i = 0; i < count; i++) {
        out << profiles[i].name << '|'
            << profiles[i].age << '|'
            << profiles[i].gender << '|'
            << profiles[i].heightCm << '|'
            << profiles[i].weightKg << '|'
            << profiles[i].goal << '|'
            << profiles[i].dietPreference << '\n';
    }

    out.close();
}

void listProfiles(UserProfile profiles[], int count) {
    cout << "Existing Profiles:\n";
    cout << "-----------------------------------------------------\n";
    for (int i = 0; i < count; i++) {
        cout << " " << (i + 1) << ") " << profiles[i].name
             << "  [" << profiles[i].goal
             << ", " << profiles[i].dietPreference << "]\n";
    }
    cout << "-----------------------------------------------------\n";
}

int chooseProfileIndex(UserProfile profiles[], int count) {
    if (count == 0) {
        cout << COL_WARN << "No profiles yet.\n" << COL_RESET;
        return -1;
    }

    listProfiles(profiles, count);
    int choice = getIntInRange("\nEnter profile number to select (0 to cancel): ", 0, count);
    if (choice == 0) return -1;
    return choice - 1;
}

// --------- CREATE PROFILE ---------

void createNewProfile(UserProfile profiles[], int &count, int &currentIndex) {
    if (count >= MAX_PROFILES) {
        cout << COL_WARN << "Maximum number of profiles reached.\n" << COL_RESET;
        pauseScreen();
        return;
    }

    clearScreen();
    printMainBanner();
    cout << COL_TITLE << ">>> Create New Profile\n\n" << COL_RESET;
    cout << "We will ask you a few simple questions to set up your profile.\n";
    cout << "You can always update these later from the menu.\n\n";

    UserProfile u;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter your name: ";
    getline(cin, u.name);

    u.age      = getIntInRange("Enter your age (10-100): ", 10, 100);
    u.gender   = getCharFromOptions("Enter gender (M/F): ", "MmFf");
    u.heightCm = getFloatInRange("Enter height (in cm, 100-250): ", 100.0f, 250.0f);
    u.weightKg = getFloatInRange("Enter weight (in kg, 30-250): ", 30.0f, 250.0f);

    cout << "\nChoose your main goal:\n";
    cout << " 1) lose      - Fat loss\n";
    cout << " 2) maintain  - General fitness\n";
    cout << " 3) gain      - Weight / muscle gain\n";
    cout << " 4) strength  - Get stronger\n";
    cout << " 5) stamina   - Improve endurance\n";
    int g = getIntInRange("Enter choice (1-5): ", 1, 5);
    if      (g == 1) u.goal = "lose";
    else if (g == 2) u.goal = "maintain";
    else if (g == 3) u.goal = "gain";
    else if (g == 4) u.goal = "strength";
    else             u.goal = "stamina";

    cout << "\nDiet preference:\n";
    cout << " 1) veg\n";
    cout << " 2) non-veg\n";
    int d = getIntInRange("Enter choice (1-2): ", 1, 2);
    u.dietPreference = (d == 1 ? "veg" : "non-veg");

    playBeep();
    cout << COL_OK << "\nProfile info captured!\n" << COL_RESET;

    profiles[count] = u;
    count++;
    currentIndex = count - 1;

    saveAllProfiles(profiles, count);
    cout << COL_OK << "New profile saved and set as active.\n" << COL_RESET;
    pauseScreen();
}

// --------- VIEW PROFILE ---------

void viewProfile(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Active Profile Overview\n\n" << COL_RESET;
    cout << " Name           : " << user.name << '\n';
    cout << " Age            : " << user.age << '\n';
    cout << " Gender         : " << user.gender << '\n';
    cout << " Height         : " << user.heightCm << " cm\n";
    cout << " Weight         : " << user.weightKg << " kg\n";
    cout << " Goal           : " << user.goal << '\n';
    cout << " Diet Preference: " << user.dietPreference << '\n';
    cout << "\n ---------------------------------------------------\n";
    pauseScreen();
}

// --------- UPDATE PROFILE (field-wise with back) ---------

void updateCurrentProfile(UserProfile profiles[], int count, int currentIndex) {
    if (currentIndex < 0 || currentIndex >= count) {
        cout << COL_WARN << "No active profile to update.\n" << COL_RESET;
        pauseScreen();
        return;
    }

    bool done = false;
    while (!done) {
        clearScreen();
        printMainBanner();
        cout << COL_TITLE << ">>> Update Profile: "
             << profiles[currentIndex].name << "\n\n" << COL_RESET;
        cout << "Select the field you want to change. You don't need to re-enter\n";
        cout << "everything, only the parts you want to update.\n\n";

        cout << "Current values:\n";
        cout << " 1) Name           : " << profiles[currentIndex].name << '\n';
        cout << " 2) Age            : " << profiles[currentIndex].age << '\n';
        cout << " 3) Gender         : " << profiles[currentIndex].gender << '\n';
        cout << " 4) Height (cm)    : " << profiles[currentIndex].heightCm << '\n';
        cout << " 5) Weight (kg)    : " << profiles[currentIndex].weightKg << '\n';
        cout << " 6) Goal           : " << profiles[currentIndex].goal << '\n';
        cout << " 7) Diet Preference: " << profiles[currentIndex].dietPreference << '\n';
        cout << " 0) Back to previous menu\n";
        cout << "-----------------------------------------------------\n";

        int choice = getIntInRange("Select field to update (0-7): ", 0, 7);
        if (choice == 0) {
            done = true;
            break;
        }

        switch (choice) {
            case 1: {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter new name: ";
                getline(cin, profiles[currentIndex].name);
                break;
            }
            case 2: {
                profiles[currentIndex].age =
                    getIntInRange("Enter new age (10-100): ", 10, 100);
                break;
            }
            case 3: {
                profiles[currentIndex].gender =
                    getCharFromOptions("Enter new gender (M/F): ", "MmFf");
                break;
            }
            case 4: {
                profiles[currentIndex].heightCm =
                    getFloatInRange("Enter new height (100-250 cm): ", 100.0f, 250.0f);
                break;
            }
            case 5: {
                profiles[currentIndex].weightKg =
                    getFloatInRange("Enter new weight (30-250 kg): ", 30.0f, 250.0f);
                break;
            }
            case 6: {
                cout << "\nChoose new goal:\n";
                cout << " 1) lose\n";
                cout << " 2) maintain\n";
                cout << " 3) gain\n";
                cout << " 4) strength\n";
                cout << " 5) stamina\n";
                int g = getIntInRange("Enter choice (1-5): ", 1, 5);
                if      (g == 1) profiles[currentIndex].goal = "lose";
                else if (g == 2) profiles[currentIndex].goal = "maintain";
                else if (g == 3) profiles[currentIndex].goal = "gain";
                else if (g == 4) profiles[currentIndex].goal = "strength";
                else             profiles[currentIndex].goal = "stamina";
                break;
            }
            case 7: {
                cout << "\nDiet preference:\n";
                cout << " 1) veg\n";
                cout << " 2) non-veg\n";
                int d = getIntInRange("Enter choice (1-2): ", 1, 2);
                profiles[currentIndex].dietPreference = (d == 1 ? "veg" : "non-veg");
                break;
            }
        }

        saveAllProfiles(profiles, count);
        playBeep();
        cout << COL_OK << "\nField updated successfully.\n" << COL_RESET;
        pauseScreen();
    }
}

// ===================== BMI =====================

float calculateBMI(const UserProfile &user) {
    float h = user.heightCm / 100.0f;
    if (h <= 0) return 0.0f;
    return user.weightKg / (h * h);
}

string interpretBMI(float bmi) {
    if (bmi < 18.5f) return "Underweight";
    else if (bmi < 25.0f) return "Normal weight";
    else if (bmi < 30.0f) return "Overweight";
    else return "Obese";
}

void showBMI(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> BMI & Category\n\n" << COL_RESET;
    cout << "We use your height and weight to estimate Body Mass Index (BMI).\n";
    cout << "This is a rough indicator of your weight category.\n\n";

    float bmi = calculateBMI(user);
    cout << " BMI value : " << bmi << '\n';
    cout << " Category  : " << interpretBMI(bmi) << '\n';

    if (bmi < 18.5f)
        cout << " Tip: You may need a calorie surplus and strength training.\n";
    else if (bmi < 25.0f)
        cout << " Tip: Great! Maintain with balanced diet & regular exercise.\n";
    else if (bmi < 30.0f)
        cout << " Tip: Aim for a slow, steady calorie deficit and daily walks.\n";
    else
        cout << " Tip: Start gently with low-impact exercise and talk to a doctor.\n";

    cout << "\n ---------------------------------------------------\n";
    playBeep();
    pauseScreen();
}

// ===================== WORKOUT PLAN =====================

void generateWorkoutPlan(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Personalized Workout Planner\n\n" << COL_RESET;
    cout << "We will use your goal and available time to suggest a weekly plan.\n";
    cout << "This is a simple guide, not medical advice.\n\n";

    string goal = user.goal;
    char change = getYesNo("Use profile goal '" + user.goal + "'? (y/n): ");

    if (change == 'n') {
        cout << "\nChoose goal:\n";
        cout << " 1) lose\n";
        cout << " 2) maintain\n";
        cout << " 3) gain\n";
        cout << " 4) strength\n";
        cout << " 5) stamina\n";
        int g = getIntInRange("Enter choice (1-5): ", 1, 5);
        if      (g == 1) goal = "lose";
        else if (g == 2) goal = "maintain";
        else if (g == 3) goal = "gain";
        else if (g == 4) goal = "strength";
        else             goal = "stamina";
    }

    int dailyMinutes = getIntInRange(
        "\nHow many minutes can you give per workout day (10-180)? ", 10, 180);
    int daysPerWeek = getIntInRange(
        "How many days can you work out per week (1-7)? ", 1, 7);

    cout << "\nChoose difficulty level:\n";
    cout << " 1) Beginner\n";
    cout << " 2) Intermediate\n";
    cout << " 3) Advanced\n";
    int difficulty = getIntInRange("Enter choice (1-3): ", 1, 3);

    clearScreen();
    printMainBanner();
    cout << COL_TITLE << ">>> Weekly Workout Plan (" << goal << ")\n\n" << COL_RESET;

    cout << "Time commitment: " << dailyMinutes << " min/day, "
         << daysPerWeek << " days/week\n\n";

    string days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                      "Friday", "Saturday", "Sunday"};

    for (int d = 0; d < daysPerWeek && d < 7; d++) {
        cout << COL_MENU << "---------------- " << days[d]
             << " ----------------\n" << COL_RESET;

        if (goal == "gain" || goal == "strength") {
            int splitDay = d % 5; // chest, back, legs, shoulders, arms

            if (splitDay == 0) { // Chest
                cout << " Focus: Chest & Triceps\n";
                if (difficulty == 1) {
                    cout << "  * Push-ups 3 x 8\n";
                    cout << "  * Incline push-ups 3 x 10\n";
                } else if (difficulty == 2) {
                    cout << "  * Bench press 4 x 8\n";
                    cout << "  * Incline dumbbell press 3 x 10\n";
                    cout << "  * Tricep dips 3 x 10\n";
                } else {
                    cout << "  * Heavy bench press 5 x 5\n";
                    cout << "  * Weighted dips 4 x 8\n";
                    cout << "  * Close-grip bench press 4 x 8\n";
                }
            } else if (splitDay == 1) { // Back
                cout << " Focus: Back & Biceps\n";
                if (difficulty == 1) {
                    cout << "  * Dumbbell rows 3 x 10\n";
                    cout << "  * Bicep curls 3 x 10\n";
                } else if (difficulty == 2) {
                    cout << "  * Pull-ups or lat pulldown 4 x 8\n";
                    cout << "  * Barbell rows 4 x 8\n";
                    cout << "  * Barbell curls 3 x 10\n";
                } else {
                    cout << "  * Weighted pull-ups 5 x 5\n";
                    cout << "  * T-bar rows 4 x 8\n";
                    cout << "  * Heavy curls 4 x 8\n";
                }
            } else if (splitDay == 2) { // Legs
                cout << " Focus: Legs\n";
                if (difficulty == 1) {
                    cout << "  * Bodyweight squats 3 x 12\n";
                    cout << "  * Lunges 3 x 10\n";
                } else if (difficulty == 2) {
                    cout << "  * Barbell squats 4 x 8\n";
                    cout << "  * Romanian deadlift 4 x 8\n";
                    cout << "  * Calf raises 3 x 15\n";
                } else {
                    cout << "  * Heavy squats 5 x 5\n";
                    cout << "  * Deadlift 5 x 5\n";
                    cout << "  * Walking lunges 4 x 12 steps\n";
                }
            } else if (splitDay == 3) { // Shoulders
                cout << " Focus: Shoulders\n";
                if (difficulty == 1) {
                    cout << "  * Dumbbell shoulder press 3 x 10\n";
                    cout << "  * Side raises 3 x 12\n";
                } else if (difficulty == 2) {
                    cout << "  * Barbell overhead press 4 x 8\n";
                    cout << "  * Lateral raises 3 x 12\n";
                    cout << "  * Rear delt flyes 3 x 12\n";
                } else {
                    cout << "  * Heavy overhead press 5 x 5\n";
                    cout << "  * Upright rows 4 x 8\n";
                    cout << "  * Superset lateral + front raises 3 x 12\n";
                }
            } else { // Arms / accessory
                cout << " Focus: Arms & Accessory\n";
                if (difficulty == 1) {
                    cout << "  * Bicep curls 3 x 12\n";
                    cout << "  * Tricep extensions 3 x 12\n";
                } else if (difficulty == 2) {
                    cout << "  * Close-grip bench press 4 x 8\n";
                    cout << "  * EZ-bar curls 4 x 10\n";
                    cout << "  * Forearm curls 3 x 12\n";
                } else {
                    cout << "  * Superset curls & skull crushers 4 x 10\n";
                    cout << "  * Heavy dips 4 x 8\n";
                    cout << "  * Grip work / farmer walks\n";
                }
            }

        } else if (goal == "lose") {
            int dayType = d % 3;
            if (dayType == 0) {
                cout << " Focus: Steady-state cardio\n";
                cout << "  * Brisk walk or light jog " << dailyMinutes - 5 << " min\n";
                cout << "  * 5 min stretching\n";
            } else if (dayType == 1) {
                cout << " Focus: Full-body circuit\n";
                cout << "  * 3 rounds of:\n";
                cout << "      - Squats 15 reps\n";
                cout << "      - Push-ups 10 reps\n";
                cout << "      - Mountain climbers 20 reps\n";
                cout << "  * 5–10 min cool-down walk\n";
            } else {
                cout << " Focus: Intervals\n";
                cout << "  * 1 min fast / 1 min slow (walk or jog) for "
                     << dailyMinutes - 5 << " min\n";
                cout << "  * 5 min stretching\n";
            }
        } else { // maintain / stamina
            int dayType = d % 2;
            if (dayType == 0) {
                cout << " Focus: Cardio + light strength\n";
                cout << "  * " << dailyMinutes - 10 << " min cycling / brisk walk\n";
                cout << "  * 2 sets of push-ups + squats + plank\n";
            } else {
                cout << " Focus: Mobility & core\n";
                cout << "  * 10–15 min dynamic stretching\n";
                cout << "  * 3 sets of core exercises (plank, leg raises, dead bug)\n";
                cout << "  * Easy walk to finish\n";
            }
        }

        cout << "----------------------------------------------\n\n";
    }

    cout << "Tip: Always warm up 5–10 min and cool down 5 min.\n\n";
    playBeep();
    pauseScreen();
}

// ===================== DIET PLAN =====================

void printMeal(const string &title,
               const string &foods,
               const string &nutrients) {
    cout << COL_MENU << " " << title << ":" << COL_RESET << "\n";
    cout << "   Foods    : " << foods << '\n';
    cout << "   Nutrients: " << nutrients << "\n\n";
}

void generateDietPlan(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Diet Planner (" << user.dietPreference
         << ", goal: " << user.goal << ")\n\n" << COL_RESET;
    cout << "We will show a sample full day of eating based on your goal.\n";
    cout << "You can adjust portion sizes according to your hunger.\n\n";

    string dietType = user.dietPreference;
    char ch = getYesNo("Use profile diet preference '" + dietType + "'? (y/n): ");
    if (ch == 'n') {
        cout << "Choose preference: 1) veg  2) non-veg\n";
        int x = getIntInRange("Enter choice (1-2): ", 1, 2);
        dietType = (x == 1 ? "veg" : "non-veg");
    }

    clearScreen();
    printMainBanner();
    cout << COL_TITLE << ">>> Sample Day Diet Plan (" << dietType << ")\n\n" << COL_RESET;

    cout << "Choose plan variant:\n";
    cout << " 1) Balanced\n";
    cout << " 2) Higher protein\n";
    int variant = getIntInRange("Enter choice (1-2): ", 1, 2);

    cout << "\n==============================================\n";

    if (dietType == "veg") {
        if (user.goal == "lose") {
            if (variant == 1) {
                printMeal("Breakfast",
                          "Oats with skim milk, apple, handful of nuts",
                          "Protein ~15g, Carbs ~45g, Fats ~10g, Fiber ~8g");
                printMeal("Lunch",
                          "Mixed vegetable curry, 1-2 chapati (brown), salad",
                          "Protein ~12g, Carbs ~50g, Fats ~8g, Fiber ~10g");
                printMeal("Snack",
                          "Low-fat yogurt, cucumber sticks",
                          "Protein ~8g, Carbs ~15g, Fats ~2g");
                printMeal("Dinner",
                          "Lentil soup (daal), stir-fried veggies",
                          "Protein ~18g, Carbs ~35g, Fats ~5g, Fiber ~8g");
            } else {
                printMeal("Breakfast",
                          "Besan chilla with veggies, green tea",
                          "Protein ~18g, Carbs ~30g, Fats ~8g");
                printMeal("Lunch",
                          "Paneer tikka with salad, small portion brown rice",
                          "Protein ~22g, Carbs ~35g, Fats ~12g");
                printMeal("Snack",
                          "Roasted chickpeas / chana",
                          "Protein ~10g, Carbs ~20g, Fiber ~6g");
                printMeal("Dinner",
                          "Mixed dal, sautéed spinach",
                          "Protein ~20g, Carbs ~30g, Fats ~6g");
            }
        } else if (user.goal == "gain" || user.goal == "strength") {
            if (variant == 1) {
                printMeal("Breakfast",
                          "Paratha with curd, banana, nuts",
                          "Protein ~18g, Carbs ~60g, Fats ~18g");
                printMeal("Lunch",
                          "Rajma / chole with rice, salad",
                          "Protein ~20g, Carbs ~65g, Fats ~10g");
                printMeal("Snack",
                          "Peanut butter sandwich, milk",
                          "Protein ~18g, Carbs ~35g, Fats ~15g");
                printMeal("Dinner",
                          "Paneer bhurji with chapati, vegetables",
                          "Protein ~25g, Carbs ~45g, Fats ~15g");
            } else {
                printMeal("Breakfast",
                          "Protein oats (milk + whey), almonds",
                          "Protein ~25g, Carbs ~40g, Fats ~10g");
                printMeal("Lunch",
                          "Soy chunks curry, brown rice, salad",
                          "Protein ~30g, Carbs ~55g, Fats ~8g");
                printMeal("Snack",
                          "Greek yogurt with fruits",
                          "Protein ~15g, Carbs ~25g, Fats ~5g");
                printMeal("Dinner",
                          "Dal + paneer combo, veggies",
                          "Protein ~30g, Carbs ~40g, Fats ~10g");
            }
        } else { // maintain / stamina
            printMeal("Breakfast",
                      "Idli / dosa with sambar, fruit",
                      "Protein ~15g, Carbs ~45g, Fats ~8g");
            printMeal("Lunch",
                      "Mixed veg pulao, raita, salad",
                      "Protein ~15g, Carbs ~60g, Fats ~10g");
            printMeal("Snack",
                      "Fruit + handful of nuts",
                      "Protein ~6g, Carbs ~25g, Fats ~10g");
            printMeal("Dinner",
                      "Vegetable khichdi, salad",
                      "Protein ~14g, Carbs ~45g, Fats ~6g");
        }
    } else { // non-veg
        if (user.goal == "lose") {
            if (variant == 1) {
                printMeal("Breakfast",
                          "Boiled eggs (2 whites + 1 whole), toast, green tea",
                          "Protein ~20g, Carbs ~20g, Fats ~8g");
                printMeal("Lunch",
                          "Grilled chicken breast, salad, small brown rice",
                          "Protein ~30g, Carbs ~35g, Fats ~8g");
                printMeal("Snack",
                          "Apple + handful of almonds",
                          "Protein ~6g, Carbs ~20g, Fats ~10g");
                printMeal("Dinner",
                          "Fish curry with vegetables (low oil)",
                          "Protein ~25g, Carbs ~20g, Fats ~8g");
            } else {
                printMeal("Breakfast",
                          "Omelette with veggies (2 eggs), oats",
                          "Protein ~22g, Carbs ~30g, Fats ~10g");
                printMeal("Lunch",
                          "Stir-fried chicken with veggies, small quinoa portion",
                          "Protein ~32g, Carbs ~30g, Fats ~9g");
                printMeal("Snack",
                          "Low-fat yogurt, berries",
                          "Protein ~10g, Carbs ~15g, Fats ~3g");
                printMeal("Dinner",
                          "Tandoori chicken, salad",
                          "Protein ~28g, Carbs ~10g, Fats ~8g");
            }
        } else if (user.goal == "gain" || user.goal == "strength") {
            if (variant == 1) {
                printMeal("Breakfast",
                          "3-egg omelette, toast, milk",
                          "Protein ~28g, Carbs ~35g, Fats ~18g");
                printMeal("Lunch",
                          "Chicken curry, rice, salad",
                          "Protein ~30g, Carbs ~65g, Fats ~15g");
                printMeal("Snack",
                          "Peanut butter sandwich, banana",
                          "Protein ~16g, Carbs ~45g, Fats ~15g");
                printMeal("Dinner",
                          "Beef / mutton with chapati, veggies",
                          "Protein ~30g, Carbs ~40g, Fats ~18g");
            } else {
                printMeal("Breakfast",
                          "Protein shake, oats with honey",
                          "Protein ~30g, Carbs ~50g, Fats ~10g");
                printMeal("Lunch",
                          "Grilled fish, pasta / rice, vegetables",
                          "Protein ~32g, Carbs ~60g, Fats ~12g");
                printMeal("Snack",
                          "Egg sandwich + milk",
                          "Protein ~20g, Carbs ~35g, Fats ~12g");
                printMeal("Dinner",
                          "Chicken breast, mashed potatoes, veggies",
                          "Protein ~32g, Carbs ~45g, Fats ~10g");
            }
        } else { // maintain / stamina
            printMeal("Breakfast",
                      "2 eggs, toast, fruit",
                      "Protein ~20g, Carbs ~35g, Fats ~10g");
            printMeal("Lunch",
                      "Chicken with mixed veggies, small rice portion",
                      "Protein ~28g, Carbs ~45g, Fats ~10g");
            printMeal("Snack",
                      "Yogurt + nuts",
                      "Protein ~10g, Carbs ~15g, Fats ~10g");
            printMeal("Dinner",
                      "Fish or chicken, salad",
                      "Protein ~25g, Carbs ~20g, Fats ~8g");
        }
    }

    cout << "Note: Values are rough estimates; adjust portions to your needs.\n";
    cout << "Drink plenty of water throughout the day.\n";
    cout << "==============================================\n\n";
    playBeep();
    pauseScreen();
}

// ===================== WORKOUT LOG & SUMMARY =====================

// Cardio: calories depend on exercise name + intensity
int estimateCardioCalories(const string &exercise, int durationMinutes, int intensityLevel) {
    string e = toLowerString(exercise);

    int basePerMin = 6; // default moderate

    if (e == "walk" || e == "walking")
        basePerMin = 4;
    else if (e == "run" || e == "running" || e == "jog" || e == "jogging")
        basePerMin = 10;
    else if (e == "cycle" || e == "cycling" || e == "bike" || e == "biking")
        basePerMin = 8;
    else if (e == "swim" || e == "swimming")
        basePerMin = 9;
    else if (e == "hiit")
        basePerMin = 11;

    // adjust by intensity
    if (intensityLevel == 1)      basePerMin -= 2;  // easy
    else if (intensityLevel == 3) basePerMin += 2;  // hard

    if (basePerMin < 3) basePerMin = 3;
    return durationMinutes * basePerMin;
}

// Strength: calories depend on sets * reps + intensity
int estimateStrengthCalories(const string &exercise, int sets, int repsPerSet, int intensityLevel) {
    int totalReps = sets * repsPerSet;

    double basePerRep = 0.5; // default
    string e = toLowerString(exercise);

    // heavy compound lifts burn more per rep
    if (e.find("squat") != string::npos || e.find("deadlift") != string::npos)
        basePerRep = 0.8;
    else if (e.find("bench") != string::npos || e.find("push") != string::npos)
        basePerRep = 0.6;
    else if (e.find("row") != string::npos || e.find("pull") != string::npos)
        basePerRep = 0.7;

    // adjust for intensity
    if (intensityLevel == 1)      basePerRep *= 0.8;
    else if (intensityLevel == 3) basePerRep *= 1.2;

    int calories = static_cast<int>(totalReps * basePerRep);
    if (calories < 5) calories = 5;
    return calories;
}

void addWorkoutLog() {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Add Daily Workout Log\n\n" << COL_RESET;
    cout << "This section lets you record one workout for a specific day.\n";
    cout << "You can log either cardio (with duration) or strength (with sets & reps).\n\n";

    WorkoutLog log;
    log.weekNumber = getIntInRange(
        "Enter week number for this workout (1-52): ", 1, 52);

    cout << "\nEnter date for this workout (YYYY-MM-DD, e.g. 2025-12-13): ";
    cin >> log.date;

    cout << "\nWhat type of workout did you do today?\n";
    cout << " 1) Cardio (walking, running, cycling, treadmill, etc.)\n";
    cout << " 2) Strength / weights (push-ups, squats, bench press, etc.)\n";
    cout << " 3) Other / mixed (we'll treat it like light cardio)\n";
    int workoutType = getIntInRange("Choose workout type (1-3): ", 1, 3);

    // Consume leftover newline so getline works correctly
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter a short name for the exercise:\n";
    cout << " > ";
    getline(cin, log.exercise);

    cout << "\nRate how hard this session felt:\n";
    cout << " 1) Easy    (you could talk comfortably)\n";
    cout << " 2) Moderate(you were working but okay)\n";
    cout << " 3) Hard    (really challenging)\n";
    int intensity = getIntInRange("Intensity (1-3): ", 1, 3);

    if (workoutType == 1 || workoutType == 3) {
        // CARDIO / OTHER: ask for duration only
        cout << "\nEnter duration of the cardio part in minutes (5-300): ";
        log.durationMinutes = getIntInRange("", 5, 300);

        log.caloriesBurned = estimateCardioCalories(
            log.exercise,
            log.durationMinutes,
            intensity
        );

        cout << COL_MUTED
             << "\n(We estimated calories based on exercise type + intensity.)\n"
             << COL_RESET;

    } else {
        // STRENGTH: ask for sets and reps, derive approx minutes + calories
        int sets = getIntInRange(
            "\nEnter number of sets (1-30): ", 1, 30);
        int reps = getIntInRange(
            "Enter average reps per set (1-50): ", 1, 50);

        // Approximate session duration for summary purposes
        // ~2 minutes per set (including rest), with a minimum of 5 minutes
        log.durationMinutes = std::max(5, sets * 2);

        log.caloriesBurned = estimateStrengthCalories(
            log.exercise,
            sets,
            reps,
            intensity
        );

        // Append sets x reps info to the exercise name for clarity in logs
        log.exercise += " (" + to_string(sets) + "x" + to_string(reps) + ")";

        cout << COL_MUTED
             << "\n(We estimated workout time and calories based on sets, reps, and exercise.)\n"
             << COL_RESET;
    }

    ofstream out(WORKOUT_LOG_FILE.c_str(), ios::app);
    if (!out) {
        cout << COL_WARN << "Error: Could not open workout log file.\n" << COL_RESET;
        pauseScreen();
        return;
    }

    // File format stays the same: week|date|exercise|minutes|calories
    out << log.weekNumber << '|'
        << log.date << '|'
        << log.exercise << '|'
        << log.durationMinutes << '|'
        << log.caloriesBurned << '\n';

    out.close();

    cout << COL_OK << "\nWorkout logged successfully!\n" << COL_RESET;
    cout << " Estimated time    : " << log.durationMinutes << " minutes\n";
    cout << " Estimated calories: " << log.caloriesBurned  << " kcal\n\n";

    playBeep();
    pauseScreen();
}

WeeklySummary computeWeeklySummary(int selectedWeek) {
    WeeklySummary summary;
    summary.totalMinutes  = 0;
    summary.totalCalories = 0;
    summary.daysLogged    = 0;

    ifstream in(WORKOUT_LOG_FILE.c_str());
    if (!in) return summary;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        WorkoutLog log;

        getline(ss, token, '|');
        log.weekNumber = atoi(token.c_str());
        if (log.weekNumber != selectedWeek) continue;

        getline(ss, log.date, '|');
        getline(ss, log.exercise, '|');
        getline(ss, token, '|');
        log.durationMinutes = atoi(token.c_str());
        getline(ss, token, '|');
        log.caloriesBurned = atoi(token.c_str());

        summary.totalMinutes  += log.durationMinutes;
        summary.totalCalories += log.caloriesBurned;
        summary.daysLogged++;
    }

    in.close();
    return summary;
}

void showAchievements(const WeeklySummary &summary) {
    cout << COL_MENU << "\n>>> Achievement Badges\n\n" << COL_RESET;

    bool any = false;
    if (summary.daysLogged >= 5) {
        cout << " [*] Consistency Star  (5+ days logged)\n";
        any = true;
    }
    if (summary.totalMinutes >= 300) {
        cout << " [*] Endurance Hero    (300+ minutes)\n";
        any = true;
    }
    if (summary.totalCalories >= 2000) {
        cout << " [*] Calorie Crusher   (2000+ calories)\n";
        any = true;
    }
    if (!any) {
        cout << " No badges yet — this is your chance to earn one!\n";
    }
    cout << '\n';
}

// ASCII bar for charts
void drawBar(const string &label, int value, int maxValue) {
    if (maxValue <= 0) maxValue = 1;
    int width  = 30; // bar width
    int filled = (value * width) / maxValue;

    cout << " " << label << " ";
    cout << "[";
    for (int i = 0; i < width; i++) {
        if (i < filled) cout << "#";
        else cout << "-";
    }
    cout << "] " << value << "\n";
}

// XP + level system
int calculateXP(const WeeklySummary &s) {
    int xp = 0;
    xp += s.daysLogged * 20;      // 20 XP per active day
    xp += s.totalMinutes / 5;     // 1 XP per 5 minutes
    xp += s.totalCalories / 50;   // 1 XP per 50 kcal
    return xp;
}

string levelTitle(int level) {
    if (level <= 1) return "Beginner";
    if (level == 2) return "Rookie";
    if (level == 3) return "Grinder";
    if (level == 4) return "Athlete";
    return "Beast Mode";
}

void showWeeklySummary() {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Weekly Progress Summary\n\n" << COL_RESET;
    cout << "You will see your total minutes, calories and a small bar chart\n";
    cout << "for the week you choose.\n\n";

    int week = getIntInRange("Enter week number (1-52): ", 1, 52);

    WeeklySummary summary = computeWeeklySummary(week);

    if (summary.daysLogged == 0) {
        cout << COL_WARN << "\nNo logs found for this week.\n" << COL_RESET;
        pauseScreen();
        return;
    }

    cout << "\nWeek " << week << " summary:\n";
    cout << " Days logged    : " << summary.daysLogged << '\n';
    cout << " Total minutes  : " << summary.totalMinutes << '\n';
    cout << " Total calories : " << summary.totalCalories << '\n';

    // Visual summary
    cout << "\n" << COL_MENU << "Visual summary:\n" << COL_RESET;
    int maxVal = std::max(summary.totalMinutes, summary.totalCalories);
    drawBar("Minutes ", summary.totalMinutes, maxVal);
    drawBar("Calories", summary.totalCalories, maxVal);

    showAchievements(summary);

    int xp    = calculateXP(summary);
    int level = 1 + xp / 500; // every 500 XP = new level

    cout << COL_OK << "\nGamified Progress:\n" << COL_RESET;
    cout << " XP earned this week : " << xp << "\n";
    cout << " Estimated level     : " << level
         << " (" << levelTitle(level) << ")\n\n";

    playBeep();
    pauseScreen();
}

// ===================== MENU & MAIN =====================

void showMenu() {
    cout << COL_MENU;
    cout << "==================== MAIN MENU ====================\n";
    cout << "Type the number of an option and press Enter.\n";
    cout << "If you're new, start from 1) Create new profile.\n\n";
    cout << " 1) Create new profile      (set up your details)\n";
    cout << " 2) Switch active profile   (change user)\n";
    cout << " 3) View active profile     (see your info)\n";
    cout << " 4) Update active profile   (edit age/weight/goal)\n";
    cout << " 5) Calculate BMI           (see your BMI category)\n";
    cout << " 6) Generate workout plan   (weekly exercise guide)\n";
    cout << " 7) Generate diet plan      (daily sample meals)\n";
    cout << " 8) Add daily workout log   (record what you did)\n";
    cout << " 9) View weekly summary     (see progress & graphs)\n";
    cout << " 0) Exit                    (close GitFit)\n";
    cout << "===================================================\n";
    cout << COL_RESET;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    UserProfile profiles[MAX_PROFILES];
    int profileCount        = loadAllProfiles(profiles, MAX_PROFILES);
    int currentProfileIndex = -1;

    // Choose or create profile at start (optional)
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
        choice = getIntInRange("Enter your choice (0-9): ", 0, 9);

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
                    playBeep();
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
                    addWorkoutLog();
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 9:
                if (currentProfileIndex >= 0)
                    showWeeklySummary();
                else {
                    cout << COL_WARN
                         << "Please create or select a profile first (option 1 or 2).\n"
                         << COL_RESET;
                    pauseScreen();
                }
                break;
            case 0:
                clearScreen();
                cout << "Thanks for using GitFit. Stay strong! 💪\n";
                playBeep();
                break;
        }

    } while (choice != 0);

    return 0;
}
