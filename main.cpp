#include "AgendaService.h"
#include <iostream>
int main() {
    AgendaService a1;
    AgendaService *ptr = &a1;
    ptr->startAgenda();
    //test userLogIn false
    std::cout << ptr->userLogIn("oyz", "9010sozihrrocam") << std::endl;
    //test userRegister
    std::cout << ptr->userRegister("oyz", "9010sozihrrocam",
    "645948261@qq.com", "13535085465") << std::endl;
    std::cout << ptr->userRegister("oyz", "9010sozihrrocam",
    "645948261@qq.com", "13535085465")<< std::endl;
    std::cout << ptr->userRegister("Sicily", "9010\"sozi,hrrocam",
    "645948261@qq.com", "13535085465")<< std::endl;
    //test listAllUsers
    std::list<User> userlist = ptr->listAllUsers();
    for (std::list<User>::iterator it = userlist.begin(); it != userlist.end();
    ++it) {
        std::cout << it->getName()+" "+it->getPassword()+" "
        << it->getEmail()+" "+it->getPhone() << std::endl;
    }
    //test userLogIn true
    std::cout << ptr->userLogIn("oyz", "9010sozihrrocam") << std::endl;
    //test createMeeting
    std::cout << ptr->createMeeting("Sicil", "love1", "oyz", "2014-08-18/08:00",
    "2015-7-8/09:00") << std::endl;
    //test deleteUser
    std::cout << ptr->deleteUser("Sicily", "9010\"sozihrrocam") << std::endl;
    //std::cout << ptr->deleteUser("Sicily", "9010\"sozi,hrrocam") << std::endl;
    //test
    ptr->quitAgenda();
    //std::cout << ptr << std::endl;
    return 0;
}
