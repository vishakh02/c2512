#include <iostream>
//#include <iomanip>
#include <cstdlib>

#include "Patient.h"

int main() {
        Patient p1("P001",23);
        Patient p2("P002",25);

        std::cout << std::boolalpha;
        std::cout << "Equals: " << p1.Equals(p2) << std::endl;
        std::cout << "NotEquals: " << p1.NotEquals(p2) << std::endl;
        std::cout << "LessThan: " << p1.LessThan(p2) << std::endl;
        std::cout << "LessThanEqualto: " << p1.LessThanEqualto(p2) << std::endl;
        std::cout << "GreaterThan: " << p1.GreaterThan(p2) << std::endl;
        std::cout << "GreaterThanEqualto: " << p1.GreaterThanEqualto(p2) << std::endl;

        return EXIT_SUCCESS;
}
~                    
