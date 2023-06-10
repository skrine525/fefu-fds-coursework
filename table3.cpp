#include "QString"
#include "table3.h"

table3::Datetime::operator QString() const
{
    return QString::number(this->day) + "." + QString::number(this->month)
            + "." + QString::number(this->year) + " "
            + QString::number(this->hour) + ":" + QString::number(this->minute);
}

bool table3::operator<(const table3::Datetime& a, const table3::Datetime& b)
{
    if (a.year != b.year)
        return a.year < b.year;
    if (a.month != b.month)
        return a.month < b.month;
    if (a.day != b.day)
        return a.day < b.day;
    if (a.hour != b.hour)
        return a.hour < b.hour;
    return a.minute < b.minute;
}

bool table3::operator>(const table3::Datetime& a, const table3::Datetime& b)
{
    return b < a;
}

bool table3::operator==(const table3::Datetime& a, const table3::Datetime& b)
{
    return a.year == b.year && a.month == b.month && a.day == b.day &&
            a.hour == b.hour && a.minute == b.minute;
}
