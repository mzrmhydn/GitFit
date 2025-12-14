#include "gitfit.hpp"

// DIET PLAN 

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
    pauseScreen();
}