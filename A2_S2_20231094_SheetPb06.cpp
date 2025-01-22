#include <bits/stdc++.h>
#include <fstream>
#include <cstdlib>
#include <windows.h> // for system("cls")

using namespace std;

// Class representing the "Game of Life" universe
class Universe {
private:
    int rows, cols;                     // Dimensions of the grid
    vector<vector<bool>> grid;          // 2D grid representing the game state (alive or dead cells)

public:
    // Constructor: Initializes the grid with given dimensions and sets all cells as dead by default
    Universe(int r, int c) : rows(r), cols(c), grid(r, vector<bool>(c, false)) {}


    // Clears the terminal and displays a game header
    static void clear_terminal() {
        system("cls"); // Clear the terminal (Windows-specific)
        cout << "*****************>>> GAME OF LIFE <<<*****************\n\n";
    }

    // Randomly initializes the grid with a given percentage of alive cells
    void initialize(double aliveProbability = 0.2) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = (rand() / double(RAND_MAX)) < aliveProbability; // Randomly assign cells as alive or dead
            }
        }
    }

    // Resets all cells in the grid to the dead state
    void reset() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = false;
            }
        }
    }

    // Sets a specific cell (by number) as alive, validating the input
    void set_cell_alive(int cellNumber) {
        if (cellNumber < 1 || cellNumber > rows * cols) {
            cerr << "Invalid cell number. Try again.\n";
            return;
        }

        // Convert the cell number to grid coordinates
        int x = (cellNumber - 1) / cols; // Row index
        int y = (cellNumber - 1) % cols; // Column index
        grid[x][y] = true;
    }

    // Displays the grid with cell numbers for interactive initialization
    void display_matrix_with_numbers() {
        clear_terminal();
        cout << "Initial Grid (Numbered):\n";

        int maxNumber = rows * cols;                   // Maximum cell number
        int cellWidth = to_string(maxNumber).size() + 1; // Calculate width for alignment

        int cellNumber = 1;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << setw(cellWidth) << cellNumber++; // Print cell numbers aligned for clarity
            }
            cout << '\n';
        }
    }

    // Displays the current grid with alive ('*') and dead ('-') cells for a specific generation
    void display(int generation) {
        clear_terminal();
        cout << "Generation " << generation << ":\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << (grid[i][j] ? "*" : "-");
            }
            cout << '\n';
        }
    }

    // Allows the user to manually set specific cells as alive
    void interactive_initialization() {
        display_matrix_with_numbers();

        int cellNumber;
        do {
            cout << "Enter the cell number to make it alive (or -1 to finish): ";
            cin >> cellNumber;
            if(cin.fail()){
                cout<<"\nInvalid input please enter a valid number!!!!\n\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            clear_terminal(); // Clear after each cell input
            if (cellNumber == -1) break;
            set_cell_alive(cellNumber);
            display_matrix_with_numbers();
        } while (true);
    }

    // Saves the states of all generations to a file
    void save_to_file(const string &filename, const vector<vector<vector<bool>>> &generationsHistory) {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file for writing.\n";
            return;
        }

        // Iterate through each generation and save its grid state
        for (size_t gen = 0; gen < generationsHistory.size(); ++gen) {
            outFile << "Generation " << gen + 1 << ":\n";
            const auto &gridState = generationsHistory[gen];
            for (const auto &row : gridState) {
                for (bool cell : row) {
                    outFile << (cell ? "*" : "-");
                }
                outFile << '\n';
            }
            outFile << '\n'; // Separate generations for readability
        }

        outFile.close();
    }

    // Loads a grid pattern from a file
    void load_pattern_from_file(const string &filename) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file.\n";
            return;
        }

        inFile >> rows >> cols; // Read grid dimensions from the first line in the file
        grid.assign(rows, vector<bool>(cols, false)); // Resize grid based on file data

        string line;
        for (int i = 0; i < rows; ++i) {
            inFile >> line;
            for (int j = 0; j < cols && j < line.size(); ++j) {
                grid[i][j] = (line[j] == '*'); // Mark cells as alive based on file content
            }
        }
        inFile.close();
    }

    // Counts the number of alive neighbors for a specific cell
    int count_neighbors(int x, int y) {
        int count = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue; // Skip the cell itself
                int nx = x + i;
                int ny = y + j;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                    count += grid[nx][ny];
                }
            }
        }
        return count;
    }

    // Advances the grid to the next generation based on the game's rules
    void next_generation() {
        vector<vector<bool>> newGrid = grid; // Copy current grid to apply updates

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = count_neighbors(i, j); // Count alive neighbors

                // Apply the rules of the game
                if (grid[i][j]) {
                    // Current cell is alive
                    newGrid[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3);
                } else {
                    // Current cell is dead
                    newGrid[i][j] = (aliveNeighbors == 3);
                }
            }
        }
        grid = newGrid; // Update the grid with the new state
    }



    // Runs the simulation for a specified number of generations and records the history
    vector<vector<vector<bool>>> run(int generations, int delay = 200000) {
        vector<vector<vector<bool>>> generationsHistory; // Store all grid states
        for (int i = 0; i < generations; ++i) {
            display(i + 1); // Display current generation
            generationsHistory.push_back(grid); // Save the current grid state
            next_generation(); // Calculate the next generation
            Sleep(delay / 1000); // Convert microseconds to milliseconds
        }
        return generationsHistory;
    }
};


