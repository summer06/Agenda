#include"Date.hpp"
#include <iostream>
#include<sstream>

Date::Date():m_year(0), m_month(0), m_day(0),
    m_hour(0), m_minute(0) {}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute):
    m_year(t_year), m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {}

bool testChracter(std::string test) {
        for (int i = 0; i < test.size(); i++) {
            if (test[i] < '0' || test[i] > '9') {
                return false;
            }
        }
        return true;
    }

Date::Date(std::string dateString) {
    
    std::stringstream y, m, d, h, min;
    std::string year, month, day, hour, minute;
    int flag = 0;
    int a = 0;
    if (dateString.size() != 16) {
        a = 1;
    }
   for (int i = 0; i < dateString.size(); i++) {
        if (dateString[i] == '-') {
            if (i != 4) {
                a = 1;
                break;
            }
            year = dateString.substr(0, i - 0);
            if (testChracter(year)) {
            flag = i;
            break;
            } else {
                a = 1;
                break;
            }
        }
    }
    if (a == 0) {
        for (int i = flag + 1; i < dateString.size(); i++) {
            if (dateString[i] == '-') {
                if (i - flag - 1 != 2) {
                    a = 1;
                    break;
                }
                month = dateString.substr(flag + 1, i - flag - 1);
                if (testChracter(month)) {
                flag = i;
                break;
                } else {
                    a = 1;
                    break;
                }
            }
        }
    }
    if (a == 0) {
        for (int i = flag + 1; i < dateString.size(); i++) {
            if (dateString[i] == '/') {
                if (i - flag - 1 != 2) {
                    a = 1;
                    break;
                }
                 day = dateString.substr(flag + 1, i - flag - 1);
                if (testChracter(day)) {
                flag = i;
                break;
                } else {
                    a = 1;
                    break;
                }
            }
        }
    }
    if (a == 0) {
        for (int i = flag + 1; i < dateString.size(); i++) {
        if (dateString[i] == ':') {
            if (i - flag - 1 != 2) {
                a = 1;
                break;
            }
             hour = dateString.substr(flag + 1, i - flag - 1);
            if (testChracter(hour)) {
                flag = i;
                break;
                } else {
                    a = 1;
                    break;
                }
        }
    }
    }
    if (a == 0) {
        if (dateString.size() - flag - 1 != 2) {
            a = 1;
        } else {
        minute = dateString.substr(flag + 1, dateString.size() - flag - 1);
        if (!testChracter(minute)) {
            a = 1;
        }
        }
    }
    if (a == 0) {
    y << year;
    y >> m_year;
    m << month;
    m >> m_month;
    d << day;
    d >> m_day;
    h << hour;
    h >> m_hour;
    min << minute;
    min >> m_minute;
    } else {
        m_year = 0; m_month = 0; m_day = 0; m_hour = 0; m_minute = 0;
    }
}

int Date::getYear(void) const {
    return m_year;
}

void Date::setYear(const int t_year) {
    m_year = t_year;
}

int Date::getMonth(void) const {
    return m_month;
}

void Date::setMonth(const int t_month) {
    m_month = t_month;
}

int Date::getDay(void) const {
    return m_day;
}

void Date::setDay(const int t_day) {
    m_day = t_day;
}

int Date::getHour(void) const {
    return m_hour;
}

void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

int Date::getMinute(void) const {
    return m_minute;
}

void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

bool Date::isValid(const Date d_date) {
    if (d_date.getYear()< 1000 || d_date.getYear() > 9999) {
        return false;
    }
    if (d_date.getMonth() < 1 || d_date.getMonth() > 12) {
        return false;
    }
    if (d_date.getHour() < 0 || d_date.getHour() > 23) {
        return false;
    }
    if (d_date.getMinute() < 0 || d_date.getMinute() > 59) {
        return false;
    }
    if (d_date.getMonth() == 1 || d_date.getMonth() == 3 || d_date.getMonth() == 5 || d_date.getMonth() == 7 \
        || d_date.getMonth() == 8 || d_date.getMonth() == 10 || d_date.getMonth() == 12) {
        if (d_date.getDay() < 1 || d_date.getDay() > 31) {
            return false;
        }
    }
    if (d_date.getMonth() == 4 || d_date.getMonth() == 6 || d_date.getMonth() == 9 || d_date.getMonth() == 11) {
        if (d_date.getDay() < 1 || d_date.getDay() > 30) {
            return false;
        }
    }
    if ((d_date.getYear() % 4 == 0 && d_date.getYear() % 100 != 0) || d_date.getYear() % 400 == 0) {
        if (d_date.getMonth() == 2) {
            if (d_date.getDay() < 1 || d_date.getDay() > 29) {
                return false;
            }
        }
    } else {
        if (d_date.getMonth() == 2) {
            if (d_date.getDay() < 1 || d_date.getDay() > 28) {
                return false;
            }
        }
    }
    return true;
}

