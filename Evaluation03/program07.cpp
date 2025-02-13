#include <iostream>
#include <iomanip>
#include <deque>
#include <algorithm>
#include <string>

using namespace std;

// ==== PatientManager CLASS ====
class PatientManager {
private:
    deque<string> PatientIDs;   // Stores Patient IDs
    deque<int> Ages;            // Stores Patient Ages

public:
    int findIndexById(string& id); // Find the index of a patient by ID
    void create();                 // Add new patient
    void displayAll();             // Display all patients
    void editById();               // Edit patient details
    void deleteById();             // Delete a patient
    void findMinAge();             // Find patient with minimum age
    void findMaxAge();             // Find patient with maximum age
};

// ==== FUNCTION: Print Menu ====
void printMenu() {
    cout << "\n=== Patient Management Module ===\n";
    cout << "1. Create Patient\n";
    cout << "2. Display All Patients\n";
    cout << "3. Edit Patient\n";
    cout << "4. Delete Patient\n";
    cout << "5. Find Patient with Min Age\n";
    cout << "6. Find Patient with Max Age\n";
    cout << "7. Exit\n";
}

int main() {
    PatientManager manager;
    int choice;

    do {
        printMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: manager.create();       break;
            case 2: manager.displayAll();   break;
            case 3: manager.editById();     break;
            case 4: manager.deleteById();   break;
            case 5: manager.findMinAge();   break;
            case 6: manager.findMaxAge();   break;
            case 7: cout << "Exiting system. Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 7);

    return 0;
}

// ==== FUNCTION DEFINITIONS ====

// Create Patient
void PatientManager::create() {
    string id;
    int age;

    cout << "Enter Patient ID: ";
    cin >> id;

    // Check if ID is unique
    if (findIndexById(id) != -1) {
        cout << "Error: Patient ID already exists.\n";
        return;
    }

    cout << "Enter Patient Age: ";
    cin >> age;

    // Add to deque
    PatientIDs.push_back(id);
    Ages.push_back(age);

    cout << "Patient added successfully.\n";
}

// Display All Patients
void PatientManager::displayAll() {
    if (PatientIDs.empty()) {
        cout << "No patient details available.\n";
        return;
    }

    cout << "------------------------\n";
    cout << "| Patient ID | Age     |\n";
    cout << "------------------------\n";
    for (size_t i = 0; i < PatientIDs.size(); ++i) {
        cout << "| " << setw(10) << PatientIDs[i] << " | "
             << setw(7) << Ages[i] << " |\n";
    }
    cout << "------------------------\n";
}

// Find Index of Patient by ID
int PatientManager::findIndexById(string& id) {
    auto it = find(PatientIDs.begin(), PatientIDs.end(), id);
    if (it != PatientIDs.end()) {
        return distance(PatientIDs.begin(), it);
    }
    return -1;
}

// Edit Patient Details
void PatientManager::editById() {
    string id;
    cout << "Enter Patient ID to edit: ";
    cin >> id;

    int index = findIndexById(id);
    if (index == -1) {
        cout << "Error: Patient ID not found.\n";
        return;
    }

    cout << "Current Age: " << Ages[index] << "\n";
    cout << "Enter new Age: ";
    cin >> Ages[index];

    cout << "Patient updated successfully.\n";
}

// Delete Patient
void PatientManager::deleteById() {
    string id;
    cout << "Enter Patient ID to delete: ";
    cin >> id;

    int index = findIndexById(id);
    if (index == -1) {
        cout << "Error: Patient ID not found.\n";
        return;
    }

    PatientIDs.erase(PatientIDs.begin() + index);
    Ages.erase(Ages.begin() + index);

    cout << "Patient deleted successfully.\n";
}

// Find Patient with Minimum Age
void PatientManager::findMinAge() {
    if (Ages.empty()) {
        cout << "No patients available.\n";
        return;
    }

    auto minIt = min_element(Ages.begin(), Ages.end());
    int index = distance(Ages.begin(), minIt);
    cout << "Patient with Min Age: " << PatientIDs[index] << " (Age: " << Ages[index] << ")\n";
}

// Find Patient with Maximum Age
void PatientManager::findMaxAge() {
    if (Ages.empty()) {
        cout << "No patients available.\n";
        return;
    }

    auto maxIt = max_element(Ages.begin(), Ages.end());
    int index = distance(Ages.begin(), maxIt);
    cout << "Patient with Max Age: " << PatientIDs[index] << " (Age: " << Ages[index] << ")\n";
}
