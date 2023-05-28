#include "QString"
#include "table3.h"

bool table3::operator<(const table3::Fullname& a, const table3::Fullname& b)
{
    // Если номера одинаковые, сравниваем ФИО
    if (a.lastName < b.lastName)
        return true;
    else if (a.lastName == b.lastName)
    {
        if (a.firstName < b.firstName)
            return true;
        else if (a.firstName == b.firstName)
        {
            if (a.middleName < b.middleName)
                return true;
            else if(a.middleName == b.middleName)
            {
                if(a.phoneNumber < b.phoneNumber)
                    return true;
            }
        }
    }

    return false;
}

bool table3::operator>(const table3::Fullname& a, const table3::Fullname& b)
{
    // Если номера одинаковые, сравниваем ФИО
    if (a.lastName > b.lastName)
        return true;
    else if (a.lastName == b.lastName)
    {
        if (a.firstName > b.firstName)
            return true;
        else if (a.firstName == b.firstName)
        {
            if (a.middleName > b.middleName)
                return true;
            else if(a.middleName == b.middleName)
            {
                if(a.phoneNumber > b.phoneNumber)
                    return true;
            }
        }
    }

    return false;
}

bool table3::operator==(const table3::Fullname& a, const table3::Fullname& b)
{
    return a.lastName == b.lastName && a.firstName == a.firstName &&
            a.middleName == b.middleName && a.phoneNumber == b.phoneNumber;
}

bool table3::operator>=(const table3::Fullname& a, const table3::Fullname& b)
{
    return (a > b) || (a == b);
}

bool table3::operator<=(const table3::Fullname& a, const table3::Fullname& b)
{
    return (a < b) || (a == b);
}

bool table3::operator!=(const table3::Fullname& a, const table3::Fullname& b)
{
    return !(a == b);
}

table3::Fullname::operator QString() const
{
    return this->lastName + " " + this->firstName
            + " " + this->middleName + " -(" + QString::number(this->phoneNumber) + ")";
}

table3::Datetime::operator QString() const
{
    return QString::number(this->day) + "." + QString::number(this->month)
            + "." + QString::number(this->year) + " "
            + QString::number(this->hour) + ":" + QString::number(this->minute);
}
