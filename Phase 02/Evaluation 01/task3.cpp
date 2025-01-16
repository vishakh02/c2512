// pipe 

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
 
 
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
 
void client(int& write_to_server, int& read_from_server) {
    vector<Patient> patients = {
        Patient("101", 1),
        Patient("102", 2),
        Patient("103", 3),
        Patient("104", 4),
        Patient("105", 5)
    };
 
    int size = patients.size();
    int ages[100];
 
    // Fill the array with patient age
    for (int i = 0; i < size; ++i) {
        ages[i] = patients[i].getPatientAge();
    }
 
    // Print the client sending array
    cout << "Client sending ages: ";
    for (int i = 0; i < size; ++i) {
        cout << ages[i] << " ";
    }
    cout << endl;
 
    // Send data to server
    write(write_to_server, &size, sizeof(size));
    write(write_to_server, ages, sizeof(int) * size);
 
    // Receive the sum from server
    int sum;
    read(read_from_server, &sum, sizeof(sum));
    cout << "Client received sum from server: " << sum << endl;
 
    close(write_to_server);
    close(read_from_server);
}
 
void server(int& read_from_client, int& write_to_client) {
    int resultValues[100];
    int size;
 
    // Read size and data from client
    read(read_from_client, &size, sizeof(size));
    read(read_from_client, resultValues, sizeof(int) * size);
 
    // Print the server receiving array
    cout << "Server received ages: ";
    for (int i = 0; i < size; ++i) {
        cout << resultValues[i] << " ";
    }
    cout << endl;
 
    // Calculate the sum
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += resultValues[i];
    }
 
    cout << "Server calculated sum: " << sum << endl;
 
    // Send sum back to client
    write(write_to_client, &sum, sizeof(sum));
 
    close(read_from_client);
    close(write_to_client);
}
 
int main() {
    int pipe_client_to_server[2];
    int pipe_server_to_client[2];
 
 
 
    // Create the pipes
    if (pipe(pipe_client_to_server) == -1 || pipe(pipe_server_to_client) == -1) {
        perror("Pipe creation failed");
        return 1;
    }
 
    int& write_to_server = pipe_client_to_server[1];
    int& read_from_server = pipe_server_to_client[0];
 
    int& read_from_client = pipe_client_to_server[0];
    int& write_to_client = pipe_server_to_client[1];
 
    {
       pid_t pid = fork();
       if (0 == pid) { // Child process (Client)
           close(read_from_client); 
           close(write_to_client); 
           client(write_to_server, read_from_server);
           return 0;
        }
    }
 
    {                    // child process (Server)
        pid_t pid = fork();
        if(0 == pid) {
           close(write_to_server);
           close(write_to_server); 
           server(read_from_client, write_to_client);
           wait(nullptr); // Wait for the child process to finish
           return 0;
        }
    }
 
    wait(nullptr);
    return 0;
}
 
