#include "AgendaService.hpp"
#include <algorithm>
#include <string>
#include <iostream>
// #include "MyException.hpp"

AgendaService::AgendaService() {
    m_storage = Storage::getInstance();
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}


bool AgendaService::userLogIn(const std::string userName, const std::string password) {
    std::list<User> available = m_storage->queryUser([&userName, &password](const User& a) {
    	if(a.getName() == userName && a.getPassword() == password) {
    		return true;
    	}
    	return false;
    });
    if (!available.empty()) {
        return true;
    } else {
        return false;
    }
}

bool AgendaService::userRegister(const std::string userName, const std::string password,
                      const std::string email, const std::string phone) {
    std::list<User> exitst = m_storage->queryUser([&userName](const User& a) {
        if (a.getName() == userName) {
            return true;
        }
        return false;
    });
    if (exitst.empty()) {
        User user(userName, password, email, phone);
        m_storage->createUser(user);
        m_storage->sync();
        return true;
    } else {
        return false;
    } 
}

bool AgendaService::deleteUser(const std::string userName, const std::string password) {
    int de = m_storage->deleteUser([&userName, &password](const User& a) {
        if(a.getName() == userName && a.getPassword() == password) {
            return true;
        }
        return false;
    });
    std::list<Meeting> count = m_storage->queryMeeting([&userName](const Meeting& a) {
        auto v = a.getParticipator();
        if (find(v.begin(), v.end(), userName) != v.end() || a.getSponsor() == userName) {
            return true;
        }
        return false;
    });
    if (count.empty()) {
        if (de != 0) {
            m_storage->sync();
            return true;
        }
        return false;
    } else {
    int dc = m_storage->deleteMeeting([&userName](const Meeting& a) {
        auto v = a.getParticipator();
        if (find(v.begin(), v.end(), userName) != v.end() || a.getSponsor() == userName) {
            return true;
        }
        return false;
    });
    if (de != 0 && dc != 0) {
        m_storage->sync();
        return true;
    } else {
        return false;
    }
    }
}

std::list<User> AgendaService::listAllUsers(void) const {
    std::list<User> user = m_storage->queryUser([](const User& a) {return true;});
    return user;
}

