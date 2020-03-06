#include "AgendaUI.h"
#include <iomanip>
void unloginmenu() {
    std::cout << "----------------------- Agenda -----------------------------";
    std::cout << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "l   - log in Agenda by user name and password" << std::endl;
    std::cout << "r   - register an Agenda account" << std::endl;
    std::cout << "q   - quit Agenda" << std::endl;
    std::cout << "------------------------------------------------------------";
    std::cout << std::endl << std::endl;
    std::cout << "Agenda : ~$ ";
}

void loginmenu(const std::string& username) {
    std::cout << "----------------------- Agenda -----------------------------";
    std::cout << "" << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "o   - log out Agenda" << std::endl;
    std::cout << "dc  - delete Agenda account" << std::endl;
    std::cout << "lu  - list all Agenda user" << std::endl;
    std::cout << "cm  - create a meeting" << std::endl;
    std::cout << "la  - list all meetings" << std::endl;
    std::cout << "las - list all sponsor meetings" << std::endl;
    std::cout << "lap - list all participate meetings" << std::endl;
    std::cout << "qm  - query meeting by title" << std::endl;
    std::cout << "qt  - query meeting by time interval" << std::endl;
    std::cout << "dm  - delete meeting by title" << std::endl;
    std::cout << "da  - delete all meetings" << std::endl;
    std::cout << "------------------------------------------------------------";
    std::cout << std::endl << std::endl;
    std::cout << "Agenda@" << username << " : # ";
}
AgendaUI::AgendaUI() {
    userName_ = userPassword_ = "";
}

void AgendaUI::startAgenda(void) {
    agendaService_.startAgenda();
}

void AgendaUI::OperationLoop(void) {
    //unloginmenu();
    bool quit = false;
    while (true) {
        while (userName_ != "") {
            loginmenu(userName_);
            std::string op = getOperation();
            executeOperation(op);
        }
        while (userName_ == "") {
            unloginmenu();
            std::string op = getOperation();
            if (op == "q") {
                quit = true;
                break;
            }
            else if (op == "r") {
                userRegister();
            }
            else if(op == "l") {
                userLogIn();
            }
        }
        if (quit)
            break;
    }
}

std::string AgendaUI::getOperation() {
    std::string oper;
    std::cin >> oper;
    std::cout << std::endl;
    return oper;
}

bool AgendaUI::executeOperation(std::string op) {
    if (op == "o") {
        userLogOut();
        //return true;
    }
    else if (op == "dc") {
        deleteUser();
        return true;
    }
    else if (op == "lu")
        listAllUsers();
    else if (op == "cm")
        createMeeting();
    else if (op == "la")
        listAllMeetings();
    else if (op == "las")
        listAllSponsorMeetings();
    else if (op == "lap")
        listAllParticipateMeetings();
    else if (op == "qm")
        queryMeetingByTitle();
    else if (op == "qt")
        queryMeetingByTimeInterval();
    else if (op == "dm")
        deleteMeetingByTitle();
    else if (op == "da")
        deleteAllMeetings();
    std::cout << std::endl;
    return false;
}

void AgendaUI::userLogIn(void) {
    std::cout << "[log in] [user name] [password]" << std::endl;
    std::cout << "[log in] ";
    std::string name, pw;
    std::cin >> name >> pw;
    if (agendaService_.userLogIn(name, pw)) {
        userName_ = name;
        userPassword_ = pw;
        std::cout << "[log in] succeed!" << std::endl << std::endl;
    } else {
        std::cout << "[error] log in fail!" << std::endl << std::endl;
    }
}

void AgendaUI::userRegister(void) {
    std::cout << "[register] [user name] [password] [email] [phone]"
    << std::endl;
    std::cout << "[register] ";
    std::string name, pw, email, phone;
    std::cin >> name >> pw >> email >> phone;
    if (agendaService_.userRegister(name, pw, email, phone)) {
        std::cout << "[register] succeed!" << std::endl << std::endl;
    } else {
        std::cout << "[error] register fail!" << std::endl << std::endl;
    }
}

void AgendaUI::quitAgenda(void) {
    agendaService_.quitAgenda();
}

void AgendaUI::userLogOut(void) {
    userName_ = userPassword_ = "";
}

