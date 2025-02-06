#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Record {
    int id;
    string name;
    int age;
};

class MiniSQL {
private:
    vector<Record> records;
    unordered_map<int, size_t> index;
    string filename;

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Record rec;
            ss >> rec.id >> rec.name >> rec.age;
            records.push_back(rec);
            index[rec.id] = records.size() - 1;
        }
    }

    void saveToFile() {
        ofstream file(filename);
        for (const auto &rec : records) {
            file << rec.id << " " << rec.name << " " << rec.age << endl;
        }
    }

public:
    MiniSQL(const string &file) : filename(file) {
        loadFromFile();
    }

    void insert(int id, const string &name, int age) {
        if (index.find(id) != index.end()) {
            cout << "Record with ID " << id << " already exists." << endl;
            return;
        }
        records.push_back({id, name, age});
        index[id] = records.size() - 1;
        saveToFile();
    }

    void select(int id) {
        if (index.find(id) != index.end()) {
            auto rec = records[index[id]];
            cout << "ID: " << rec.id << ", Name: " << rec.name << ", Age: " << rec.age << endl;
        } else {
            cout << "Record not found." << endl;
        }
    }

    void update(int id, const string &name, int age) {
        if (index.find(id) != index.end()) {
            records[index[id]] = {id, name, age};
            saveToFile();
        } else {
            cout << "Record not found." << endl;
        }
    }

    void remove(int id) {
        if (index.find(id) != index.end()) {
            records.erase(records.begin() + index[id]);
            index.erase(id);
            saveToFile();
        } else {
            cout << "Record not found." << endl;
        }
    }
};

int main() {
    MiniSQL db("database.txt");
    
    while (true) {
        cout << "\nChoose an operation: insert, select, update, remove, exit\n> ";
        string command;
        cin >> command;
        
        if (command == "insert") {
            int id, age;
            string name;
            cout << "Enter ID, Name, Age: ";
            cin >> id >> name >> age;
            db.insert(id, name, age);
        } else if (command == "select") {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            db.select(id);
        } else if (command == "update") {
            int id, age;
            string name;
            cout << "Enter ID, New Name, New Age: ";
            cin >> id >> name >> age;
            db.update(id, name, age);
        } else if (command == "remove") {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            db.remove(id);
        } else if (command == "exit") {
            break;
        } else {
            cout << "Invalid command." << endl;
        }
    }
    
    return 0;
}