// **********>>> MENU <<<**********
int main() {
    // Welcome message for the Game of Life
    cout << "*****************>>> GAME OF LIFE <<<*****************\n\n";
    while(true) {

        int rows, cols, generations, choice;

        // Prompt the user for the number of rows, allowing only valid inputs
        while (true) {
            cout << "Enter the grid size (rows) [20 or 30 only]: ";
            cin >> rows;

            // Validate the input for rows
            if (cin.fail() || (rows != 20 && rows != 30)) {
                cin.clear(); // Reset error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                Universe::clear_terminal(); // Clear screen for better user experience
                cout << "Invalid input! Rows must be 20 or 30 only. Please try again.\n";
                continue; // Restart the loop for valid input
            }
            break; // Exit loop if input is valid
        }
        Universe::clear_terminal(); // Clear the screen after row input

        // Prompt the user for the number of columns, allowing only valid inputs
        while (true) {
            cout << "Enter the grid size (cols) [20, 30, or 50 only]: ";
            cin >> cols;

            // Validate the input for columns
            if (cin.fail() || (cols != 20 && cols != 30 && cols != 50)) {
                cin.clear(); // Reset error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                Universe::clear_terminal(); // Clear screen for better user experience
                cout << "Invalid input! Columns must be 20, 30, or 50 only. Please try again.\n";
                continue; // Restart the loop for valid input
            }
            break; // Exit loop if input is valid
        }
        Universe::clear_terminal(); // Clear the screen after column input

        // Create the game universe with the specified dimensions
        Universe game(rows, cols);

        bool validInput = false; // Flag to ensure valid initialization method selection

        // Prompt the user to choose an initialization method
        while (!validInput) {
            cout << "\nChoose an initialization method:\n";
            cout << "1. Random initialization with a percentage\n";
            cout << "2. Interactive initialization\n";
            cout << "3. Load pattern from file\n";
            cout << "==> Enter your choice: ";

            cin >> choice;
            Universe::clear_terminal(); // Clear the screen for a cleaner interface

            if (cin.fail()) { // Check for invalid input
                cin.clear(); // Reset error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cerr << "Invalid input. Please enter a number.\n";
                continue; // Restart the loop
            }

            // Handle the initialization method based on the user's choice
            if (choice == 1) {
                // Random initialization with a specified alive cell percentage
                double alivePercentage;
                cout << "Enter the alive cell percentage (e.g., 0.3 for 30%): ";
                cin >> alivePercentage;
                Universe::clear_terminal();

                if (cin.fail() || alivePercentage < 0 || alivePercentage > 1) {
                    cin.clear(); // Reset error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                    cerr << "Invalid percentage. Please enter a number between 0 and 1.\n";
                    continue; // Restart the loop
                }

                game.initialize(alivePercentage); // Initialize the grid randomly
                validInput = true; // Exit the loop
            } else if (choice == 2) {
                // Interactive initialization allowing the user to set cells alive
                game.interactive_initialization();
                validInput = true; // Exit the loop
            } else if (choice == 3) {
                // Load a predefined pattern from a file
                string filename;
                cout << "Enter the filename: ";
                cin >> filename;
                Universe::clear_terminal();

                game.load_pattern_from_file(filename); // Load the pattern
                validInput = true; // Exit the loop
            } else {
                // Handle invalid choice
                cerr << "**>> Invalid choice. Please select a valid option.\n";
            }
        }

        // Prompt the user for the number of generations to simulate
        cout << "Enter the number of generations to simulate: ";
        cin >> generations;
        Universe::clear_terminal();

        // Run the simulation and store the history of generations
        vector<vector<vector<bool>>> genHis = game.run(generations);

        // Ask the user if they want to save the simulation results
        string saveChoice;
        cout << "Do you want to save the current grid state? (y/yes for Yes, n/no for No): ";
        cin >> saveChoice;
        Universe::clear_terminal();

        // Convert the input to lowercase for consistency
        transform(saveChoice.begin(), saveChoice.end(), saveChoice.begin(), ::tolower);

        // If the user wants to save, prompt for a filename
        if (saveChoice == "y" || saveChoice == "yes") {
            string saveFilename;
            cout << "Enter the filename to save: ";
            cin >> saveFilename;
            Universe::clear_terminal();

            game.save_to_file(saveFilename, genHis); // Save all generations to the file
            cout << ">>> All generations have been saved to \"" << saveFilename << "\"\n\n";
        } else {
            // If the user chooses not to save
            cout << ">>>Grid not saved.\n\n";
        }
        while(true){
            cin.ignore();
            string choice2;
            cout<<"(1) Reset the game and restart new game\n(2) Exit the game!\n";
            cout<<"==> Enter your choice [1 or 2] :";
            cin>>choice2;
            if(choice2 == "1"){
                game.reset();
                Universe::clear_terminal();
                break;
            }else if (choice2 == "2"){
                cout<<"\n*************>>> Good Bye <<<**************"<<endl;
                return 0;
            }else{
                cout<<"Invalid choice, Please choose only [1 or 2]\n";
            }
        }
    }
}