void AgendaUI::deleteUser(void) {
    if (agendaService_.deleteUser(userName_, userPassword_)) {
        std::cout << "[delete agenda account] succeed!" << std::endl;
        std::cout << std::endl;
        userLogOut();
    } else {
        std::cout << "[error] delete agenda account fail!" << std::endl;
        std::cout << std::endl;
    }
}

void AgendaUI::listAllUsers(void) {
    std::cout << "[list all users]" << std::endl << std::endl;
    std::cout << std::setw(10) << std::left << "name";
    std::cout << std::setw(20) << std::left << "email";
    std::cout << std::setw(15) << std::left << "phone";
    std::cout << std::endl;
    std::list<User> userlist = agendaService_.listAllUsers();
    for (std::list<User>::iterator it = userlist.begin(); it != userlist.end()
    ; it++) {
        std::cout << std::setw(10) << std::left << it->getName()+"  ";
        std::cout << std::setw(20) << std::left << it->getEmail()+"  ";
        std::cout << std::setw(15) << std::left << it->getPhone();
        std::cout << std::endl;
    }
}

void AgendaUI::createMeeting(void) {
    std::cout << "[create meeting] [title] [participator] "
    << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
    << std::endl;
    std::cout << "[create meeting] ";
    std::string title, pa, start, end;
    std::cin >> title >> pa >> start >> end;
    if (agendaService_.createMeeting(userName_, title, pa, start, end)) {
        std::cout << "[create meeting] succeed!" << std::endl << std::endl;
    } else {
        std::cout << "[error] create meeting fail!" << std::endl;
    }
}

void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]" << std::endl << std::endl;
    std::list<Meeting> mlist = agendaService_.listAllMeetings(userName_);
    printMeetings(mlist);
}

void AgendaUI::listAllSponsorMeetings(void) {
    std::cout << "[list all sponsor meetings]" << std::endl << std::endl;
    std::list<Meeting> mlist = agendaService_.listAllSponsorMeetings(userName_);
    printMeetings(mlist);
}

void AgendaUI::listAllParticipateMeetings(void) {
    std::cout << "[list all participate meetings]" << std::endl << std::endl;
    std::list<Meeting> mlist =
    agendaService_.listAllParticipateMeetings(userName_);
    printMeetings(mlist);
}

void AgendaUI::queryMeetingByTitle(void) {
    std::cout << "[query meetings] [title]:" << std::endl;
    std::cout << "[query meetings] ";
    std::string title;
    std::cin >> title;
    std::cout << std::endl;
    std::list<Meeting> mlist = agendaService_.meetingQuery(userName_, title);
    printMeetings(mlist);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] ";
    std::cout << "[end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::cout << "[query meetings] ";
    std::string st, et;
    std::cin >> st >> et;
    std::cout << std::endl;
    std::list<Meeting> mlist = agendaService_.meetingQuery(userName_, st, et);
    printMeetings(mlist);
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::cout << "[delete meeting] [title]" << std::endl;
    std::cout << "[delete meeting] ";
    std::string title;
    std::cin >> title;
    std::cout << std::endl;
    if (agendaService_.deleteMeeting(userName_, title)) {
        std::cout << "[delete meeting by title] succeed!" << std::endl;
    } else {
        std::cout << "[error] delete meeting fail!" << std::endl;
    }
}

void AgendaUI::deleteAllMeetings(void) {
    if (agendaService_.deleteAllMeetings(userName_)) {
        std::cout << "[delete all meetings] succeed!" << std::endl;
    } else {
        std::cout << "[error] delete meeting fail!" << std::endl;
    }
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
    std::cout << std::setw(20) << std::left << "title";
    std::cout << std::setw(15) << std::left << "sponsor";
    std::cout << std::setw(15) << std::left << "participator";
    std::cout << std::setw(20) << std::left << "start time";
    std::cout << std::setw(20) << std::left << "end time" << std::endl;
    std::list<Meeting>::iterator it;
    for (it = meetings.begin(); it != meetings.end(); it++) {
        std::string st = Date::dateToString(it->getStartDate());
        std::string et = Date::dateToString(it->getEndDate());
        std::cout << std::setw(20) << std::left << it->getTitle()+"  ";
        std::cout << std::setw(15) << std::left << it->getSponsor()+"  ";
        std::cout << std::setw(15) << std::left << it->getParticipator()+"  ";
        std::cout << std::setw(20) << std::left << st+"  ";
        std::cout << std::setw(20) << std::left << et+"  " << std::endl;
    }
}
