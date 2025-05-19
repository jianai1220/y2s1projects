#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;
//struct to define book
struct Book {
    int id;       //Book ID
    string title; // Book title
    string author;//Author name
    Book* left;   // Point to the left child in binary tree
    Book* right;  //Point to the right child in binary tree
};
//class library to manage the library functions
class Library {
public:
    Library();   //constructor to initialize library
    ~Library();  //destructor
    void RetrieveBook(string t, ofstream& output);//Function to search a book that was in the library
    void InsertBook(int i, string t, string a, ofstream& output);//Function to add book in the library
    void DeleteBook(int i, ofstream& output);//Function to delete book in the library
    void SaveLibrary(const string& name) const;//function to save current library data to txt file
    void LoadLibrary(const string& name);//Function to load library data from txt file
    void DisplayLibrary(ofstream& result) const; //Function to display entire library

private:
    Book* root;//root of the binary search tree

    void Destroy(Book*& tree);//function to destroy the entire tree
    void Insert(Book*& tree, int i, string t, string a);//Function to insert a book in tree
    Book* Retrieve(Book*& tree, string t);//function to retrieve a book by title
    Book* Delete(Book*& tree, int i, bool& status);//function to delete a book by book ID
    void SaveLibrary(Book* tree, ofstream& name) const;//function save the tree to txt file
    void DisplayLibrary(Book* tree, ofstream& result) const;//function to display the tree
    string Convert(const string& s) const;//function to convert string to lowercase
};

//Default Operator
Library::Library() {
    root = nullptr;
}

//Default destructor
Library::~Library() {
    Destroy(root); //Calling the destroy function to delete the whole tree
}

//Destroy function to delete the tree
void Library::Destroy(Book*& tree) {
    if (tree != nullptr) { //if the node is not empty
        Destroy(tree->left); //destroy the left node
        Destroy(tree->right); //destroy the right node
        delete tree;
        tree = nullptr; //the node is deleted and empty now
    }
}

//Function to insert the book into library
void Library::InsertBook(int i, string t, string a, ofstream& output) {
    Insert(root, i, t, a); //insert it into the bst by insert into a node first
    output << "Book with ID: " << i
           << "\nTitle: " << t
           << "\nAuthor: " << a
           << "\nhas successfully been inserted.\n";
    cout << "Book has been inserted.\n";
}

void Library::Insert(Book*& tree, int i, string t, string a) {
    if (tree == nullptr) { //if the node is empty
        tree = new Book;
        tree->id = i;
        tree->title = t;
        tree->author = a;
        tree->left = nullptr;
        tree->right = nullptr;
    } else if (t < tree->title) { //if the node title is less than the current node's title
        Insert(tree->left, i, t, a); //insert to the left node
    } else {
        Insert(tree->right, i, t, a); //or else insert to the right node
    }
}

//convert the string into lower case to ease searching
string Library::Convert(const string& s) const {
    string result = s;
    result.erase(0, result.find_first_not_of(' ')); //erase the first character in string if it is an empty space
    result.erase(result.find_last_not_of(' ') + 1); //erase the last character in string if it is an empty space
    for (size_t i = 0; i < result.length(); i++) { //a loop to convert all the characters in the string into lowercase
        result[i] = tolower(result[i]);
    }
    return result;
}

//Function to search a book that was in the library
void Library::RetrieveBook(string t, ofstream& output) {
    t = Convert(t); //convert the given title into lower case first
    Book* result = Retrieve(root, t); //search through the nodes with the converted title
    if (result) { //if the book was found
        output << "\nBook with ID: " << result->id
               << "\nTitle: " << result->title
               << "\nAuthor: " << result->author
               << "\nhas been found.\n";
        cout << "Book has been found.\n";
    } else { //if the book was not found
        output << "Book with title \"" << t << "\" not found."<<endl;
        cout << "Book not found."<<endl;
    }
}

Book* Library::Retrieve(Book*& tree, string t) {
    if (!tree) { // If the node is empty
        return nullptr;
    }
    string currTitle = Convert(tree->title); // Convert the node's book title to lowercase

    if (t == currTitle) { // If the given title matches the current node's book title
        return tree; // Return current node
    } else if(t < currTitle) { // If the title is less than the current title 
        return Retrieve(tree->left, t); // Recursively search in the left subtree
    } else {
        return Retrieve(tree->right, t); // Recursively search in the right subtree
    }
}

//Function to delete a book from the library
void Library::DeleteBook(int i, ofstream& output) {
    bool status = false; //a boolean status to check if the book was deleted or not
    root = Delete(root, i, status); //delete a book with the given book id
    if (status) { //if the status is true (that the book is deleted)
        output << "Book with ID: " << i << " has been deleted."<<endl;
        cout << "Book has been deleted."<<endl;
    } else { //if the book is not deleted, as the id is not found
        output << "Book with ID " << i << " not found."<<endl;
        cout << "Book not found."<<endl;
    }
}

