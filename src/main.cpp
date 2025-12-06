#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // system("cls")
#include <ctime>
#include <limits>   // numeric_limits

using namespace std;

// ===================== DATA STRUCTURES =====================

struct UserProfile {
    string name;
    int age;
    char gender;          // 'M' / 'F'
    float heightCm;
    float weightKg;
    string goal;          // lose / maintain / gain / strength / stamina
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

const string PROFILES_FILE     = "profiles.txt";      // multiple profiles
const string WORKOUT_LOG_FILE  = "workout_log.txt";
const int    MAX_PROFILES      = 100;

// ===================== UTILS / UI =====================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void playBeep() {
    cout << '\a'; // simple console beep
}

void printMainBanner() {
    cout << "=====================================================\n";
    cout << "   ____ _ _   _____ _ _   \n";
    cout << "  / ___(_) | |_   _(_| |_ \n";
    cout << " | |  _| | |   | |  | | __|\n";
    cout << " | |_| | | |___| |  | | |_ \n";
    cout << "  \\____|_|_____|_|  |_|\\__|\n";
    cout << "                Your Console Fitness Buddy\n";
    cout << "=====================================================\n";
    cout << "          [==]----o  GITFIT  o----[==]\n\n";
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
        cout << "Error: Could not save profiles file.\n";
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
        cout << (i + 1) << ") " << profiles[i].name
             << "  [" << profiles[i].goal
             << ", " << profiles[i].dietPreference << "]\n";
    }
    cout << "-----------------------------------------------------\n";
}

int chooseProfileIndex(UserProfile profiles[], int count) {
    if (count == 0) {
        cout << "No profiles yet.\n";
        return -1;
    }

    listProfiles(profiles, count);
    cout << "\nEnter profile number to select (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice <= 0 || choice > count) return -1;
    return choice - 1;
}

void inputProfileData(UserProfile &user, bool isUpdate) {
    clearScreen();
    printMainBanner();

    if (isUpdate)
        cout << ">>> Update Current Profile\n\n";
    else
        cout << ">>> Create New Profile\n\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter name: ";
    getline(cin, user.name);

    cout << "Enter age: ";
    cin >> user.age;

    cout << "Enter gender (M/F): ";
    cin >> user.gender;

    cout << "Enter height (in cm): ";
    cin >> user.heightCm;

    cout << "Enter weight (in kg): ";
    cin >> user.weightKg;

    cout << "\nChoose main goal:\n";
    cout << " 1) lose      - Fat loss\n";
    cout << " 2) maintain  - General fitness\n";
    cout << " 3) gain      - Weight / muscle gain\n";
    cout << " 4) strength  - Get stronger\n";
    cout << " 5) stamina   - Improve endurance\n";
    cout << "Enter choice (1-5): ";
    int g;
    cin >> g;
    if      (g == 1) user.goal = "lose";
    else if (g == 2) user.goal = "maintain";
    else if (g == 3) user.goal = "gain";
    else if (g == 4) user.goal = "strength";
    else             user.goal = "stamina";

    cout << "\nDiet preference:\n";
    cout << " 1) veg\n";
    cout << " 2) non-veg\n";
    cout << "Enter choice (1-2): ";
    int d;
    cin >> d;
    user.dietPreference = (d == 1 ? "veg" : "non-veg");

    playBeep();
    cout << "\nProfile info captured!\n";
}

void createNewProfile(UserProfile profiles[], int &count, int &currentIndex) {
    if (count >= MAX_PROFILES) {
        cout << "Maximum number of profiles reached.\n";
        pauseScreen();
        return;
    }

    UserProfile u;
    inputProfileData(u, false);

    profiles[count] = u;
    count++;
    currentIndex = count - 1;

    saveAllProfiles(profiles, count);
    cout << "\nNew profile saved and set as active.\n";
    pauseScreen();
}

