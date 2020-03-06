#include "Date.h"
#include <sstream>
#include <vector>
template<typename T>
std::string trans(T val) {
    std::string tmp = "";
    std::stringstream str;
    str << val;
    str >> tmp;
    return tmp;
}

int trani(const std::string& str) {
    int ans;
    std::stringstream ss;
    ss.clear();
    ss.str("");
    ss << str;
    ss >> ans;
    return ans;
}

Date::Date() {
    year_ = month_ = day_ = hour_ = minute_ = 0;
}

Date::Date(int y, int m, int d, int h, int mi) {
    year_ = y;
    month_ = m;
    day_ = d;
    hour_ = h;
    minute_ = mi;
}

int Date::getYear(void) const {
    return year_;
}

void Date::setYear(int year) {
    year_ = year;
}

int Date::getMonth(void) const {
    return month_;
}

void Date::setMonth(int month) {
    month_ = month;
}

int Date::getDay(void) const {
    return day_;
}

void Date::setDay(int day) {
    day_ = day;
}

int Date::getHour(void) const {
    return hour_;
}

void Date::setHour(int hour) {
    hour_ = hour;
}

int Date::getMinute(void) const {
    return minute_;
}

void Date::setMinute(int minute) {
    minute_ = minute;
}

bool Date::isValid(Date date) {  //  using private member
    if (date.year_ < 1000 || date.year_ > 9999) {
        return false;
    }  //  [1000,9999]
    if (date.hour_ < 0 || date.hour_ >= 24) {
        return false;
    }  // [1000,9999]-[]-[]/[0,23]:[]
    if (date.minute_ < 0 || date.minute_ >= 60) {
        return false;
    }  // [1000,9999]-[]-[]/[0,23]:[0,59]
    if (date.month_ <= 0 || date.month_ > 12) {
        return false;
    }  // [1000,9999]-[1,12]-[]/[0,23]:[0,59]
    if (date.month_ == 2) {  // [1000,9999]-[2]-[]/[0,23]:[0,59]
        if (!(date.year_%400) || (!(date.year_%4) && date.year_%100)) {
            if (date.day_ > 29 || date.day_ <= 0)   // [1000,9999]-[2]-[1,29]/[0,23]:[0,59]
                return false;
        } else {
            if (date.day_ > 28 || date.day_ <= 0) {  // [1000,9999]-[2]-[1,28]/[0,23]:[0,59]
                return false;
            }
        }
    } else if (date.month_ == 1 || date.month_ == 3 || date.month_ == 5 ||
    date.month_ == 7 || date.month_ == 8 || date.month_ == 10 ||
    date.month_ == 12) {
        if (date.day_ > 31 || date.day_ <= 0)
            return false;
    } else if (date.month_ == 4 || date.month_ == 6 ||
    date.month_ == 9 || date.month_ == 11) {
        if (date.day_ > 30 || date.day_ <= 0)
            return false;
    }
    return true;
}

Date Date::stringToDate(std::string dateString) {
    std::vector<int> ans;
    std::string str = "";
    int len = dateString.size();
    for (int i = 0; i < len; i++) {
        if ('0' <= dateString[i] && dateString[i] <= '9') {
            str+=dateString[i];
        } else if (str.size() != 0) {
            int temp = trani(str);
            str = "";
            ans.push_back(temp);
        }
    }
    if (str.size() != 0) {
        int temp = trani(str);
        str = "";
        ans.push_back(temp);
    }
    if (ans.size() >= 5) {
        Date tmp(ans[0], ans[1], ans[2], ans[3], ans[4]);
        if (isValid(tmp))
            return tmp;
        else return Date();
    } else
        return Date();
}

std::string Date::dateToString(Date date) {
    std::string ans = "";
    ans = trans(date.year_)+"-";
    if (date.month_ < 10) {
        ans+='0';
    }
    ans+=trans(date.month_)+"-";
    if (date.day_ < 10) {
        ans+='0';
    }
    ans+=trans(date.day_)+'/';
    if (date.hour_ < 10) {
        ans+='0';
    }
    ans+=trans(date.hour_)+':';
    if (date.minute_ < 10) {
        ans+='0';
    }
    ans+=trans(date.minute_);
    return ans;
}

Date& Date::operator=(const Date& date) {
    if (isValid(date)) {
        year_ = date.year_;
        month_ = date.month_;
        day_ = date.day_;
        hour_ = date.hour_;
        minute_ = date.minute_;
    }
    return *this;
}

bool Date::operator==(const Date& date) const {
    return (year_ == date.year_) && (month_ == date.month_) &&
    (day_ == date.day_) && (hour_ == date.hour_) && (minute_ == date.minute_);
}

bool Date::operator>(const Date& date) const {
    std::vector<int> sort;
    sort.push_back(year_-date.year_);
    sort.push_back(month_-date.month_);
    sort.push_back(day_-date.day_);
    sort.push_back(hour_-date.hour_);
    sort.push_back(minute_-date.minute_);
    for (int i = 0; i < 5; i++) {
        if (sort[i] == 0 && i == 4)
            return false;
        else if (sort[i] > 0)
            return true;
        else if (sort[i] < 0)
            return false;
    }
}

bool Date::operator<(const Date& date) const {
    return !((*this) > date || (*this) == date);
}

bool Date::operator>=(const Date& date) const {
    return !((*this) < date);
}

bool Date::operator<=(const Date& date) const {
    return !((*this) > date);
}

