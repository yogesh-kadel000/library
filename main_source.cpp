#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>
#include <cmath>

using namespace std;

// Book Structure
struct Book {
    string ISBN;
    string title;
    string author;
    string genre;
    int yearPublished;
    double price;
    int quantity;
    bool isAvailable;

    void displayBook() const {
        cout << "\n=== Book Details ===\n";
        cout << "ISBN: " << ISBN << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Genre: " << genre << endl;
        cout << "Year Published: " << yearPublished << endl;
        cout << "Price: $" << price << endl;
        cout << "Quantity Available: " << quantity << endl;
        cout << "Status: " << (isAvailable ? "Available" : "Not Available") << endl;
    }
};

// Library Management System
class Library {
private:
    vector<Book> books;
    string libraryName;

public:
    Library(string name) : libraryName(name) {}

    // Add a new book
    void addBook(const Book& book) {
        books.push_back(book);
        cout << "\n✓ Book '" << book.title << "' added successfully!" << endl;
    }

    // Remove a book by ISBN
    void removeBook(string ISBN) {
        auto it = find_if(books.begin(), books.end(),
                         [ISBN](const Book& b) { return b.ISBN == ISBN; });
        if (it != books.end()) {
            cout << "\n✓ Book '" << it->title << "' removed successfully!" << endl;
            books.erase(it);
        } else {
            cout << "\n✗ Book with ISBN " << ISBN << " not found!" << endl;
        }
    }

    // Search book by title
    void searchByTitle(string title) {
        bool found = false;
        for (const auto& book : books) {
            if (book.title.find(title) != string::npos) {
                book.displayBook();
                found = true;
            }
        }
        if (!found) cout << "\n✗ No books found with title containing '" << title << "'" << endl;
    }

    // Search book by author
    void searchByAuthor(string author) {
        bool found = false;
        for (const auto& book : books) {
            if (book.author.find(author) != string::npos) {
                book.displayBook();
                found = true;
            }
        }
        if (!found) cout << "\n✗ No books found by author '" << author << "'" << endl;
    }

    // Search book by ISBN
    void searchByISBN(string ISBN) {
        auto it = find_if(books.begin(), books.end(),
                         [ISBN](const Book& b) { return b.ISBN == ISBN; });
        if (it != books.end()) {
            it->displayBook();
        } else {
            cout << "\n✗ Book with ISBN " << ISBN << " not found!" << endl;
        }
    }

    // Display all books
    void displayAllBooks() const {
        if (books.empty()) {
            cout << "\n✗ No books in the library!" << endl;
            return;
        }
        cout << "\n=== Library Catalog ===\n";
        for (size_t i = 0; i < books.size(); i++) {
            cout << "\n[" << i + 1 << "] " << books[i].title << " by " << books[i].author << endl;
        }
    }

    // Borrow a book
    void borrowBook(string ISBN) {
        auto it = find_if(books.begin(), books.end(),
                         [ISBN](const Book& b) { return b.ISBN == ISBN; });
        if (it != books.end()) {
            if (it->quantity > 0) {
                it->quantity--;
                cout << "\n✓ Book '" << it->title << "' borrowed successfully!" << endl;
                if (it->quantity == 0) it->isAvailable = false;
            } else {
                cout << "\n✗ Book '" << it->title << "' is out of stock!" << endl;
            }
        } else {
            cout << "\n✗ Book with ISBN " << ISBN << " not found!" << endl;
        }
    }

    // Return a book
    void returnBook(string ISBN) {
        auto it = find_if(books.begin(), books.end(),
                         [ISBN](const Book& b) { return b.ISBN == ISBN; });
        if (it != books.end()) {
            it->quantity++;
            it->isAvailable = true;
            cout << "\n✓ Book '" << it->title << "' returned successfully!" << endl;
        } else {
            cout << "\n✗ Book with ISBN " << ISBN << " not found!" << endl;
        }
    }

    // Get library statistics
    void displayStatistics() const {
        cout << "\n=== Library Statistics ===\n";
        cout << "Library Name: " << libraryName << endl;
        cout << "Total Books: " << books.size() << endl;
        int totalQuantity = 0;
        for (const auto& book : books) totalQuantity += book.quantity;
        cout << "Total Copies Available: " << totalQuantity << endl;
    }

