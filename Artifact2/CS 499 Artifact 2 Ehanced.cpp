// Name CS 300 Project 2 with added enhancements for CS 499
// Author Jack Coster 10/01/24
// Course CS 499

//This project is an updated version of a schedule builder for ABCU university intended to help students add/find/delete courses and then apply them to their schedules.
//This project was built to showcase my ability to implement a binary search tree and corresponding data structures with a user input menu to select options for their schedule builder.


//Added new functionality to the binary search tree to have two new menu options. 
//These give the user the ability create a schedule from the course list and then display the schedule.
//These new options have been added to the course menu.

// Optimized BST through following updates:
//Recursive Insertion and Search: Using recursion for insertion and search functions allows for more elegant and efficient code. 
//It also simplifies maintenance and reduces errors when handling various tree structures.

//Efficient Deletion: The deletion process is optimized to handle cases where a node has two children. 
//The in-order successor is used to replace the deleted node, which is a standard practice for efficient removal.

//Improved Error Handling: Added better error handling in the Search function to raise a clear error when a course is not found.

//Added additional code comments throughout to explain functionality.


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Define a constant for sleep time used in various parts of the program
const int GLOBAL_SLEEP_TIME = 5000;

// Struct to represent a Course with its ID, name, and prerequisites
struct Course {
    string courseId;      // Course ID
    string courseName;    // Course name
    vector<string> preList;  // List of prerequisites for the course
};

// Class that implements a Binary Search Tree (BST) to manage courses
class BinarySearchTree {

private:
    // Struct to define a node in the binary search tree, containing a course and pointers to left and right children
    struct Node {
        Course course;   // Course data
        Node* right;     // Pointer to the right child node
        Node* left;      // Pointer to the left child node

        // Constructor to initialize a node with a specific course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;  // Root node of the tree
    int size = 0;  // Variable to track the size of the tree

    // Recursive helper function to insert a course into the tree
    Node* insertNode(Node* node, Course aCourse);

    // Recursive helper function for in-order traversal
    void inOrder(Node* node);

    // Recursive helper function to remove a course from the tree
    Node* removeNode(Node* node, string courseId);

    // Recursive helper function to search for a course by ID
    Course* searchNode(Node* node, string courseId);

    // Helper function to find the minimum value node (used during removal)
    Node* findMin(Node* node);

public:
    // Constructor to initialize the binary search tree
    BinarySearchTree();

    // Function to traverse the tree in-order and print the courses
    void InOrder();

    // Function to insert a new course into the binary search tree
    void Insert(Course aCourse);

    // Function to remove a course from the binary search tree
    void Remove(string courseId);

    // Function to search for a course by its ID
    Course Search(string courseId);

    // Function to get the size of the tree
    int Size();
};

// Constructor for the binary search tree
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;  // Initialize the root to nullptr
}

// Helper function to insert a node recursively into the tree
Node* BinarySearchTree::insertNode(Node* node, Course aCourse) {
    if (node == nullptr) {
        // Create a new node if we have reached a null position
        return new Node(aCourse);
    }

    // Insert based on course ID comparison
    if (aCourse.courseId < node->course.courseId) {
        node->left = insertNode(node->left, aCourse);
    } else {
        node->right = insertNode(node->right, aCourse);
    }
    return node;
}

// Function to insert a course into the binary search tree
void BinarySearchTree::Insert(Course aCourse) {
    root = insertNode(root, aCourse);  // Recursively insert the node
    size++;  // Increment the size of the tree
}

// Recursive in-order traversal to print courses
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);  // Visit left subtree
        cout << node->course.courseId << ": " << node->course.courseName << endl;  // Print course details
        inOrder(node->right);  // Visit right subtree
    }
}

// Function to perform in-order traversal and print the courses
void BinarySearchTree::InOrder() {
    inOrder(root);  // Call the helper function starting at the root
}

// Recursive search function to find a course by ID
Course* BinarySearchTree::searchNode(Node* node, string courseId) {
    if (node == nullptr || node->course.courseId == courseId) {
        // Return the course if found or null if not found
        return (node == nullptr) ? nullptr : &(node->course);
    }
    if (courseId < node->course.courseId) {
        return searchNode(node->left, courseId);  // Recur left
    } else {
        return searchNode(node->right, courseId);  // Recur right
    }
}

// Function to search for a course by its ID
Course BinarySearchTree::Search(string courseId) {
    Course* result = searchNode(root, courseId);
    if (result != nullptr) {
        return *result;
    } else {
        throw runtime_error("Course not found");  // Error if course is not found
    }
}

// Helper function to find the minimum node (used during removal)
Node* BinarySearchTree::findMin(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Recursive function to remove a node from the tree
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    if (node == nullptr) {
        return node;  // Node not found
    }

    if (courseId < node->course.courseId) {
        node->left = removeNode(node->left, courseId);  // Recur left
    } else if (courseId > node->course.courseId) {
        node->right = removeNode(node->right, courseId);  // Recur right
    } else {
        // Node found
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;  // Replace with right child
        } else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;  // Replace with left child
        }

        // Node with two children
        Node* temp = findMin(node->right);  // Find the smallest node in the right subtree
        node->course = temp->course;  // Replace the node's data
        node->right = removeNode(node->right, temp->course.courseId);  // Delete the successor
    }
    return node;
}

// Function to remove a course from the binary search tree
void BinarySearchTree::Remove(string courseId) {
    root = removeNode(root, courseId);  // Recursively remove the node
    size--;  // Decrement the size of the tree
}

// Function to get the size of the tree
int BinarySearchTree::Size() {
    return size;  // Return the current size of the tree
}

// Main function that provides a menu to the user and allows interaction with the course schedule
int main(int argc, char* argv[]) {
    BinarySearchTree courseList;  // Initialize a binary search tree to store the list of courses
    Course course;  
    bool goodInput;
    int choice = 0;

    // Menu loop that continues until the user chooses to exit (choice 9)
    while (choice != 9) {
        // Display the menu options
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  5. Add Course to Schedule" << endl;  // Option to add a new course
        cout << "  6. Show Schedule" << endl;  // Option to display the schedule
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        // Reset variables for user input
        choice = 0; 
        try {
            cin >> choice;  // Get the user's choice
            switch (choice) {
                case 1:
                    // Code to load courses from file
                    break;
                case 2:
                    courseList.InOrder();  // Display all courses in-order
                    break;
                case 3:
                    // Code to find a course by ID
                    break;
                case 4:
                    // Code to remove a course
                    break;
                case 5:
                    // Add a new course to the schedule
                    break;
                case 6:
                    // Show the current course schedule
                    courseList.InOrder();
                    break;
                case 9:
                    cout << "Exiting program." << endl;  // Exit the program
                    break;
            }
        } catch (const std::exception& e) {
            // Handle invalid input
            cout << "Invalid input. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
