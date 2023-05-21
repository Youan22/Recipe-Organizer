#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// Recipe class
class Recipe {
private:
    string name;
    vector<string> ingredients;
    string instructions;

public:
    Recipe(string n, vector<string> ingr, string instr) : name(n), ingredients(ingr), instructions(instr) {}

    string getName() const {
        return name;
    }

    void displayRecipe() const {
        cout << "Recipe Name: " << name << endl;
        cout << "Ingredients:" << endl;
        for (size_t i = 0; i < ingredients.size(); ++i) {
            cout << "- " << ingredients[i] << endl;
        }
        cout << "Instructions: " << instructions << endl;
        cout << endl;
    }
};

// Recipe organizer class
class RecipeOrganizer {
private:
    vector<Recipe*> recipes;
    map<string, vector<Recipe* > > recipeMap;

public:
    RecipeOrganizer() {}

    ~RecipeOrganizer() {
        for (size_t i = 0; i < recipes.size(); ++i) {
            delete recipes[i];
        }
    }

    void addRecipe(Recipe* recipe) {
        recipes.push_back(recipe);
        recipeMap[recipe->getName()].push_back(recipe);
    }

    void displayAllRecipes() const {
        for (size_t i = 0; i < recipes.size(); ++i) {
            recipes[i]->displayRecipe();
        }
    }

    void searchRecipesByName(const string& name) const {
        if (recipeMap.count(name) > 0) {
            const vector<Recipe*>& matchingRecipes = recipeMap.at(name);
            for (size_t i = 0; i < matchingRecipes.size(); ++i) {
                matchingRecipes[i]->displayRecipe();
            }
        } else {
            cout << "No recipes found with the given name." << endl;
        }
    }
};

// Helper function to trim leading and trailing whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == string::npos || last == string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

// Function to read recipes from a file and add them to the Recipe Organizer
void loadRecipesFromFile(const string& filename, RecipeOrganizer& organizer) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        string recipeName;
        vector<string> ingredients;
        string instructions;

        while (getline(file, line)) {
            if (line.empty()) {
                // Recipe ends, add it to the organizer
                if (!recipeName.empty() && !ingredients.empty() && !instructions.empty()) {
                    Recipe* recipe = new Recipe(recipeName, ingredients, instructions);
                    organizer.addRecipe(recipe);
                }
                // Reset variables for the next recipe
                recipeName = "";
                ingredients.clear();
                instructions = "";
            } else {
                if (recipeName.empty()) {
                    recipeName = line;
                } else if (instructions.empty()) {
                    instructions = line;
                } else {
                    ingredients.push_back(trim(line));
                }
            }
        }

        // Add the last recipe in case the file does not end with an empty line
        if (!recipeName.empty() && !ingredients.empty() && !instructions.empty()) {
            Recipe* recipe = new Recipe(recipeName, ingredients, instructions);
            organizer.addRecipe(recipe);
        }

        file.close();
    } else {
        cout << "Failed to open the file." << endl;
    }
}

// Function to get user input for a new recipe
Recipe* getUserInputForRecipe() {
    string name;
    cout << "Enter recipe name: ";
    getline(cin, name);

    vector<string> ingredients;
    string ingredient;
    cout << "Enter ingredients (one per line, leave empty to finish):" << endl;
    while (getline(cin, ingredient) && !ingredient.empty()) {
        ingredients.push_back(trim(ingredient));
    }

    string instructions;
    cout << "Enter instructions:" << endl;
    getline(cin, instructions);

    Recipe* recipe = new Recipe(name, ingredients, instructions);
    return recipe;
}

int main() {
    RecipeOrganizer organizer;
    string filename = "recipes.txt";

    // Load recipes from file
    loadRecipesFromFile(filename, organizer);

    // User menu
    int choice;
    do {
        cout << "Menu:" << endl;
        cout << "1. Display all recipes" << endl;
        cout << "2. Search recipes by name" << endl;
        cout << "3. Add a new recipe" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Ignore the newline character after reading choice

        switch (choice) {
            case 1:
                cout << "All Recipes:" << endl;
                organizer.displayAllRecipes();
                break;
            case 2: {
                string name;
                cout << "Enter the recipe name to search for: ";
                getline(cin, name);
                cout << "Matching Recipes:" << endl;
                organizer.searchRecipesByName(name);
                break;
            }
            case 3: {
                Recipe* recipe = getUserInputForRecipe();
                organizer.addRecipe(recipe);
                cout << "Recipe added successfully!" << endl;
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;

    } while (choice != 0);

    return 0;
}