void updateCurrentProfile(UserProfile profiles[], int count, int currentIndex) {
    if (currentIndex < 0 || currentIndex >= count) {
        cout << "No active profile to update.\n";
        pauseScreen();
        return;
    }

    inputProfileData(profiles[currentIndex], true);
    saveAllProfiles(profiles, count);
    cout << "\nProfile updated successfully.\n";
    pauseScreen();
}

void viewProfile(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << ">>> Active Profile Overview\n\n";
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

    cout << ">>> BMI & Category\n\n";
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

    cout << ">>> Personalized Workout Planner\n\n";
    cout << "We will ask a few quick questions.\n\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string goal = user.goal;
    char change;
    cout << "Use profile goal '" << user.goal << "'? (y/n): ";
    cin >> change;

    if (change == 'n' || change == 'N') {
        cout << "\nChoose goal:\n";
        cout << " 1) lose\n";
        cout << " 2) maintain\n";
        cout << " 3) gain\n";
        cout << " 4) strength\n";
        cout << " 5) stamina\n";
        cout << "Enter choice: ";
        int g;
        cin >> g;
        if      (g == 1) goal = "lose";
        else if (g == 2) goal = "maintain";
        else if (g == 3) goal = "gain";
        else if (g == 4) goal = "strength";
        else             goal = "stamina";
    }

    int dailyMinutes;
    int daysPerWeek;
    int difficulty;

    cout << "\nHow many minutes can you give per workout day? ";
    cin >> dailyMinutes;

    cout << "How many days can you work out per week? ";
    cin >> daysPerWeek;

    do{
        cout << "\nChoose difficulty level:\n";
        cout << " 1) Beginner\n";
        cout << " 2) Intermediate\n";
        cout << " 3) Advanced\n";
        cout << "Enter choice: ";
        cin >> difficulty;
        if(difficulty < 1 || difficulty > 3){
            cout << "Invalid choice. Please select between 1 and 3.\n";
        }
    }while(difficulty < 1 || difficulty > 3);

    clearScreen();
    printMainBanner();
    cout << ">>> Weekly Workout Plan (" << goal << ")\n\n";

    cout << "Time commitment: " << dailyMinutes << " min/day, "
         << daysPerWeek << " days/week\n\n";

    // Simple ASCII layout: Day-wise plan
    string days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                      "Friday", "Saturday", "Sunday"};

    for (int d = 0; d < daysPerWeek && d < 7; d++) {
        cout << "---------------- " << days[d] << " ----------------\n";

        if (goal == "lose") {
            if (difficulty == 1) {
                cout << "  * 10 min brisk walk\n";
                cout << "  * 10 min light bodyweight circuit\n";
                cout << "  * 5 min stretching\n";
            } else if (difficulty == 2) {
                cout << "  * 15 min jogging\n";
                cout << "  * 3 x 12 squats + lunges\n";
                cout << "  * 3 x 20 sec plank\n";
            } else {
                cout << "  * 20 min interval running (1 min fast / 1 min slow)\n";
                cout << "  * 4 x 15 burpees / jump squats\n";
                cout << "  * 10 min core circuit\n";
            }
        } else if (goal == "gain" || goal == "strength") {
            if (difficulty == 1) {
                cout << "  * Push-ups 3 x 8\n";
                cout << "  * Bodyweight squats 3 x 10\n";
                cout << "  * Dumbbell rows 3 x 10 (if available)\n";
            } else if (difficulty == 2) {
                cout << "  * Bench press 4 x 8\n";
                cout << "  * Squats 4 x 8\n";
                cout << "  * Bent-over rows 4 x 8\n";
            } else {
                cout << "  * Heavy compound lifts (5 x 5):\n";
                cout << "      - Squat\n";
                cout << "      - Bench press\n";
                cout << "      - Deadlift\n";
                cout << "  * Accessory strength: pull-ups / dips 4 x 8\n";
            }
        } else { // maintain / stamina
            if (difficulty == 1) {
                cout << "  * 15 min walk\n";
                cout << "  * 10 min mobility (arm circles, hip circles)\n";
            } else if (difficulty == 2) {
                cout << "  * 20-25 min steady-state cardio (cycling, brisk walk)\n";
                cout << "  * 3 x 12 push-ups + squats\n";
            } else {
                cout << "  * 30-40 min mixed cardio (jog + skipping)\n";
                cout << "  * Short bodyweight circuit: 3 rounds\n";
            }
        }

        cout << "----------------------------------------------\n\n";
    }

    cout << "Tip: Always warm up 5-10 min and cool down 5 min.\n\n";
    playBeep();
    pauseScreen();
}

