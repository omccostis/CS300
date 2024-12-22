//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Owen McCostis
// Version     : 1.0
// Description : CS 300 Project Two - ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
using namespace std;

// Course structure to hold all information about a single course
struct Course {
    string courseNumber; // Unique identifier for the course
    string name; // Full course name
    vector<string> prerequisites; // A list of prerequisite courseNumbers
};

// BSTNode structure to represent one node in the binary search tree
struct BSTNode {
    Course course; // Course data stored in this node
    BSTNode* left; // Pointer to the left child node
    BSTNode* right; // Pointer to the right child node

    // Constructor to create a node from a Course
    BSTNode(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// BST class to manage the binary search tree
class BST 
{
private:
    BSTNode* root; // Root node of the BST

    // Recursively inserts a Course into the tree
    // Returns the updated subtree root after insertion
    BSTNode* insertNode(BSTNode* node, Course c) {
        
        // If a null node is reached, place a new node
        if (node == nullptr) {
            return new BSTNode(c);
        }

        // Compare course numbers to maintain BST order
        if (c.courseNumber < node->course.courseNumber) {
            
            // Insert into the left subtree
            node->left = insertNode(node->left, c);
        }
        else if (c.courseNumber > node->course.courseNumber) {
            
            // Insert into the right subtree
            node->right = insertNode(node->right, c);
        }
        
        // If equal, we do nothing (assuming no duplicates)
        return node;
    }

    // Recursively searches the BST for a given courseNumber
    // Returns the node if found, or nullptr if not found
    BSTNode* searchNode(BSTNode* node, const string& courseNumber) {
        
        // If node is null, the course is not found
        if (node == nullptr) {
            return nullptr;
        }
        
        // If the current node's courseNumber matches, the course has been found
        if (courseNumber == node->course.courseNumber) {
            return node;
        }

        // If the search number is less, go left
        else if (courseNumber < node->course.courseNumber) {
            return searchNode(node->left, courseNumber);
        }

        // If the search number is more, go right
        else {
            return searchNode(node->right, courseNumber);
        }
    }

    // Performs an in-order traversal of the tree to print all courses (visitng nodes in asc. order of courseNumber)
    void inOrder(BSTNode* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.name << "\n";
            inOrder(node->right);
        }
    }

public:
    
    // Constructor to create an empty BST
    BST() {
        root = nullptr;
    }

    // Insert a course into the BST
    void insertCourse(Course c) {
        root = insertNode(root, c);
    }

    // Search for a course by courseNumber and return a pointer to it if found
    Course* searchCourse(const string& courseNumber) {
        BSTNode* found = searchNode(root, courseNumber);
        if (found == nullptr) return nullptr;
        return &(found->course);
    }

    // Print all courses in sorted order (in-order traversal)
    void printAllCourses() {
        inOrder(root);
    }
};

// Forward declarations of functions used in main function
void loadDataIntoBST(BST& bst);
void printCourseList(BST& bst);
void printSingleCourse(BST& bst);

int main() {
    
    // Instantiate the binary search tree
    BST bst;
    bool dataLoaded = false; // Tracks if data has been loaded

    cout << "Welcome to the course planner.\n\n";

    int choice = 0; // User menu choice
    do { // Main menu loop
        
        // Print the menu options
        cout << "1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n\n";
        cout << "What would you like to do? ";
        
        cin >> choice; // Read user choice

        // Handle failed input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please try again.\n\n";
            continue;
        }

        // Handle each menu option
        switch (choice) {
        case 1:
            
            // Load the course data from a file into the tree
            loadDataIntoBST(bst);
            dataLoaded = true;
            break;

        case 2:
            
            // Print all courses only if data has been loaded first
            if (!dataLoaded) {
                cout << "\nPlease load data first.\n\n";
            }
            else {
                printCourseList(bst);
            }
            break;

        case 3:
            
            // Print info about a single course only if data has been loaded first
            if (!dataLoaded) {
                cout << "\nPlease load data first.\n\n";
            }
            else {
                printSingleCourse(bst);
            }
            break;

        case 9:
            
            // Exit program
            cout << "Thank you for using the course planner!\n";
            break;

        default:
            
            // Invalid menu choice
            cout << choice << " is not a valid option.\n\n";
            break;
        }

    } while (choice != 9); // Loop menu until user chooses option 9

    return 0; // Exit the program
}

/*
    loadDataIntoBST:
    - Prompts the user for filename
    - Opens the file and reads line by line
    - Each line corresponds to a course and possibly its prerequisites
    - Builds course objects and inserts them into the BST
*/
void loadDataIntoBST(BST& bst) {
    cout << "\nEnter the file name to load the course data: ";
    string filename;
    cin >> filename; // Read filename from input

    ifstream file(filename);
    if (!file.is_open()) {
        
        // If file cannot be opened, print an error message
        cout << "Error: Cannot open file " << filename << "\n\n";
        return;
    }

    string line;
    
    // Read file line by line until the end is reached
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip line if empty

        // Split the line by commas
        vector<string> tokens;
        {
            
            // Manual parsing of CSV line
            size_t start = 0;
            size_t pos = line.find(',');
            while (pos != string::npos) {
                tokens.push_back(line.substr(start, pos - start));
                start = pos + 1;
                pos = line.find(',', start);
            }
            
            // Add the last token after the last comma
            tokens.push_back(line.substr(start));
        }

        // Validate there is at least a course number and a name
        if (tokens.size() < 2) {
            cout << "Warning: Invalid line format, skipping line: " << line << "\n";
            continue;
        }

        // Create Course object
        Course c;
        c.courseNumber = tokens[0];
        c.name = tokens[1];

        // If there are prerequisites, add them to the course's prerequisites list
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) { // Skip empty tokens
                c.prerequisites.push_back(tokens[i]);
            }
        }

        // Insert the course into the BST
        bst.insertCourse(c);
    }

    file.close(); // Close the file after loading
    cout << "\nData loaded successfully.\n\n";
}

/*
    printCourseList:
    - Prints all courses sorted in alphanumeric order by course number
    - Uses BST's in-order traversal
*/
void printCourseList(BST& bst) {
    cout << "\nHere is a sample schedule:\n";
    bst.printAllCourses(); // Print sorted course list
    cout << "\n";
}

/*
    printSingleCourse:
    - Prompts the user for a course number
    - Coverts it to uppercase to ensure case-insensitive matching
    - Searches the BST for the course
    - If found, prints course title and prerequisites
    - If not found, prints error message
*/
void printSingleCourse(BST& bst) {
    cout << "\nWhat course do you want to know about? ";
    string userInput;
    cin >> userInput;

    // Convert userInput to uppercase for matching
    for (auto& ch : userInput) {
        ch = toupper(static_cast<unsigned char>(ch));
    }

    // Search for the course in the BST
    Course* found = bst.searchCourse(userInput);
    if (found == nullptr) {
        cout << "\nCourse not found.\n\n";
        return;
    }

    // Print course's number and name
    cout << "\n" << found->courseNumber << ", " << found->name << "\n";
    
    // If the course does not have prerequisites
    if (found->prerequisites.empty()) {
        cout << "Prerequisites: None\n\n"; // Handle courses with no prerequisites
    }
    
    // If the course has prerequisites
    else {
        cout << "Prerequisites: ";
        
        // Loop through course prerequisites, printing each one
        for (size_t i = 0; i < found->prerequisites.size(); ++i) {
            cout << found->prerequisites[i];
            if (i < found->prerequisites.size() - 1) cout << ", ";
        }
        cout << "\n\n";
    }
}

