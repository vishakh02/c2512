#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>

using namespace std;

class Patient {
private:
    string PatientId;
    int PatientAge;

public:
    Patient(string id, int age) : PatientId(id), PatientAge(age) {}

    int getPatientAge() const {
        return PatientAge;
    }
};

int main() {
    // Create Patient records
    vector<Patient> patients = {
        Patient("101", 1),
        Patient("102", 2),
        Patient("103", 3),
        Patient("104", 4),
        Patient("105", 5)
    };

    int ages[patients.size()] = {0};

    for (int i = 0; i < patients.size(); i++) {
        ages[i] = patients[i].getPatientAge();
    }

    int numOfElements = patients.size();

    // Named pipes (FIFOs)
    const char* pipe1 = "pipe1";
    const char* pipe2 = "pipe2";

    // Open named pipes
    int pipe_one_write_fd = open(pipe1, O_WRONLY);
    int pipe_two_read_fd = open(pipe2, O_RDONLY);

    if (pipe_one_write_fd == -1) {
        perror("pipe1 open failure at client");
        return 1;
    }
    if (pipe_two_read_fd == -1) {
        perror("pipe2 open failure at client");
        return 1;
    }

    // Write data to server
    write(pipe_one_write_fd, &numOfElements, sizeof(int));
    cout << "Client sent number of elements: " << numOfElements << endl;

    write(pipe_one_write_fd, ages, sizeof(int) * numOfElements);
    cout << "Client sent ages: ";
    for (int age : ages) {
        cout << age << " ";
    }
    cout << endl;
    close(pipe_one_write_fd);

    // Read sum result from server
    int sum;
    read(pipe_two_read_fd, &sum, sizeof(int));
    cout << "Client received sum from server: " << sum << endl;

    close(pipe_two_read_fd);

    return 0;
}

