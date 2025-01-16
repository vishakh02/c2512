#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CONNS 5

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

void server(int port);
void serveClient(int& client_socket_fd);
void client(string server_ip, int port);
void sendPatientData(int& client_socket_fd);

void server(int port) {
    int server_socket_fd;

    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_socket_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_socket_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket_fd, MAX_CONNS) < 0) {
        perror("Listen failed");
        close(server_socket_fd);
        exit(EXIT_FAILURE);
    }

    int client_socket_fd;
    int addrlen = sizeof(address);
    if ((client_socket_fd = accept(server_socket_fd, (sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_socket_fd);
        exit(EXIT_FAILURE);
    }

    serveClient(client_socket_fd);

    close(server_socket_fd);
}

void serveClient(int& client_socket_fd) {
    char buffer[BUFFER_SIZE];
    int size;

    // Receive the size of the array
    read(client_socket_fd, buffer, sizeof(int));
    memcpy(&size, buffer, sizeof(int));

    // Receive the array of patient ages
    vector<int> ages(size);
    read(client_socket_fd, buffer, size * sizeof(int));
    memcpy(ages.data(), buffer, size * sizeof(int));

    cout << "Server received patient ages: ";
    for (int age : ages) {
        cout << age << " ";
    }
    cout << endl;

    // Calculate the sum of ages
    int sum = 0;
    for (int age : ages) {
        sum += age;
    }

    // Send the sum back to the client
    memcpy(buffer, &sum, sizeof(int));
    write(client_socket_fd, buffer, sizeof(int));

    cout << "Server calculated sum of ages: " << sum << endl;

    close(client_socket_fd);
}

void client(string server_ip, int port) {
    int client_socket_fd = 0;

    if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    sendPatientData(client_socket_fd);

    close(client_socket_fd);
}

void sendPatientData(int& client_socket_fd) {
    char buffer[BUFFER_SIZE];

    // Create and populate the list of patients
    vector<Patient> patients = {
        Patient("P001", 25),
        Patient("P002", 30),
        Patient("P003", 35),
        Patient("P004", 40),
        Patient("P005", 20)
    };

    int size = patients.size();
    vector<int> ages(size);
    for (int i = 0; i < size; ++i) {
        ages[i] = patients[i].getPatientAge();
    }

    cout << "Client sending patient ages: ";
    for (int age : ages) {
        cout << age << " ";
    }
    cout << endl;

    // Send the size of the array
    memcpy(buffer, &size, sizeof(int));
    write(client_socket_fd, buffer, sizeof(int));

    // Send the array of patient ages
    memcpy(buffer, ages.data(), size * sizeof(int));
    write(client_socket_fd, buffer, size * sizeof(int));

    // Receive the sum from the server
    int sum;
    read(client_socket_fd, buffer, sizeof(int));
    memcpy(&sum, buffer, sizeof(int));

    cout << "Client received sum of ages from server: " << sum << endl;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "usage:\n\t./patientApp server 8080" << endl;
        cout << "\t./patientApp client 127.0.0.1 8080" << endl;
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "server") == 0) {
        if (argc != 3) {
            cout << "usage:\n\t./patientApp server <port>" << endl;
            return EXIT_FAILURE;
        }

        cout << "Server listening on port " << argv[2] << endl;
        server(atoi(argv[2]));
    }

    if (strcmp(argv[1], "client") == 0) {
        if (argc != 4) {
            cout << "usage:\n\t./patientApp client <server_ip> <port>" << endl;
            return EXIT_FAILURE;
        }
        cout << "Client connecting to server at " << argv[2] << ":" << argv[3] << endl;
        client(argv[2], atoi(argv[3]));
    }

    return EXIT_SUCCESS;
}