// ===================== DIET PLAN =====================

void printMeal(const string &title,
               const string &foods,
               const string &nutrients) {
    cout << " " << title << ":\n";
    cout << "   Foods    : " << foods << '\n';
    cout << "   Nutrients: " << nutrients << "\n\n";
}

void generateDietPlan(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << ">>> Diet Planner (" << user.dietPreference << ", goal: "
         << user.goal << ")\n\n";

    string dietType = user.dietPreference;
    char ch;
    cout << "Use profile diet preference '" << dietType << "'? (y/n): ";
    cin >> ch;
    if (ch == 'n' || ch == 'N') {
        cout << "Choose preference: 1) veg  2) non-veg: ";
        int x;
        cin >> x;
        dietType = (x == 1 ? "veg" : "non-veg");
    }

    clearScreen();
    printMainBanner();
    cout << ">>> Sample Day Diet Plan (" << dietType << ")\n\n";

    // We create 2 choices per combo
    cout << "Choose plan variant:\n";
    cout << " 1) Balanced\n";
    cout << " 2) Higher protein\n";
    cout << "Enter choice: ";
    int variant;
    cin >> variant;

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
                          "Fish curry with vegetables (no heavy oil)",
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

int estimateCalories(int durationMinutes, int intensityLevel) {
    int caloriesPerMinute;
    if (intensityLevel == 1)      caloriesPerMinute = 5;
    else if (intensityLevel == 2) caloriesPerMinute = 8;
    else                          caloriesPerMinute = 10;
    return durationMinutes * caloriesPerMinute;
}

void addWorkoutLog() {
    clearScreen();
    printMainBanner();

    cout << ">>> Add Daily Workout Log\n\n";

    WorkoutLog log;
    cout << "Enter week number (1-52): ";
    cin >> log.weekNumber;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> log.date;
    cout << "Enter exercise (one word, e.g. Running): ";
    cin >> log.exercise;
    cout << "Enter duration (minutes): ";
    cin >> log.durationMinutes;

    int intensity;
    cout << "Intensity (1=Light, 2=Moderate, 3=Intense): ";
    cin >> intensity;

    log.caloriesBurned = estimateCalories(log.durationMinutes, intensity);

    ofstream out(WORKOUT_LOG_FILE.c_str(), ios::app);
    if (!out) {
        cout << "Error: Could not open workout log file.\n";
        pauseScreen();
        return;
    }

    out << log.weekNumber << '|'
        << log.date << '|'
        << log.exercise << '|'
        << log.durationMinutes << '|'
        << log.caloriesBurned << '\n';

    out.close();

    cout << "\nLog saved. Estimated calories burned: "
         << log.caloriesBurned << "\n";
    playBeep();
    pauseScreen();
}

WeeklySummary computeWeeklySummary(int selectedWeek) {
    WeeklySummary summary;
    summary.totalMinutes = 0;
    summary.totalCalories = 0;
    summary.daysLogged = 0;

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

        summary.totalMinutes += log.durationMinutes;
        summary.totalCalories += log.caloriesBurned;
        summary.daysLogged++;
    }

    in.close();
    return summary;
}

