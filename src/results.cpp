// Workout Logs, Summary and Achievements

#include "gitfit.hpp"

// CARDIO_EXERCISES, STRENGTH_EXERCISES, OTHER_EXERCISES arrays + counts

// Exercise Lists (for selection in Add Workout Log) 

const string CARDIO_EXERCISES[] = {
    "Walking (light)",
    "Brisk walking",
    "Jogging",
    "Running",
    "Cycling (outdoor)",
    "Stationary bike",
    "Treadmill run",
    "Elliptical trainer",
    "Swimming (easy)",
    "Swimming (laps)",
    "Skipping rope",
    "Stair climbing",
    "Rowing machine",
    "Aerobic dance / Zumba",
    "HIIT cardio circuit"
};
const int NUM_CARDIO_EXERCISES =
    sizeof(CARDIO_EXERCISES) / sizeof(CARDIO_EXERCISES[0]);

const string STRENGTH_EXERCISES[] = {
    "Push-ups",
    "Bench press",
    "Incline dumbbell press",
    "Pull-ups / Lat pulldown",
    "Barbell rows",
    "Deadlift",
    "Barbell squats",
    "Leg press",
    "Lunges",
    "Shoulder press",
    "Dumbbell shoulder press",
    "Bicep curls",
    "Tricep dips",
    "Plank + core circuit",
    "Kettlebell swings",
    "Hip thrusts / glute bridge"
};
const int NUM_STRENGTH_EXERCISES =
    sizeof(STRENGTH_EXERCISES) / sizeof(STRENGTH_EXERCISES[0]);

const string OTHER_EXERCISES[] = {
    "Mixed full-body circuit",
    "Sports (football/cricket/basketball)",
    "Yoga / mobility",
    "Home chores workout"
};
const int NUM_OTHER_EXERCISES =
    sizeof(OTHER_EXERCISES) / sizeof(OTHER_EXERCISES[0]);


// WORKOUT LOG & SUMMARY 

