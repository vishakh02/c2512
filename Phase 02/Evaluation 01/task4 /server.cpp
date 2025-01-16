// named pipe

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

using namespace std;

int findSum(int array[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    // Named pipes (FIFOs)
    const char* pipe1 = "pipe1";
    const char* pipe2 = "pipe2";

    // Open named pipes for reading and writing
    int pipe_one_read_fd = open(pipe1, O_RDONLY);
    int pipe_two_write_fd = open(pipe2, O_WRONLY);

    if (pipe_one_read_fd == -1) {
        perror("pipe1 open failure at server");
        return 1;
    }
    if (pipe_two_write_fd == -1) {
        perror("pipe2 open failure at server");
        return 1;
    }

    // Read the number of elements from the client
    int numOfElements;
    read(pipe_one_read_fd, &numOfElements, sizeof(int));
    cout << "Server received number of elements: " << numOfElements << endl;

    // Read the patient ages array from the client
    int ages[numOfElements] = {0};
    read(pipe_one_read_fd, ages, sizeof(int) * numOfElements);
    cout << "Server received ages: ";
    for (int age : ages) {
        cout << age << " ";
    }
    cout << endl;
    close(pipe_one_read_fd);

    // Calculate the sum of ages using the helper function
    int sum = findSum(ages, numOfElements);
    write(pipe_two_write_fd, &sum, sizeof(int));
    cout << "Sum sent from server: " << sum << endl;

    close(pipe_two_write_fd);
    return 0;
}

