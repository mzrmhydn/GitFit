// gitfit.hpp
#ifndef GITFIT_HPP
#define GITFIT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>   // system("cls")
#include <ctime>
#include <limits>    // numeric_limits
#include <algorithm> // std::max
#include <cctype>    // tolower
#include <vector>

using namespace std;

// DATA STRUCTURES

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
    string profileName;   // identify which profile the log belongs to
    int weekNumber;
    string exercise;
    int durationMinutes;
    int caloriesBurned;
};

struct WeeklySummary {
    int totalMinutes;
    int totalCalories;
    int daysLogged;
};

// CONSTANTS DECLARATIONS

extern const string PROFILES_FILE;      // multiple profiles
extern const string WORKOUT_LOG_FILE;
extern const int    MAX_PROFILES;

// Simple ANSI color codes (VS Code terminal / Git Bash etc.)
extern const string COL_LOCKED; // RGB grey for locked items
extern const string COL_RESET;
extern const string COL_TITLE;  // bright cyan
extern const string COL_MENU;   // yellow
extern const string COL_OK;     // green
extern const string COL_WARN;   // red
extern const string COL_MUTED;  // grey

// UTILS / UI 

void clearScreen();
void pauseScreen();
void printMainBanner();

// Input helpers
int   getIntInRange(const string &prompt, int minVal, int maxVal);
float getFloatInRange(const string &prompt, float minVal, float maxVal);
char  getCharFromOptions(const string &prompt, const string &options);
char  getYesNo(const string &prompt);

// string helpers
string toLowerString(string s);

// PROFILE HANDLING 

int  loadAllProfiles(UserProfile profiles[], int maxProfiles);
void saveAllProfiles(UserProfile profiles[], int count);
void listProfiles(UserProfile profiles[], int count);
int  chooseProfileIndex(UserProfile profiles[], int count);
void deleteWorkoutLogsForProfile(const string &profileName);

void createNewProfile(UserProfile profiles[], int &count, int &currentIndex);
void viewProfile(const UserProfile &user);
void updateCurrentProfile(UserProfile profiles[], int count, int currentIndex);
void deleteCurrentProfile(UserProfile profiles[], int &count, int &currentIndex);

// BMI 

float  calculateBMI(const UserProfile &user);
string interpretBMI(float bmi);
void   showBMI(const UserProfile &user);

// WORKOUT PLAN 

void generateWorkoutPlan(const UserProfile &user);

// DIET PLAN 

void printMeal(const string &title,
               const string &foods,
               const string &nutrients);
void generateDietPlan(const UserProfile &user);

// WORKOUT LOG & SUMMARY 

int estimateCardioCalories(const string &exercise, int durationMinutes, int intensityLevel);
int estimateStrengthCalories(const string &exercise, int sets, int repsPerSet, int intensityLevel);

void          addWorkoutLog(const UserProfile &user);
WeeklySummary computeWeeklySummary(const UserProfile &user, int selectedWeek);
WeeklySummary computeOverallSummary(const UserProfile &user);
void          showAchievements(const WeeklySummary &summary);

int    calculateXP(const WeeklySummary &s);
string levelTitle(int level);

void showWeeklySummary(const UserProfile &user);
void showAchievementsScreen(const UserProfile &user);

#endif // GITFIT_HPP
