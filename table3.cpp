#include "table3.h"

#include <QString>

table3::Datetime::operator QString() const
{
    QString dayString;
    if (this->day < 10)
        dayString = "0" + QString::number(this->day);
    else
        dayString = QString::number(this->day);

    QString monthString;
    if(this->month < 10)
        monthString = "0" + QString::number(this->month);
    else
        monthString = QString::number(this->month);

    QString yearString = QString::number(this->year);

    QString hourString;
    if(this->hour < 10)
        hourString = "0" + QString::number(this->hour);
    else
        hourString = QString::number(this->hour);

    QString minuteString;
    if(this->minute < 10)
        minuteString = "0" + QString::number(this->minute);
    else
        minuteString = QString::number(this->minute);

    return QString("%1.%2.%3 %4:%5").arg(dayString, monthString, yearString,
                                         hourString, minuteString);
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