Book* Library::Delete(Book*& tree, int i, bool& status) {
    if (!tree) { //if the node is empty
        return nullptr;
    }

    if (i < tree->id) { //if the given id is less than the current node's id
        tree->left = Delete(tree->left, i, status); //recursively search for it to be deleted from the left subtree
    } else if (i > tree->id) { //if the given id is greter than the current node's id
        tree->right = Delete(tree->right, i, status);//recursively search for it to be deleted from the right subtree
    } else { //if the given id matches the current node's id
        status = true; //the status of deletion is true
        if (!tree->left) { //if the left node is null
            Book* temp = tree->right; //store the right node in temp
            delete tree; //delete current node
            return temp;
        } else if (!tree->right) { //if the right node is null
            Book* temp = tree->left; //store the left node in temp
            delete tree; //delete current node
            return temp;
        }

        //if the node has two children, get the smallest in the right subtree
        Book* temp = tree->right;
        while (temp && temp->left) 
            temp = temp->left; //try to find the leftmost node in the right subtree

        //copy up the content into temp
        tree->id = temp->id;
        tree->title = temp->title;
        tree->author = temp->author;
        tree->right = Delete(tree->right, temp->id, status); //delete the inorder successor
    }
    return tree;
}

//Function to save the library's current data
void Library::SaveLibrary(const string& name) const {
    ofstream file(name); //create an output file and open the file with given file name 
    if (file.is_open()) { //if the file is open
        SaveLibrary(root, file); //save the current library into the file
        file.close(); //close the file
    } else {
        cerr << "Failed to open file for saving: " << name << endl; //error handling operation if the file cannot be open for saving
    }
}

void Library::SaveLibrary(Book* tree, ofstream& name) const {
    if (!tree) //if the current node is empty
    return; 
    SaveLibrary(tree->left, name); //save the left subtree recursively 
    name << tree->id << "," << tree->title << "," << tree->author << endl; //save the data into the file
    SaveLibrary(tree->right, name); //save the right subtree recursively
}

//Function to load the library
void Library::LoadLibrary(const string& name) {
    ifstream file(name); //create an input file and open the file with given file name
    if (file.is_open()) { //if the file is open
        string line; 
        while (getline(file, line)) { //read all the lines in the file
            stringstream ss(line); //create a string stream from current line
            string idStr, title, author;

            getline(ss, idStr, ','); //extract book id as a string from the line until the comma
            getline(ss, title, ','); //extract book title from the line until the comma
            getline(ss, author); //extract book author from the line
            int id = stoi(idStr); //convert the book id from string to integer
            Insert(root, id, title, author); //insert the book with its data into the library
        }
        file.close(); //close the file
    } else {
        cerr << "Failed to open file for loading: " << name << endl; //error handling operation if the file cannot be open for loading
    }
}

//Display the books in the library in a table
void Library::DisplayLibrary(ofstream& result) const {
    result << "+----------+----------------------------------------------------+----------------------------------------------------+\n";
    result << "|   ID     |                      Title                         |                      Author                        |\n";
    result << "+----------+----------------------------------------------------+----------------------------------------------------+\n";

    DisplayLibrary(root, result); //display all the books in the library with in-order traversal

    result << "+----------+----------------------------------------------------+----------------------------------------------------+\n";
}

void Library::DisplayLibrary(Book* tree, ofstream& result) const {
    if (!tree) //if the current node is null
        return;
    DisplayLibrary(tree->left, result); //display the left subtree recursively
    result << "| " << setw(8) << tree->id << " | "
           << setw(50) << tree->title << " | "
           << setw(50) << tree->author << " |\n";
    DisplayLibrary(tree->right, result); //display the right subtree recursively
}

//The main menu of the program for selecting the function to use
void mainMenu(Library& library, const string& inputFile) {
    while (true) { //loop until the program is called to be terminated
        cout << "\nLibrary Management System\n";
        cout << "1. View Library\n";
        cout << "2. Add Book\n";
        cout << "3. Delete Book\n";
        cout << "4. Search for a Book\n";
        cout << "5. Exit\n";
        cout << "Choose an option: ";

        int choice; 
        cin >> choice; //get the choice as an integer
        cin.ignore(); //ignore any newline character

        ofstream output("output.txt", ios::app); //open an output file to copy all the results

        switch (choice) {
        case 1: {
            library.DisplayLibrary(output); //display the entire library into the output file
            cout << "Library displayed.\n";
            break;
        }

        case 2: {
            int id;
            string title, author;

            cout << "Enter book ID: ";
            cin >> id; //get the book id to insert
            cin.ignore(); //ignore any newline character
            cout << "Enter book title: ";
            getline(cin, title); //get the book title to insert
            cout << "Enter book author: ";
            getline(cin, author); //get the book author to insert

            library.InsertBook(id, title, author, output); //insert the new book into the library
            library.SaveLibrary(inputFile); //save the modified library back to the input file
            break;
        }

        case 3: {
            int i;
            cout << "Enter book ID to delete: ";
            cin >> i; //get the book id to delete

            if (cin.fail()) { //to check if the input is not an integer
                cin.clear(); //clear the error flag
                string invalid;
                cin>>invalid; //consume the invalid input
                cout<<"Invalid input. Please enter an integer."<<endl;
                break;
            }

            else{
                library.DeleteBook(i, output); //delete the book from the library
                library.SaveLibrary(inputFile); //save the modified library back to the input file
                break;
            }
        }

        case 4: {
            string t;
            cout << "Enter book title to search: ";
            getline(cin, t); //get the book title to search

            library.RetrieveBook(t, output); //Search for the book in the library
            break;
        }

        case 5:
            cout << "Exiting program. Goodbye!\n";
            output.close(); //close the output file
            return; //exit the loop and stop the program

        default:
            cout << "Invalid option. Please try again.\n"; //error handling for any invalid options
        }

        output.close(); //close the output file
    }
}

int main() {
    string inputFile = "books.txt"; //the input file that stores the library data
    Library library; //create an instance of the library class
    library.LoadLibrary(inputFile); //load the library data from file
    mainMenu(library, inputFile); //start the main menu
    return 0;
}