// Cardio: calories depend on exercise name + intensity
int estimateCardioCalories(const string &exercise, int durationMinutes, int intensityLevel) {
    string e = toLowerString(exercise);

    int basePerMin = 6; // default moderate

    if (e.find("walk") != string::npos)
        basePerMin = 4;
    else if (e.find("jog") != string::npos)
        basePerMin = 7;
    else if (e.find("run") != string::npos)
        basePerMin = 10;
    else if (e.find("cycle") != string::npos || e.find("bike") != string::npos)
        basePerMin = 8;
    else if (e.find("swim") != string::npos)
        basePerMin = 9;
    else if (e.find("elliptical") != string::npos)
        basePerMin = 8;
    else if (e.find("row") != string::npos)
        basePerMin = 9;
    else if (e.find("stair") != string::npos)
        basePerMin = 8;
    else if (e.find("rope") != string::npos)
        basePerMin = 10;
    else if (e.find("hiit") != string::npos)
        basePerMin = 11;
    else if (e.find("dance") != string::npos || e.find("zumba") != string::npos)
        basePerMin = 7;
    else if (e.find("sport") != string::npos)
        basePerMin = 8;

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

void addWorkoutLog(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Add Daily Workout Log\n\n" << COL_RESET;
    cout << "This section lets you record one workout for a specific week.\n";
    cout << "Pick the type of workout, then choose an exercise from the list.\n\n";

    WorkoutLog log;
    log.profileName = user.name;  // bind log to current profile
    log.weekNumber = getIntInRange(
        "Enter week number for this workout (1-52): ", 1, 52);

    cout << "\nWhat type of workout did you do today?\n";
    cout << " 1) Cardio (walking, running, cycling, treadmill, etc.)\n";
    cout << " 2) Strength / weights (push-ups, squats, bench press, etc.)\n";
    cout << " 3) Other / mixed (sports, yoga, mixed circuits)\n";
    int workoutType = getIntInRange("Choose workout type (1-3): ", 1, 3);

    string baseExerciseName;

    if (workoutType == 1) {
        cout << "\nSelect a CARDIO exercise from the list:\n";
        for (int i = 0; i < NUM_CARDIO_EXERCISES; ++i) {
            cout << " " << (i + 1) << ") " << CARDIO_EXERCISES[i] << "\n";
        }
        int exChoice = getIntInRange(
            "Enter your choice (1-" + to_string(NUM_CARDIO_EXERCISES) + "): ",
            1, NUM_CARDIO_EXERCISES
        );
        baseExerciseName = CARDIO_EXERCISES[exChoice - 1];
    } else if (workoutType == 2) {
        cout << "\nSelect a STRENGTH exercise from the list:\n";
        for (int i = 0; i < NUM_STRENGTH_EXERCISES; ++i) {
            cout << " " << (i + 1) << ") " << STRENGTH_EXERCISES[i] << "\n";
        }
        int exChoice = getIntInRange(
            "Enter your choice (1-" + to_string(NUM_STRENGTH_EXERCISES) + "): ",
            1, NUM_STRENGTH_EXERCISES
        );
        baseExerciseName = STRENGTH_EXERCISES[exChoice - 1];
    } else { // Other / mixed
        cout << "\nSelect the option that best matches your workout:\n";
        for (int i = 0; i < NUM_OTHER_EXERCISES; ++i) {
            cout << " " << (i + 1) << ") " << OTHER_EXERCISES[i] << "\n";
        }
        int exChoice = getIntInRange(
            "Enter your choice (1-" + to_string(NUM_OTHER_EXERCISES) + "): ",
            1, NUM_OTHER_EXERCISES
        );
        baseExerciseName = OTHER_EXERCISES[exChoice - 1];
    }

    cout << "\nRate how hard this session felt:\n";
    cout << " 1) Easy\n";
    cout << " 2) Moderate\n";
    cout << " 3) Hard\n";
    int intensity = getIntInRange("Intensity (1-3): ", 1, 3);

    if (workoutType == 1 || workoutType == 3) {
        // CARDIO / OTHER: ask for duration only
        cout << "\nEnter duration of the cardio part in minutes (5-300): ";
        log.durationMinutes = getIntInRange("", 5, 300);

        log.exercise = baseExerciseName;
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

        log.exercise = baseExerciseName;
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

    // File format: profileName|week|exercise|minutes|calories
    out << log.profileName << '|'
        << log.weekNumber << '|'
        << log.exercise << '|'
        << log.durationMinutes << '|'
        << log.caloriesBurned << '\n';

    out.close();

    cout << COL_OK << "\nWorkout logged successfully!\n" << COL_RESET;
    cout << " Profile           : " << log.profileName    << "\n";
    cout << " Exercise          : " << log.exercise        << "\n";
    cout << " Estimated time    : " << log.durationMinutes << " minutes\n";
    cout << " Estimated calories: " << log.caloriesBurned  << " kcal\n\n";

    pauseScreen();
}

WeeklySummary computeWeeklySummary(const UserProfile &user, int selectedWeek) {
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
        WorkoutLog log;
        string token;

        // profileName
        getline(ss, log.profileName, '|');
        if (log.profileName != user.name) {
            // log belongs to some other profile
            continue;
        }

        // weekNumber
        getline(ss, token, '|');
        log.weekNumber = atoi(token.c_str());
        if (log.weekNumber != selectedWeek) continue;

        // exercise
        getline(ss, log.exercise, '|');

        // durationMinutes
        getline(ss, token, '|');
        log.durationMinutes = atoi(token.c_str());

        // caloriesBurned
        getline(ss, token, '|');
        log.caloriesBurned = atoi(token.c_str());

        summary.totalMinutes  += log.durationMinutes;
        summary.totalCalories += log.caloriesBurned;
        summary.daysLogged++;
    }

    in.close();
    return summary;
}

// Overall summary across all weeks
WeeklySummary computeOverallSummary(const UserProfile &user) {
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
        WorkoutLog log;
        string token;

        // profileName
        getline(ss, log.profileName, '|');
        if (log.profileName != user.name) {
            continue;
        }

        // weekNumber (ignored, but consumed)
        getline(ss, token, '|');
        log.weekNumber = atoi(token.c_str());

        // exercise
        getline(ss, log.exercise, '|');

        // durationMinutes
        getline(ss, token, '|');
        log.durationMinutes = atoi(token.c_str());

        // caloriesBurned
        getline(ss, token, '|');
        log.caloriesBurned = atoi(token.c_str());

        summary.totalMinutes  += log.durationMinutes;
        summary.totalCalories += log.caloriesBurned;
        summary.daysLogged++;   // assume one log = one workout day
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

void showWeeklySummary(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Weekly Progress Summary\n\n" << COL_RESET;
    cout << "You will see your total minutes, calories, and badges\n";
    cout << "for the week you choose (only for your profile).\n\n";

    int week = getIntInRange("Enter week number (1-52): ", 1, 52);

    WeeklySummary summary = computeWeeklySummary(user, week);

    if (summary.daysLogged == 0) {
        cout << COL_WARN << "\nNo logs found for this week for profile '"
             << user.name << "'.\n" << COL_RESET;
        pauseScreen();
        return;
    }

    cout << "\nProfile: " << user.name << "\n";
    cout << "Week " << week << " summary:\n";
    cout << " Days logged    : " << summary.daysLogged << '\n';
    cout << " Total minutes  : " << summary.totalMinutes << "\n";
    cout << " Total calories : " << summary.totalCalories << "\n";

    showAchievements(summary);

    int xp    = calculateXP(summary);
    int level = 1 + xp / 500; // every 500 XP = new level

    cout << COL_OK << "\nGamified Progress:\n" << COL_RESET;
    cout << " XP earned this week : " << xp << "\n";
    cout << " Estimated level     : " << level
         << " (" << levelTitle(level) << ")\n\n";

    pauseScreen();
}

// Achievements screen (all-time)
void showAchievementsScreen(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Achievements & Progress\n\n" << COL_RESET;

    WeeklySummary total = computeOverallSummary(user);

    if (total.daysLogged == 0) {
        cout << COL_WARN << "No workouts logged yet for profile '"
             << user.name << "'.\n" << COL_RESET;
        cout << "Log some workouts to start unlocking achievements!\n\n";
        pauseScreen();
        return;
    }

    cout << "Profile: " << user.name << "\n";
    cout << "Total workouts logged : " << total.daysLogged    << "\n";
    cout << "Total minutes trained  : " << total.totalMinutes << "\n";
    cout << "Total calories burned  : " << total.totalCalories<< "\n\n";

    const int REQ_DAYS     = 5;
    const int REQ_MINUTES  = 300;
    const int REQ_CALORIES = 2000;

    bool consistencyUnlocked = (total.daysLogged    >= REQ_DAYS);
    bool enduranceUnlocked   = (total.totalMinutes  >= REQ_MINUTES);
    bool calorieUnlocked     = (total.totalCalories >= REQ_CALORIES);

    cout << COL_MENU << "Achievement List\n" << COL_RESET;

    // Consistency Star
    cout << (consistencyUnlocked ? "" : COL_MUTED);
    cout << "\n- Consistency Star (Log workouts on 5 days)\n";
    cout << "  Status  : " << (consistencyUnlocked ? "Unlocked" : "Locked") << "\n";
    cout << "  Progress: " << total.daysLogged << "/" << REQ_DAYS << " days\n";
    cout << COL_RESET;
    
    // Endurance Hero
    cout << (enduranceUnlocked ? "" : COL_MUTED);
    cout << "\n- Endurance Hero (Accumulate 300 minutes of training)\n";
    cout << "  Status  : " << (enduranceUnlocked ? "Unlocked" : "Locked") << "\n";
    cout << "  Progress: " << total.totalMinutes << "/" << REQ_MINUTES << " min\n";
    cout << COL_RESET;
    
    // Calorie Crusher
    cout << (calorieUnlocked ? "" : COL_MUTED);
    cout << "\n- Calorie Crusher (Burn 2000 total calories)\n";
    cout << "  Status  : " << (calorieUnlocked ? "Unlocked" : "Locked") << "\n";
    cout << "  Progress: " << total.totalCalories << "/" << REQ_CALORIES << " kcal\n";
    cout << COL_RESET;

    // XP + Level (overall)
    int xp    = calculateXP(total);
    int level = 1 + xp / 500;
    int nextLevelXP = level * 500;         // threshold for next level
    int xpToNext    = std::max(0, nextLevelXP - xp);

    cout << COL_OK << "\nOverall Progress:\n" << COL_RESET;
    cout << " Current XP       : " << xp << "\n";
    cout << " Current level    : " << level
         << " (" << levelTitle(level) << ")\n";
    cout << " XP to next level : " << xpToNext << "\n\n";

    pauseScreen();
}