bool AgendaService::createMeeting(const std::string userName, const std::string title,
                       const std::string startDate, const std::string endDate,
                       const std::vector<std::string> participator) {
    Date startM = Date::stringToDate(startDate);
    Date endM = Date::stringToDate(endDate);
    int Flag1 = 0;
    if (participator.empty()) {
        Flag1 = 1;
        // throw(MyException("[error!] there must be at lest one participator in a meeting!"));
    }
    auto v = participator;
    sort(v.begin(), v.end());
    for (int i = 0; i < v.size() - 1; i++) {
        if (v[i] == v[i + 1]) {
            Flag1 = 1;
            // throw(MyException("[error!] participator can not be same!"));
            break;
        }
    }
    std::list<User> sponsor = m_storage->queryUser([&userName](const User& a) {
        if (a.getName() == userName) {
            return true;
        } else {
            return false;
        }
    });
    if (sponsor.empty()) {
        Flag1 = 1;
        // throw(MyException("[error!] sponsor isn't registered user!"));
    }
    if (Flag1 == 0) {
        for (auto each : participator) {
            std::list<User> par = m_storage->queryUser([&each, &userName](const User& a) {
            if (a.getName() == each && each != userName) {
            return true;
            } else {
            return false;
            }
        });
        if (par.empty()) {
            Flag1 = 1;
            // throw(MyException("[error!] participator isn't registered user!"));
        }
    }
}
    if (Flag1 == 0) {
        if (Date::isValid(startM) == false || Date::isValid(endM) == false) {
        Flag1 = 1;
        // throw(MyException("[error!] Date format must be like \"yyyy-mm-dd/hh:mm\""));

    } 
        if (startM >= endM) {
        Flag1 = 1;
        // throw(MyException("[error!] startDate must previous to endDate!"));
    }
}
    if (Flag1 == 0) {
        std::list<Meeting> inInterval = meetingQuery(userName, startDate, endDate);
        if (!inInterval.empty()) {
        std::list<Meeting> notOverlope = m_storage->queryMeeting([&userName, &startM, &endM](const Meeting& a) {
            auto v = a.getParticipator();
            if ((a.getSponsor() == userName || find(v.begin(), v.end(), userName) != v.end()) && ((a.getEndDate() == startM )
                || (a.getStartDate() == endM))) {
                return true;
            }
            return false;
        });
        if (inInterval.size() != notOverlope.size()) {
            Flag1 = 1;
            // throw(MyException("[error!] sponsor time overlap!"));
        }
    }
}
    if (Flag1 == 0) {       
        for (auto each : participator) {
        std::list<Meeting> patin = meetingQuery(each, startDate, endDate);
        if (!patin.empty()) {
        std::list<Meeting> notOverlope = m_storage->queryMeeting([&each, &startM, &endM](const Meeting& a) {
             auto v = a.getParticipator();
            if ((find(v.begin(), v.end(), each) != v.end() || a.getSponsor() == each) && ((a.getEndDate() == startM)
                || (a.getStartDate() == endM ))) {
                return true;
            }
            return false;
        });
        if (patin.size() != notOverlope.size()) {
            Flag1 = 1;
            // throw(MyException("[error!] participator time overlap!"));
        }
    }
    }
}
 if (Flag1 == 0) {
    std::list<Meeting> ti = m_storage->queryMeeting([&title] (const Meeting& a) {
        if (a.getTitle() == title) {
            return true;
        }
        return false;
    });
    if (!ti.empty()) {
        Flag1 = 1;
        // throw(MyException("[error!] the meeting title has exist!"));
    }
}
    if (Flag1 == 0) {
        Meeting meet(userName, participator, startM, endM, title);
        m_storage->createMeeting(meet);
        m_storage->sync();
        return true;
    } else {
        return false;
    }
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string title) const {
    std::list<Meeting> meet = m_storage->queryMeeting([&userName, &title](const Meeting& a) {
        auto v = a.getParticipator();
        if ((a.getSponsor() == userName 
            || find(v.begin(), v.end(), userName) != v.end()) && a.getTitle() == title) {
            return true;
        }
        return false;
    });
    return meet;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string startDate,
                                    const std::string endDate) const {
    Date start(startDate); Date end(endDate);
    std::list<Meeting> emptyMeet;
    if (Date::isValid(start) == false || Date::isValid(end) == false) {
        return emptyMeet;
    } else if (start > end) {
        return emptyMeet;
    }
     else {
    std::list<Meeting> meet = m_storage->queryMeeting([&userName, &start, &end](const Meeting& a) {
        auto v = a.getParticipator();
        if ((a.getSponsor() == userName 
            || find(v.begin(), v.end(), userName) != v.end()) 
            && ((a.getStartDate() >= start && a.getStartDate() <= end) || (a.getEndDate() >= start 
            && a.getEndDate() <= end) || start >= a.getStartDate()  && end <= a.getEndDate())) {
            return true;
        }
        return false;
    });
    return meet;
    }
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
    std::list<Meeting> in = m_storage->queryMeeting([&userName](const Meeting& a) {
        auto v = a.getParticipator();
        if (a.getSponsor() == userName || find(v.begin(), v.end(), userName) != v.end()) {
            return true;
        }
        return false;
    });
    return in;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
    std::list<Meeting> in = m_storage->queryMeeting([&userName](const Meeting& a) {
        if (a.getSponsor() == userName) {
            return true;
        }
        return false;
    });
    return in;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
        const std::string userName) const {
     std::list<Meeting> in = m_storage->queryMeeting([&userName](const Meeting& a) {
        auto v = a.getParticipator();
        if (find(v.begin(), v.end(), userName) != v.end()) {
            return true;
        }
        return false;
    });
    return in;
}

bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
    std::list<Meeting> count = m_storage->queryMeeting([&userName](const Meeting& a) {
        return true;
    });
    if (!count.empty()) {
    int de = m_storage->deleteMeeting([&userName, &title](const Meeting& a) {
        if (a.getSponsor() == userName  && a.getTitle() == title) {
            return true;
        }
        return false;
    });
    if (de != 0) {
        m_storage->sync();
        return true;
    }
    return false; 
    }
    return false;
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
    std::list<Meeting> count = m_storage->queryMeeting([&userName](const Meeting& a) {
        return true;
    });
    if (!count.empty()) {
    int de = m_storage->deleteMeeting([&userName](const Meeting& a) {
        if (a.getSponsor() == userName) {
            return true;
        }
        return false;
    });
    if (de != 0) {
        m_storage->sync();
        return true;
    }
    return false;
    }
    return false;
}

void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    m_storage->sync();
}




// bool AgendaService::modifyPassword(const std::string userName, std::string& password) {
//     int findUser = m_storage->updateUser([&userName](const User& a) {
//         if (a.getName() == userName) {
//             return true;
//         }
//         return false;
//     },[&userName, &password](User& a) {
//         a.setPassword(password);
//     });
//     if (findUser != 0) {
//         m_storage->sync();
//         return true;
//     }
//     return false;
// }

// bool AgendaService::modifyEmail (const std::string userName, std::string& email) {
//     int findUser = m_storage->updateUser([&userName](const User& a) {
//         if (a.getName() == userName) {
//             return true;
//         }
//         return false;
//     }, [&email] (User& a) {
//         a.setEmail(email);
//     });
//     if (findUser != 0) {
//         m_storage->sync();
//         return true;
//     }
//     return false;
// }

// bool AgendaService::modifyPhone(const std::string userName, std::string& phone) {
//    int findUser = m_storage->updateUser([&userName](const User& a) {
//         if (a.getName() == userName) {
//             return true;
//         }
//         return false;
//     }, [&phone] (User& a) {
//         a.setPhone(phone);
//     });
//     if (findUser != 0) {
//         m_storage->sync();
//         return true;
//     }
//     return false;
// }
