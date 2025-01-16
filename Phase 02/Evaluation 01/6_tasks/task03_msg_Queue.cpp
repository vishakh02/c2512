// Message Queue

#include <iostream>
#include <vector>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <unistd.h>

using namespace std;

class Patient {
private:
    string PatientId;
    int PatientAge;

public:
    Patient(string id, int age) : PatientId(id), PatientAge(age) {}

    string getPatientId() const { return PatientId; }
    int getPatientAge() const { return PatientAge; }
};

struct Message {
    long msg_type;
    int data[100];
};

void client(int msgid) {
    // Create a list of patients
    vector<Patient> patients = {
        Patient("P001", 25),
        Patient("P002", 30),
        Patient("P003", 35),
        Patient("P004", 40),
        Patient("P005", 20)
    };

    int size = patients.size();
    Message msg;
    msg.msg_type = 1;

    // Fill the message data with patient ages
    for (int i = 0; i < size; ++i) {
        msg.data[i] = patients[i].getPatientAge();
    }

    cout << "Client sending patient ages: ";
    for (int i = 0; i < size; ++i) {
        cout << msg.data[i] << " ";
    }
    cout << endl;

    // Send the message to the server
    msgsnd(msgid, &msg, sizeof(msg.data), 0);

    // Receive the sum from the server
    int sum;
    msgrcv(msgid, &msg, sizeof(msg.data), 2, 0);
    sum = msg.data[0];
    cout << "Client received sum of ages from server: " << sum << endl;
}

void server(int msgid) {
    Message msg;
    int sum = 0;

    // Receive the patient ages from the client
    msgrcv(msgid, &msg, sizeof(msg.data), 1, 0);

    cout << "Server received patient ages: ";
    for (int i = 0; i < 5; ++i) {
        cout << msg.data[i] << " ";
        sum += msg.data[i];
    }
    cout << endl;

    cout << "Server calculated sum of ages: " << sum << endl;

    // Send the sum back to the client
    msg.msg_type = 2;
    msg.data[0] = sum;
    msgsnd(msgid, &msg, sizeof(msg.data), 0);
}

int main() {
    int msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Message queue creation failed");
        return 1;
    }

    {
        int pid = fork();
        if (pid == 0) {
            client(msgid);
            return 0;
        }
    }

    {
        int pid = fork();
        if (pid == 0) {
            server(msgid);
            return 0;
        }
    }

    wait(nullptr);
    wait(nullptr);

    // Remove the message queue
    msgctl(msgid, IPC_RMID, nullptr);
    return 0;
}
