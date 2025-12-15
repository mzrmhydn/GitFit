#include "gitfit.hpp"

// PROFILE HANDLING 

// Each profile is stored as:
// name|age|gender|heightCm|weightKg|goal|dietPreference

// load all profiles from file, returns number loaded

int loadAllProfiles(UserProfile profiles[], int maxProfiles) {
    ifstream in(PROFILES_FILE.c_str());
    if (!in) return 0;

    string line;
    int count = 0;

    while (getline(in, line) && count < maxProfiles){
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        UserProfile u;

        getline(ss, token, '|');
        u.id = atoi(token.c_str());

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

// save all profiles to file

void saveAllProfiles(UserProfile profiles[], int count) {
    ofstream out(PROFILES_FILE.c_str());
    if (!out) {
        cout << COL_WARN << "Error: Could not save profiles file.\n" << COL_RESET;
        return;
    }

    for (int i = 0; i < count; i++) {
        out << profiles[i].id << '|'
            << profiles[i].name << '|'  
            << profiles[i].age << '|'
            << profiles[i].gender << '|'
            << profiles[i].heightCm << '|'
            << profiles[i].weightKg << '|'
            << profiles[i].goal << '|'
            << profiles[i].dietPreference << '\n';
    }

    out.close();
}

// append a single profile to file

void appendProfileToFile(const UserProfile &u) {
    ofstream out(PROFILES_FILE.c_str(), ios::app);
    if (!out) {
        cout << COL_WARN << "Error: Could not append to profiles file.\n" << COL_RESET;
        return;
    }

    out << u.id << '|'
        << u.name << '|'
        << u.age << '|'
        << u.gender << '|'
        << u.heightCm << '|'
        << u.weightKg << '|'
        << u.goal << '|'
        << u.dietPreference << '\n';

    out.close();
}


// List all profiles

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

//choose profile index, returns -1 if cancelled

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

// Delete all workout logs for a given profile
void deleteWorkoutLogsForProfile(int profileId) {
    ifstream in(WORKOUT_LOG_FILE.c_str());
    if (!in) {
        // No log file yet, nothing to do
        return;
    }

    vector<string> keptLines;
    string line;

    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        getline(ss, token, '|'); // profileName is first field
        int id = atoi(token.c_str());

        if (id != profileId) {
            keptLines.push_back(line);
        }
    }
    in.close();

    ofstream out(WORKOUT_LOG_FILE.c_str());
    if (!out) {
        cout << COL_WARN << "Warning: Could not rewrite workout log file.\n"
             << COL_RESET;
        return;
    }

    for (size_t i = 0; i < keptLines.size(); ++i) {
        out << keptLines[i] << '\n';
    }
    out.close();
}

// CREATE PROFILE 

    // Get next available profile ID

int getNextProfileId(UserProfile profiles[], int count) {
    int maxId = 0;
    for (int i = 0; i < count; i++) maxId = max(maxId, profiles[i].id);
    return maxId + 1;
}


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
    
    do {
        cout << "Enter your name (or 0 to cancel): ";
        getline(cin, u.name);

        if (u.name == "0") {
            cout << COL_WARN << "Profile creation cancelled.\n" << COL_RESET;
            pauseScreen();
            return;
        }

        if (u.name.empty()) {
            cout << COL_WARN
                << "Name cannot be empty. Please enter a valid name.\n"
                << COL_RESET;
        }
    }while (u.name.empty());

    u.id       = getNextProfileId(profiles, count);
    u.age      = getIntInRange("Enter your age (10-75): ", 15, 75);
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

    switch(g) {
    case 1:
        u.goal = "lose";
        break;
    case 2:
        u.goal = "maintain";
        break;
    case 3:
        u.goal = "gain";
        break;
    case 4:
        u.goal = "strength";
        break;
    default:
        u.goal = "stamina";
    }

    cout << "\nDiet preference:\n";
    cout << " 1) veg\n";
    cout << " 2) non-veg\n";
    int d = getIntInRange("Enter choice (1-2): ", 1, 2);
    u.dietPreference = (d == 1 ? "veg" : "non-veg");

    cout << COL_OK << "\nProfile info captured!\n" << COL_RESET;

    profiles[count] = u;
    count++;
    currentIndex = count - 1;

    appendProfileToFile(u);

    cout << COL_OK << "New profile saved and set as active.\n" << COL_RESET;
    pauseScreen();
}

// VIEW PROFILE 

void viewProfile(const UserProfile &user) {
    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Active Profile Overview\n\n" << COL_RESET;
    cout << " ---------------------------------------------------\n";
    cout << " Profile Details:\n\n";
    cout << " ID             : " << user.id << '\n';
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

// UPDATE PROFILE

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
        cout << COL_OK << "\nField updated successfully.\n" << COL_RESET;
        pauseScreen();
    }
}

// DELETE CURRENT PROFILE (and its logs)

void deleteCurrentProfile(UserProfile profiles[], int &count, int &currentIndex) {
    if (currentIndex < 0 || currentIndex >= count) {
        cout << COL_WARN << "No active profile to delete.\n" << COL_RESET;
        pauseScreen();
        return;
    }

    clearScreen();
    printMainBanner();

    cout << COL_TITLE << ">>> Delete Active Profile\n\n" << COL_RESET;
    cout << COL_WARN
         << "Warning: This will permanently delete profile '"
         << profiles[currentIndex].name
         << "' and all of its workout logs.\n"
         << COL_RESET;

    char confirm = getCharFromOptions("Are you sure you want to continue? (y/n): ", "YyNn");
    confirm = tolower(confirm);
    
    if (confirm != 'y') {
        cout << "\nDeletion cancelled.\n";
        pauseScreen();
        return;
    }

    // Shift profiles left to fill the gap
    for (int i = currentIndex; i < count - 1; ++i) {
        profiles[i] = profiles[i + 1];
    }
    count--;

    saveAllProfiles(profiles, count);
    deleteWorkoutLogsForProfile(profiles[currentIndex].id);

    if (count == 0) {
        currentIndex = -1;
        cout << COL_WARN << "\nProfile deleted. No profiles left.\n" << COL_RESET;
    } else {
        if (currentIndex >= count) {
            currentIndex = count - 1;
        }
        cout << COL_OK << "\nProfile deleted. Active profile is now: "
             << profiles[currentIndex].name << "\n" << COL_RESET;
    }

    pauseScreen();
}