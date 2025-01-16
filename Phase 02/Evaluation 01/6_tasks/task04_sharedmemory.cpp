#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <vector>
#include <string>

#define SHM_KEY 1234

class Patient {
public:
    std::string PatientId;
    int PatientAge;

    Patient(std::string id, int age) : PatientId(id), PatientAge(age) {}

    std::string getPatientId() const { return PatientId; }
    int getPatientAge() const { return PatientAge; }
};

struct SharedMemory {
    int ages[5];
    int sum;
    bool dataReady;   // Indicates when the client has written data
    bool serverDone;  // Indicates when the server has processed data
};

void client(int& shmid) {
    SharedMemory* sharedMem = (SharedMemory*)shmat(shmid, nullptr, 0);

    // Create and populate the list of patients
    std::vector<Patient> patients = {
        Patient("P001", 25),
        Patient("P002", 30),
        Patient("P003", 35),
        Patient("P004", 40),
        Patient("P005", 20)
    };

    // Write patient ages to shared memory
    for (int i = 0; i < 5; ++i) {
        sharedMem->ages[i] = patients[i].getPatientAge();
    }

    std::cout << "Client sent patient ages: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << sharedMem->ages[i] << " ";
    }
    std::cout << std::endl;

    // Indicate that data is ready
    sharedMem->dataReady = true;

    // Wait for the server to complete processing
    while (!sharedMem->serverDone) {
        sleep(1);
    }

    // Display the sum received from the server
    std::cout << "Client received sum of ages from server: " << sharedMem->sum << std::endl;

    // Detach shared memory
    shmdt(sharedMem);
}

void server(int& shmid) {
    SharedMemory* sharedMem = (SharedMemory*)shmat(shmid, nullptr, 0);

    // Wait for the client to write data
    while (!sharedMem->dataReady) {
        sleep(1);
    }

    // Read patient ages from shared memory
    std::cout << "Server received patient ages: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << sharedMem->ages[i] << " ";
    }
    std::cout << std::endl;

    // Calculate the sum of ages
    sharedMem->sum = 0;
    for (int i = 0; i < 5; ++i) {
        sharedMem->sum += sharedMem->ages[i];
    }

    std::cout << "Server calculated sum of ages: " << sharedMem->sum << std::endl;

    // Indicate that the server has finished processing
    sharedMem->serverDone = true;

    // Detach shared memory
    shmdt(sharedMem);
}

int main() {
    // Allocate shared memory
    int shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        std::cerr << "Shared memory allocation failed!" << std::endl;
        return 1;
    }

    // Start the server process
    {
        pid_t pid = fork();
        if (pid == 0) {
            server(shmid);
            shmctl(shmid, IPC_RMID, nullptr);  // Remove shared memory after use
            return 0;
        }
    }

    // Ensure the server process starts first
    sleep(1);

    // Start the client process
    {
        pid_t pid = fork();
        if (pid == 0) {
            client(shmid);
            return 0;
        }
    }

    // Wait for both processes to finish
    wait(nullptr);
    wait(nullptr);

    return 0;
}
