#include "Storage.h"
#include <fstream>
#include <vector>

std::string transfer(const std::string str) {
    std::string ans = str;
    int pos = 0;
    while (ans.find("\"", pos) != std::string::npos) {
        pos = ans.find("\"", pos);
        ans.insert(pos, "\"");
        pos+=2;
    }
    return ans; 
}  //  pass

Storage* Storage::instance_ = NULL;
Storage::Storage() {
    readFromFile("");
}
Storage* Storage::getInstance(void) {
    if (instance_ == NULL) {
        instance_ = new Storage;
    }
    return instance_;
}

Storage::~Storage() {
    if (instance_ != NULL) {
        sync();
        //delete instance_;
        instance_ = NULL;
    }
}

void Storage::createUser(const User& usr) {
    userList_.push_back(usr);
}

std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    std::list<User> userlist;
    std::list<User>::iterator it;
    for (it = userList_.begin(); it != userList_.end(); it++) {
        if (filter(*it)) {
            userlist.push_back(*it);
        }
    }
    return userlist;
}

int Storage::updateUser(std::function<bool(const User&)> filter,
        std::function<void(User&)> switcher) {
    std::list<User>::iterator it;
    int sum = 0;
    for (it = userList_.begin(); it != userList_.end(); it++) {
        if (filter(*it)) {
            switcher(*it);
            sum++;
        }
    }
    return sum;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
    std::list<User>::iterator it;
    std::list<User>::iterator topop = userList_.end();
    --topop;
    int sum = 0;
    for (it = userList_.begin(); it != userList_.end(); it++) {
        if (filter(*it)) {
            ++sum;
            if (it == topop) {
                userList_.pop_back();
                //  ++sum;
                break;
            } else {
                it = userList_.erase(it);
                --it;
            }
            //  ++sum;
        }
    }
    return sum;
}

void Storage::createMeeting(const Meeting& meeting) {
    meetingList_.push_back(meeting);
}

std::list<Meeting> Storage::queryMeeting(
std::function<bool(const Meeting&)> filter) {
    std::list<Meeting> meetinglist;
    std::list<Meeting>::iterator it;
    for (it = meetingList_.begin(); it != meetingList_.end(); it++) {
        if (filter(*it)) {
            meetinglist.push_back(*it);
        }
    }
    return meetinglist;
}

int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
        std::function<void(Meeting&)> switcher) {
    std::list<Meeting>::iterator it;
    int sum = 0;
    for (it = meetingList_.begin(); it != meetingList_.end(); it++) {
        if (filter(*it)) {
            switcher(*it);
            sum++;
        }
    }
    return sum;
}

int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    std::list<Meeting>::iterator it;
    std::list<Meeting>::iterator topop = meetingList_.end();
    --topop;
    int sum = 0;
    for (it = meetingList_.begin(); it != meetingList_.end(); it++) {
        if (filter(*it)) {
            sum++;
            if (it == topop) {
                meetingList_.pop_back();
                break;
            } else {
                it = meetingList_.erase(it);
                --it;
            }
            //++sum;
        }
    }
    return sum;
}

bool Storage::readFromFile(const char *fpath) {
    std::string path(fpath);
    std::string userpath = path+"User.csv";
    std::string meetingpath = path+"Meeting.csv";
    std::ifstream input;
    //  user part
    input.open(userpath);
    if (!input.is_open())
        return false;
    std::string data;
    while (getline(input, data)) {
        if (data == "\"name\",\"password\",\"email\",\"phone\"") {
            //first = false;
            continue;
        }  //  row115
        int len = data.size();
        std::vector<std::string> record;
        std::string temp = "";
        for (int i = 1; i < len-1; i++) {  //  5085465"
            if (data[i] == '"' && data[i+1] == ',' && data[i+2] == '"') {
                record.push_back(temp);
                temp = "";
                i = i+2;
            } else if (data[i] == '"' && data[i+1] == '"') {
                temp+='"';
                ++i;
            } else {
                temp+=data[i];
            }
        }
        record.push_back(temp);
        if (record.size() >= 4) {
            User info(record[0], record[1], record[2], record[3]);
            userList_.push_back(info);
        }
    }
    input.close();
    //  meeting part
    input.open(meetingpath);
    if (!input.is_open())
        return false;
    //first = true;
    while (getline(input, data)) {
        if (data ==
        "\"sponsor\",\"participator\",\"sdata\",\"edata\",\"title\"") {
            //first = false;
            continue;
        }   //  row115
        int len = data.size();
        std::vector<std::string> record;
        std::string temp = "";
        for (int i = 1; i < len-1; i++) {
            if (data[i] == '"' && data[i+1] == ',' && data[i+2] == '"') {
                record.push_back(temp);
                temp = "";
                i = i+2;
            } else if (data[i] == '"' && data[i+1] == '"') {
                temp+='"';
                ++i;
            } else {
                temp+=data[i];
            }
        }
        record.push_back(temp);
        if (record.size() >= 5) {
            Meeting info(record[0], record[1], Date::stringToDate(record[2]),
            Date::stringToDate(record[3]), record[4]);
            meetingList_.push_back(info);
        }
    }
    input.close();
    return true;
}

bool Storage::writeToFile(const char *fpath) {
    std::ofstream output;
    std::string path(fpath);
    std::string userpath = path+"User.csv";
    std::string meetingpath = path+"Meeting.csv";
    output.open(userpath);
    output << "\"name\",\"password\",\"email\",\"phone\"" << std::endl;
    if (!output.is_open())
        return false;
    std::list<User>::iterator it;
    for (it = userList_.begin(); it != userList_.end(); ++it) {
        output << '"'+transfer(it->getName())+"\",\""
        +transfer(it->getPassword())+"\",\""+transfer(it->getEmail())
        +"\",\""+it->getPhone()+'"' << std::endl;
    }
    output.close();
    output.open(meetingpath);
    output << "\"sponsor\",\"participator\",\"sdata\",\"edata\",\"title\""
            << std::endl;
    if (!output.is_open())
        return false;
    std::list<Meeting>::iterator jt;
    for (jt = meetingList_.begin(); jt != meetingList_.end(); ++jt) {
        output << '"'+transfer(jt->getSponsor())+"\",\""
        +transfer(jt->getParticipator())+"\",\""
        +Date::dateToString(jt->getStartDate())+"\",\""
        +Date::dateToString(jt->getEndDate())+"\",\""
        +transfer(jt->getTitle())+'"' << std::endl;
    }
    output.close();
    return true;
}

bool Storage::sync(void) {
    return writeToFile("./");
}
