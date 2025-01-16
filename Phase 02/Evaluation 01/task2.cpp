#include <iostream>
#include <vector>
#include <climits>
#include <thread>

using namespace std;

class Patient {
private:
    string PatientId;
    int PatientAge;

public:
    Patient(string id, int age) : PatientId(id), PatientAge(age) {}

    string getPatientId() const {
        return PatientId;
    }

    int getPatientAge() const {
        return PatientAge;
    }
};

int findSum(const vector<Patient>& patients) {
    int sum = 0;
    for (const auto& each : patients) {
        sum += each.getPatientAge();
    }
    return sum;
}

void findMinInFirstHalf(const vector<Patient>& patients, int& min) {
    min = INT_MAX;
    for (int i = 0; i < patients.size() / 2; ++i) {
        if (patients[i].getPatientAge() < min) {
            min = patients[i].getPatientAge();
        }
    }
}

void findMaxInSecondHalf(const vector<Patient>& patients, int& max) {
    max = INT_MIN;
    for (int i = ((patients.size() / 2) + 1); i < patients.size(); ++i) {
        if (patients[i].getPatientAge() > max) {
            max = patients[i].getPatientAge();
        }
    }
}

int main() {
    vector<Patient> patients = {
        Patient("101", 1),
        Patient("102", 2),
        Patient("103", 3),
        Patient("104", 4),
        Patient("105", 5)
    };

    int min;
    int max;

    thread thSum (findSum, ref(patients));
    thread thMin (findMinInFirstHalf, ref(patients), ref(min));
    thread thMax (findMaxInSecondHalf, ref(patients), ref(max));

    thSum.join();
    thMin.join();
    thMax.join();

    int sum = findSum(patients);
    cout << "The sum is " << sum << endl;

    findMinInFirstHalf(patients, min);
    cout << "The min in first half is " << min << endl;

    findMaxInSecondHalf(patients, max);
    cout << "The max in second half is " << max << endl;

    return 0;
}