Date Date::stringToDate(const std::string dateString) {
    int a = 0; 
    int flag = 0;
    if (dateString.size() != 16) {
        a = 1;
    }
    for (int i = 0; i < dateString.size(); i++) {
        if (dateString[i] == '-') {
            if (i != 4) {
                a = 1;
                break;
            }
             if (testChracter(dateString.substr(0, i - 0))) {
            flag = i;
            break;
            } else {
                a = 1;
                break;
            }
        }
    }
    if (a == 0) {
        for (int i = flag + 1; i < dateString.size(); i++) {
            if (dateString[i] == '-') {
                if (i - flag - 1 != 2) {
                    a = 1;
                    break;
                }
                if (testChracter(dateString.substr(flag + 1, i - flag - 1))) {
                flag = i;
                break;
                } else {
                    a = 1;
                    break;
            }
            }
        }
    }
    if (a == 0) {
        for (int i = flag + 1; i < dateString.size(); i++) {
            if (dateString[i] == '/') {
                if (i - flag - 1 != 2) {
                    a = 1;
                    break;
                }
                if (testChracter(dateString.substr(flag + 1, i - flag - 1))) {
                flag = i;
                break;
                } else {
                    a = 1;
                    break;
            }
            }
        }
    }
    if (a == 0) {
        for (int i = flag + 1; i < dateString.size(); i++) {
        if (dateString[i] == ':') {
            if (i - flag - 1 != 2) {
                a = 1;
                break;
            }
            if (testChracter(dateString.substr(flag + 1, i - flag - 1))) {
                flag = i;
                break;
                } else {
                    a = 1;
                    break;
            }
        }
    }
    }
    if (a == 0) {
        if (dateString.size() - flag - 1 != 2) {
            a = 1;
        } else {
            if (!testChracter(dateString.substr(flag + 1, dateString.size() - flag - 1))) {
                a = 1;
            }
        }
    }
    if (a == 1) {
        Date date;
        return date;
    } else {
        Date date(dateString);
        return date;
    }
}

std::string Date::dateToString(Date t_date) {
    if (Date::isValid(t_date)) {
    std::stringstream y, m, d, h, min;
    std::string year, month, day, hour, minute;
    y << t_date.m_year;
    y >> year;
    m << t_date.m_month;
    m >> month;
    if (t_date.m_month < 10) {
        month = "0" + month;
    }
    d << t_date.m_day;
    d >> day;
    if (t_date.m_day < 10) {
        day = "0" + day;
    }
    h << t_date.m_hour;
    h >> hour;
    if (t_date.m_hour < 10) {
        hour = "0" + hour;
    }
    min << t_date.m_minute;
    min >> minute;
    if (t_date.m_minute < 10) {
        minute = "0" + minute;
    }
    std::string ddtime;
    ddtime = year + "-" + month + "-" + day + "/" + hour + ":" + minute;
    return ddtime;
    } else {
        return "0000-00-00/00:00";
    }
}

Date& Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}
bool Date::operator==(const Date &t_date) const {
    if (m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day 
        && m_hour == t_date.m_hour && m_minute == t_date.m_minute) {
        return true;
    }
    return false;
}

bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.m_year) {
        return true;
    } else if (m_year < t_date.m_year) {
        return false;
    } else {
        if (m_month > t_date.m_month) {
            return true;
        } else if (m_month < t_date.m_month) {
            return false;
        } else {
            if (m_day > t_date.m_day) {
                return true;
            }
            else if (m_day < t_date.m_day) {
                return false;
            } else {
                if (m_hour > t_date.m_hour) {
                    return true;
                } else if (m_hour < t_date.m_hour) {
                    return false;
                } else {
                    if (m_minute > t_date.m_minute) {
                        return true;
                    } else if (m_minute <= t_date.m_minute) {
                        return false;
                    }
                }
            }
        }
    }
}

bool Date::operator<(const Date &t_date) const {
    if (*this > t_date || *this == t_date) {
        return false;
    }
    return true;
}

bool Date::operator>=(const Date &t_date) const {
    if (*this > t_date || *this == t_date) {
        return true;
    }
    return false;
}

bool Date::operator<=(const Date &t_date) const {
    if (*this < t_date || *this == t_date) {
        return true;
    }
    return false;
}