void showAchievements(const WeeklySummary &summary) {
    cout << "\n>>> Achievement Badges\n\n";

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

void showWeeklySummary() {
    clearScreen();
    printMainBanner();

    cout << ">>> Weekly Progress Summary\n\n";
    cout << "Enter week number (1-52): ";
    int week;
    cin >> week;

    WeeklySummary summary = computeWeeklySummary(week);

    if (summary.daysLogged == 0) {
        cout << "\nNo logs found for this week.\n";
        pauseScreen();
        return;
    }

    cout << "\nWeek " << week << " summary:\n";
    cout << " Days logged    : " << summary.daysLogged << '\n';
    cout << " Total minutes  : " << summary.totalMinutes << '\n';
    cout << " Total calories : " << summary.totalCalories << '\n';

    showAchievements(summary);
    playBeep();
    pauseScreen();
}

// ===================== MENU & MAIN =====================

void showMenu() {
    cout << "==================== MAIN MENU ====================\n";
    cout << " 1) Create new profile\n";
    cout << " 2) Switch active profile\n";
    cout << " 3) View active profile\n";
    cout << " 4) Update active profile\n";
    cout << " 5) Calculate BMI\n";
    cout << " 6) Generate workout plan\n";
    cout << " 7) Generate diet plan\n";
    cout << " 8) Add daily workout log\n";
    cout << " 9) View weekly progress summary\n";
    cout << " 0) Exit\n";
    cout << "===================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    UserProfile profiles[MAX_PROFILES];
    int profileCount = loadAllProfiles(profiles, MAX_PROFILES);
    int currentProfileIndex = -1;

    // Choose or create profile at start
    if (profileCount > 0) {
        clearScreen();
        printMainBanner();
        cout << "Found " << profileCount << " saved profile(s).\n\n";
        int idx = chooseProfileIndex(profiles, profileCount);
        if (idx >= 0) {
            currentProfileIndex = idx;
        } else {
            cout << "\nNo profile selected. You can create one from the menu.\n";
            pauseScreen();
        }
    } else {
        clearScreen();
        printMainBanner();
        cout << "No profiles found yet. Create one from the menu!\n\n";
        pauseScreen();
    }

    int choice;
    do {
        clearScreen();
        printMainBanner();

        if (currentProfileIndex >= 0) {
            cout << "Active profile: " << profiles[currentProfileIndex].name
                 << "  [" << profiles[currentProfileIndex].goal
                 << ", " << profiles[currentProfileIndex].dietPreference << "]\n\n";
        } else {
            cout << "No active profile selected.\n\n";
        }

        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                createNewProfile(profiles, profileCount, currentProfileIndex);
                break;
            case 2: {
                int idx = chooseProfileIndex(profiles, profileCount);
                if (idx >= 0) {
                    currentProfileIndex = idx;
                    cout << "\nActive profile changed to: "
                         << profiles[currentProfileIndex].name << '\n';
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
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 4:
                if (currentProfileIndex >= 0)
                    updateCurrentProfile(profiles, profileCount, currentProfileIndex);
                else {
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 5:
                if (currentProfileIndex >= 0)
                    showBMI(profiles[currentProfileIndex]);
                else {
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 6:
                if (currentProfileIndex >= 0)
                    generateWorkoutPlan(profiles[currentProfileIndex]);
                else {
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 7:
                if (currentProfileIndex >= 0)
                    generateDietPlan(profiles[currentProfileIndex]);
                else {
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 8:
                if (currentProfileIndex >= 0)
                    addWorkoutLog();
                else {
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 9:
                if (currentProfileIndex >= 0)
                    showWeeklySummary();
                else {
                    cout << "Please create or select a profile first.\n";
                    pauseScreen();
                }
                break;
            case 0:
                clearScreen();
                cout << "Thanks for using GitFit. Stay strong! 💪\n";
                playBeep();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                pauseScreen();
        }

    } while (choice != 0);

    return 0;
}
