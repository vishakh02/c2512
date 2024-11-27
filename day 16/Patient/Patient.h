include <string>

using std::string;

class Patient
{
        //Member data
        private:
        string PatientId;
        int Age;

        //Member functions
        public:
        bool Equals(const Patient& other);
        bool NotEquals(const Patient& other);
        bool LessThan(const Patient& other);
        bool LessThanEqualto(const Patient& other);
        bool GreaterThan(const Patient& other);
        bool GreaterThanEqualto(const Patient& other);

        //Constructor
        Patient(string p_PatientId, int p_Age);
};
