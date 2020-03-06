#include "AgendaService.h"

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
    std::list<User> user;
    user = storage_->queryUser([userName, password](const User& usr)
    {return usr.getName() == userName && usr.getPassword() == password;});
    if (user.size())
        return true;
    else
        return false;
}

bool AgendaService::userRegister(std::string userName, std::string password,
                    std::string email, std::string phone) {
    std::list<User> user;
    user = storage_->queryUser([userName](const User& usr)
    {return usr.getName() == userName;});
    if (user.size() == 0) {
        User tmp(userName, password, email, phone);
        storage_->createUser(tmp);
        return true;
    } else {
        return false;
    }
}

bool AgendaService::deleteUser(std::string userName, std::string password) {
    int num = storage_->deleteUser([userName, password](const User& usr)
    {return usr.getName() == userName && usr.getPassword() == password;});
    if (num != 0) {
        storage_->deleteMeeting([userName](const Meeting& meeting) {
            return meeting.getSponsor() == userName
            || meeting.getParticipator() == userName;
        });
        return true;
    }
    return false;
}

std::list<User> AgendaService::listAllUsers(void) {
    std::list<User> user;
    user = storage_->queryUser([](const User&){return true;});
    return user;
}

bool AgendaService::createMeeting(std::string userName, std::string title,
                     std::string participator,
                     std::string startDate, std::string endDate) {
    // participator doesn't exist
    std::list<User> par = storage_->queryUser([participator](const User& user) {
        return participator == user.getName();
    });
    if (par.size() == 0)
        return false;
    // participator doesn't exist
    // date valid
    Date StartDate = Date::stringToDate(startDate);
    Date EndDate = Date::stringToDate(endDate);
    if (Date::isValid(StartDate) == false)
        return false;
    if (Date::isValid(EndDate) == false)
        return false;
    if (StartDate >= EndDate)  //  time error
        return false;
    //  title overlap
    std::list<Meeting> titlemeeting;
    titlemeeting = storage_->queryMeeting([title](const Meeting& meeting) {
        return title == meeting.getTitle();
    });
    if (titlemeeting.size() != 0)
        return false;
    //  title overlap
    //  sponsor time overlap
    std::list<Meeting> usermeeting;
    usermeeting = storage_->queryMeeting([userName, StartDate, EndDate, title]
    (const Meeting& meeting) {
        Date MStartDate = meeting.getStartDate();
        Date MEndDate = meeting.getEndDate();
        return (meeting.getSponsor() == userName ||
        meeting.getParticipator() == userName) &&
        ((StartDate <= MEndDate && MEndDate <= EndDate) ||
        (MStartDate <= EndDate && EndDate <= MEndDate));
    });
    if (usermeeting.size() != 0)
        return false;
    //  sponsor time overlap
    //  participator time overlap
    std::list<Meeting> participatormeeting;
    participatormeeting = storage_->queryMeeting
    ([participator, StartDate, EndDate](const Meeting& meeting) {
        Date MStartDate = meeting.getStartDate();
        Date MEndDate = meeting.getEndDate();
    return (meeting.getSponsor() == participator || meeting.getParticipator()
    == participator) && ((StartDate <= MEndDate && MEndDate <= EndDate) ||
    (MStartDate <= EndDate && EndDate <= MEndDate));
    });
    if (participatormeeting.size() != 0)
        return false;
    //  participator time overlap
    storage_->createMeeting(Meeting(userName, participator, StartDate,
    EndDate, title));
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName,
                                                std::string title) {
    std::list<Meeting> meetinglist;
    meetinglist = storage_->queryMeeting([userName, title](const Meeting& meet)
    {return (meet.getSponsor() == userName || meet.getParticipator()
     == userName) && title == meet.getTitle();});
    return meetinglist;
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName,
std::string startDate, std::string endDate) {
    Date StartDate = Date::stringToDate(startDate);
    Date EndDate = Date::stringToDate(endDate);
    std::list<Meeting> meetinglist;
    meetinglist = storage_->queryMeeting([userName, StartDate, EndDate]
    (const Meeting& meet) {
        Date MStartDate = meet.getStartDate();
        Date MEndDate = meet.getEndDate();
        return (meet.getSponsor() == userName || meet.getParticipator()
        == userName) && ((StartDate <= MEndDate && MEndDate <= EndDate) ||
    (MStartDate <= EndDate && EndDate <= MEndDate));});
    return meetinglist;
}

std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
    std::list<Meeting> meetinglist;
    meetinglist = storage_->queryMeeting([userName](const Meeting& meet)
    {return meet.getSponsor() == userName || meet.getParticipator()
     == userName;});
    return meetinglist;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
    std::list<Meeting> meetinglist;
    meetinglist = storage_->queryMeeting([userName](const Meeting& meet)
    {return meet.getSponsor() == userName;});
    return meetinglist;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings
(std::string userName) {
    std::list<Meeting> meetinglist;
    meetinglist = storage_->queryMeeting([userName](const Meeting& meet)
    {return meet.getParticipator() == userName;});
    return meetinglist;
}

bool AgendaService::deleteMeeting(std::string userName, std::string title) {
    if (storage_->deleteMeeting([userName, title](const Meeting& met)
    {return met.getSponsor() == userName && met.getTitle() == title;}))
        return true;
    else
        return false;
}

bool AgendaService::deleteAllMeetings(std::string userName) {
    if (storage_->deleteMeeting([userName](const Meeting& met)
    {return met.getSponsor() == userName;}))
        return true;
    else
        return false;
}

void AgendaService::startAgenda(void) {
    storage_ = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    delete storage_;
    storage_ = NULL;
}
