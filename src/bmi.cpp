#include "gitfit.hpp"

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
    pauseScreen();
}