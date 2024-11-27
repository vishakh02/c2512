#include <string>

#include "Patient.h"

using std::string;

bool Patient::Equals(const Patient& other)
{
        return(Age == other.Age);
}
bool  Patient::NotEquals(const Patient& other)
{
        return(Age != other.Age);
}

bool  Patient::LessThan(const Patient& other)
{
        return(Age < other.Age);
}

bool  Patient::LessThanEqualto(const Patient& other)
{
        return(Age <= other.Age);
}

bool  Patient::GreaterThan(const Patient& other)
{
        return(Age > other.Age);
}

bool  Patient::GreaterThanEqualto(const Patient& other)
{
        return(Age >= other.Age);
}

Patient::Patient(string p_PatientId, int p_Age)
{
        PatientId = p_PatientId;
        Age = p_Age;
}
