#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<limits>
#include<sstream>

using namespace std;

string fbook{}, fborrowed{}, fmembers{};
vector<string> Books{};
vector<string> Borrowed_books{};
vector<string> members_who_borrowed{};
vector<string> split_csv(const string& s,char delimiter );


int search_record(string filename, string searching_term);
void swap_records(string filename1, string filename2, string searching_term);
void delete_record(string filename, string searching_term);
void delete_by_line_number(string filename, int line_to_delete);
void manage_file(string filename);
void add();
void remove_book();

class Book {
    public:
        string book{};
        int book_code{};
        int book_copies{};
        bool is_available{};
};

class Member {
    public:
        string member_name{};
        int member_id{};
        bool book_borrowed{};
};

class Taken {
    public:
        string taken_book{};
        int taken_book_code{};
        string member_who_took{};
        int member_who_took_Id{};
};



vector<string> split_csv(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
void display_file(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << "\n";
        return;
    }
    
    string line;
    int line_num = 1;
    cout << "\n--- File Contents ---\n";
    while (getline(file, line)) {
        cout << line_num << ". " << line << "\n";
        line_num++;
    }
    cout << "\n End of file\n";
    file.close();
}

void search_book() {
    string searching_term;
    cout << "\nEnter Book name: ";
    cin.ignore();
    getline(cin, searching_term);
    search_record(fbook, searching_term);
}

int search_record(string filename, string searching_term) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << "\n";
        return -1;
    }
    
    string line;
    int line_num = 0;
    while (getline(file, line)) {
        line_num++;
        if (line.find(searching_term) != string::npos) {
            cout << "\nRecord found at line " << line_num << ": " << line << "\n";
            file.close();
            return line_num;
        }
    }
    
    cout << "\nRecord not found!\n";
    file.close();
    return -1;
}

void delete_record(string filename, string searching_term) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << "\n";
        return;
    }
    
    vector<string> lines;
    string line;
    bool found = false;
    
    while (getline(file, line)) {
        if (line.find(searching_term) != string::npos) {
            cout << "\nDeleting record: " << line << "\n";
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    file.close();
    
    if (!found) {
        cout << "Record not found. Nothing deleted.\n";
        return;
    }
    
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cout << "Error opening file for writing: " << filename << "\n";
        return;
    }
    
    for (string& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
    
    cout << "Record deleted successfully!\n";
}

void delete_by_line_number(string filename, int line_to_delete) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << "\n";
        return;
    }
    
    vector<string> lines;
    string line;
    int line_num = 0;
    
    while (getline(file, line)) {
        line_num++;
        if (line_num != line_to_delete) {
            lines.push_back(line);
        } else {
            cout << "Deleting line " << line_num << ": " << line << "\n";
        }
    }
    file.close();
    
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cout << "Error opening file for writing: " << filename << "\n";
        return;
    }
    
    for (string& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
    
    cout << "Line deleted successfully!\n";
}

void manage_file(string filename) {
    int choice;
    string searching_term;
    int line_num;
    
    while (true) {
        cout << "\nFile Management Menu \n";
        cout << "1. Display all records\n";
        cout << "2. Search for a record\n";
        cout << "3. Delete record by content\n";
        cout << "4. Delete record by line number\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                display_file(filename);
                break;
                
            case 2:
                cout << "Enter search term: ";
                getline(cin, searching_term);
                search_record(filename, searching_term);
                break;
                
            case 3:
                cout << "Enter data to delete: ";
                getline(cin, searching_term);
                delete_record(filename, searching_term);
                break;
                
            case 4:
                display_file(filename);
                cout << "Enter line number to delete: ";
                cin >> line_num;
                delete_by_line_number(filename, line_num);
                break;
                
            case 5:
                cout << "Exiting...\n";
                return;
                
            default:
                cout << "Invalid choice!\n";
        }
    }
}

void create_single_file(string filename) {
    string line;
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << "\n";
        return;
    }
    cout << "Input data: ";
    getline(cin, line);
    file << line << "\n";
    file.close();
}

void file_creation() {
    cout << "Enter Book file name: ";
    cin >> fbook;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    create_single_file(fbook);
    
    cout << "\nEnter Borrowed file name: ";
    cin >> fborrowed;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    create_single_file(fborrowed);
    
    cout << "\nEnter Members file name: ";
    cin >> fmembers;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    create_single_file(fmembers);
    
    cout << "\nFiles created Successfully.\n";
}



void add() {
    if (fbook.empty()) {
        cout << "Error: Book file not initialized. Please create files first.\n";
        return;
    }
    
    string line;
    ofstream file(fbook, ios::app);  
    if (!file.is_open()) {
        cout << "Error opening file\n";
        return;
    }
    
    cout << "\nInput Data:\n 1. Book name\n 2. Book code\n 3. Book copies\n 4. Book available\n";
    cin.ignore();
    getline(cin, line);
    file << line << "\n";
    file.close();
    cout << "Book added successfully!\n";
}

void remove_book() {
    if (fbook.empty()) {
        cout << "Error: Book file not initialized. Please create files first.\n";
        return;
    }
    manage_file(fbook);
}

void add_remove() {
    int x;
    cout << "\nEnter your choice \n 1. Add \n 2. Remove: ";
    cin >> x;
    cin.ignore();
    
    switch(x) {
        case 1:
            add();
            break;
        case 2:
            remove_book();
            break;
        default:
            cout << "\nInvalid choice\n";
    }
}