    // Save library to file
    void saveToFile(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "\n✗ Error opening file!" << endl;
            return;
        }
        for (const auto& book : books) {
            file << book.ISBN << "|" << book.title << "|" << book.author << "|"
                 << book.genre << "|" << book.yearPublished << "|" << book.price
                 << "|" << book.quantity << "|" << (book.isAvailable ? "1" : "0") << "\n";
        }
        file.close();
        cout << "\n✓ Library data saved to " << filename << endl;
    }

    // Load library from file
    void loadFromFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "\n✗ Error opening file!" << endl;
            return;
        }
        books.clear();
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string ISBN, title, author, genre, yearStr, priceStr, quantityStr, availStr;
            getline(ss, ISBN, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, genre, '|');
            getline(ss, yearStr, '|');
            getline(ss, priceStr, '|');
            getline(ss, quantityStr, '|');
            getline(ss, availStr, '|');

            Book book{ISBN, title, author, genre, stoi(yearStr), stod(priceStr),
                     stoi(quantityStr), stoi(availStr) == 1};
            books.push_back(book);
        }
        file.close();
        cout << "\n✓ Library data loaded from " << filename << endl;
    }
};

// Main Menu Function
void displayMenu() {
    cout << "\n========================================\n";
    cout << "       LIBRARY MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Add a new book\n";
    cout << "2. Remove a book\n";
    cout << "3. Search by title\n";
    cout << "4. Search by author\n";
    cout << "5. Search by ISBN\n";
    cout << "6. Display all books\n";
    cout << "7. Borrow a book\n";
    cout << "8. Return a book\n";
    cout << "9. Display statistics\n";
    cout << "10. Save to file\n";
    cout << "11. Load from file\n";
    cout << "0. Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

int main() {
    Library library("City Central Library");

    // Sample data
    library.addBook({"978-0134685991", "Effective Java", "Joshua Bloch", "Programming", 2018, 45.99, 5, true});
    library.addBook({"978-0201633610", "Design Patterns", "Gang of Four", "Programming", 1994, 54.99, 3, true});
    library.addBook({"978-1491954294", "Learning JavaScript", "Ethan Brown", "Programming", 2016, 39.99, 4, true});

    int choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                Book newBook;
                cout << "\nEnter ISBN: "; getline(cin, newBook.ISBN);
                cout << "Enter Title: "; getline(cin, newBook.title);
                cout << "Enter Author: "; getline(cin, newBook.author);
                cout << "Enter Genre: "; getline(cin, newBook.genre);
                cout << "Enter Year Published: "; cin >> newBook.yearPublished;
                cout << "Enter Price: "; cin >> newBook.price;
                cout << "Enter Quantity: "; cin >> newBook.quantity;
                newBook.isAvailable = (newBook.quantity > 0);
                cin.ignore();
                library.addBook(newBook);
                break;
            }
            case 2: {
                cout << "\nEnter ISBN to remove: ";
                string isbn;
                getline(cin, isbn);
                library.removeBook(isbn);
                break;
            }
            case 3: {
                cout << "\nEnter title to search: ";
                string title;
                getline(cin, title);
                library.searchByTitle(title);
                break;
            }
            case 4: {
                cout << "\nEnter author name: ";
                string author;
                getline(cin, author);
                library.searchByAuthor(author);
                break;
            }
            case 5: {
                cout << "\nEnter ISBN: ";
                string isbn;
                getline(cin, isbn);
                library.searchByISBN(isbn);
                break;
            }
            case 6: {
                library.displayAllBooks();
                break;
            }
            case 7: {
                cout << "\nEnter ISBN to borrow: ";
                string isbn;
                getline(cin, isbn);
                library.borrowBook(isbn);
                break;
            }
            case 8: {
                cout << "\nEnter ISBN to return: ";
                string isbn;
                getline(cin, isbn);
                library.returnBook(isbn);
                break;
            }
            case 9: {
                library.displayStatistics();
                break;
            }
            case 10: {
                library.saveToFile("library_data.txt");
                break;
            }
            case 11: {
                library.loadFromFile("library_data.txt");
                break;
            }
            case 0: {
                cout << "\n✓ Thank you for using Library Management System!" << endl;
                break;
            }
            default: {
                cout << "\n✗ Invalid choice! Please try again." << endl;
            }
        }
    } while (choice != 0);

    return 0;
}
