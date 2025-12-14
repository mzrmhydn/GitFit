#include "gitfit.hpp"

//  WORKOUT PLAN 

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

    // Age-based recommendation 
    int ageDifficulty = 2; // default moderate

    if (user.age >= 15 && user.age <= 30) {
        ageDifficulty = 3; // challenging
    } else if (user.age > 30 && user.age <= 50) {
        ageDifficulty = 2; // moderate
    } else if (user.age > 50 && user.age <= 75) {
        ageDifficulty = 1; // easy
    }

    cout << "\nBased on your age (" << user.age << "), recommended intensity is:\n";
    if (ageDifficulty == 3)
        cout << " -> Advanced / challenging (15-30 years)\n";
    else if (ageDifficulty == 2)
        cout << " -> Intermediate / moderate (30-50 years)\n";
    else
        cout << " -> Beginner / easy (50-75 years)\n";

    cout << "\nChoose your actual workout difficulty:\n";
    cout << " 1) Beginner";
    if (ageDifficulty == 1) cout << "  [recommended]";
    cout << "\n 2) Intermediate";
    if (ageDifficulty == 2) cout << "  [recommended]";
    cout << "\n 3) Advanced";
    if (ageDifficulty == 3) cout << "  [recommended]";
    cout << "\n";

    int difficulty = getIntInRange("Enter choice (1-3): ", 1, 3);
    //_________________

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
                cout << "  * 5-10 min cool-down walk\n";
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
                cout << "  * 10-15 min dynamic stretching\n";
                cout << "  * 3 sets of core exercises (plank, leg raises, dead bug)\n";
                cout << "  * Easy walk to finish\n";
            }
        }

        cout << "----------------------------------------------\n\n";
    }

    cout << "Tip: Always warm up 5-10 min and cool down 5 min.\n\n";
    pauseScreen();
}