void lend() {
    if (fbook.empty() || fborrowed.empty()) {
        cout << "Error: Files do not exist. Please create files first.\n";
        return;
    }
    
    string searching_term;
    cout << "\nEnter The Book Name:\n";
    getline(cin, searching_term);
    
    string student_name;
    cout << "Enter Student Name borrowing the book: ";
    getline(cin, student_name);
    
  ifstream file(fbook);
    if (!file.is_open()) { cout << "Error opening book file.\n"; return; }
    
    vector<string> lines;
    string line;
    bool found = false;
    string book_code = "Unknown";
    int total_copies = 0;
    int available_copies = 0;
    
    while (getline(file, line)) {
        vector<string> data = split_csv(line, ',');
        
      
        if (data.size() >= 4 && data[0] == searching_term) {
            found = true;
            book_code = data[1];
            total_copies = stoi(data[2]);
            available_copies = stoi(data[3]);
            
            if (available_copies > 0) {
                available_copies--; 
                cout << "\nSuccess: 1 copy of '" << searching_term << "' lent.\n";
                cout << "Remaining copies: " << available_copies << "\n";
            } else {
                cout << "\nSorry, all copies of '" << searching_term << "' are currently borrowed.\n";
                file.close();
                return; 
            }
            
            
            line = data[0] + "," + data[1] + "," + to_string(total_copies) + "," + to_string(available_copies);
        }
        lines.push_back(line);
    }
    file.close();
    
    if (!found) {
        cout << "\nBook not found in the system.\n";
        return;
    }
    
    
    ofstream outfile(fbook);
    for (string& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
    
    
    ofstream borrow_out(fborrowed, ios::app);
    if (borrow_out.is_open()) {
        
        borrow_out << searching_term << "," << book_code << "," << student_name << ",2026-08-31\n";
        borrow_out.close();
        cout << "Transaction recorded successfully!\n";
    }
}


void ret() {
    if (fbook.empty() || fborrowed.empty()) {
        cout << "Error: Files not initialized. Please create files first.\n";
        return;
    }
    
    string searching_term;
    cout << "\nEnter The Book Name to return:\n";
    
    getline(cin, searching_term);
    
    
    ifstream file(fbook);
    if (!file.is_open()) { cout << "Error opening book file.\n"; return; }
    
    vector<string> lines;
    string line;
    bool found = false;
    string book_code = "Unknown";
    int total_copies = 0;
    int available_copies = 0;
    
    while (getline(file, line)) {
        vector<string> data = split_csv(line, ',');
        
        if (data.size() >= 4 && data[0] == searching_term) {
            found = true;
            book_code = data[1];
            total_copies = stoi(data[2]);
            available_copies = stoi(data[3]);
            
            available_copies++; 
            if (available_copies > total_copies) available_copies = total_copies; 
            
            cout << "\nSuccess: 1 copy of '" << searching_term << "' returned.\n";
            cout << "Total available copies now: " << available_copies << "\n";
            
           
            line = data[0] + "," + data[1] + "," + to_string(total_copies) + "," + to_string(available_copies);
        }
        lines.push_back(line);
    }
    file.close();
    
    if (!found) {
        cout << "\nBook not found in the system.\n";
        return;
    }
    
    
    ofstream outfile(fbook);
    for (string& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
    
  
    ofstream borrow_out(fborrowed, ios::app);
    if (borrow_out.is_open()) {
        borrow_out << searching_term << "," << book_code << ",Returned \n";
        borrow_out.close();
        cout << "Returned Book recorded successfully!\n";
    }
}

int main() {
    int x{1};
    
    
    cout << "### Library Management System ###\n";
    cout << "Enter Book file name: ";
    cin >> fbook;
    
    cout << "Enter Borrowed file name: ";
    cin >> fborrowed;
    
    cout << "Enter Members file name: ";
    cin >> fmembers;
    cin.ignore();
    
    
    ofstream temp_book(fbook, ios::app);
    ofstream temp_borrow(fborrowed, ios::app);
    ofstream temp_members(fmembers, ios::app);
    temp_book.close();
    temp_borrow.close();
    temp_members.close();
    
    cout << "\nFiles created  successfully!\n";
    
    while (x != 9) {
        system("cls");
        cout << "\n### List of actions ###";
        cout << "\n 1. Lend Book"
             << "\n 2. Return Book"
             << "\n 3. List of remaining Books"
             << "\n 4. List of Borrowed Books"
             << "\n 5. Add/Remove Book"
             << "\n 6. Borrowers' List with names"
             << "\n 7. Manage Book File"
             << "\n 8. Search record"
             << "\n 9. Exit";
                cout << "\nEnter your choice: ";
        if (!(cin >> x)) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input! Please enter a number.\n";
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        
        switch(x) {
            case 1:
                lend();
                break;
            case 2:
                ret();
                break;
            case 3:
                display_file(fbook);
                break;
            case 4:
                display_file(fborrowed);
                break;
            case 5:
                add_remove();
                break;
            case 6:
                display_file(fmembers);
                break;
            case 7:
                remove_book();
                break;
            case 8:
                search_book();
                break;
            case 9:
                cout << "Thank you for using Library Management System!\n";
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid Input!!!\n";
                cout << "Enter the number ahead of the action you want to take.\n";
        }
    }
    
    return 0;
