#include"AgendaUI.hpp"
#include"AgendaService.hpp"
#include<sstream>
#include<list>
#include<vector>
#include<iostream>
#include<iomanip>
#include "MyException.hpp"

AgendaUI::AgendaUI() {}

void AgendaUI::OperationLoop(void) {
    int choice = 3;
    while (choice != 0) {
        startAgenda();
        std::string temp = getOperation();
        while (temp < "0" || temp > "2") {
        	std::cout << "Your input is illegal, please enter again!" << std::endl;
        	std::cout << std::endl;
        	startAgenda();
        	temp = getOperation();
        }
        std::stringstream ss;
        ss << temp;
        ss >> choice;
        switch (choice) {
        	case 1: {
        		std::cout <<"[log in] [user name] [password]" << std::endl;
	            std::cout << "[log in] ";
	            std::string name, pass;
	            std::cin >> name;
	            std::cin >> pass;
	            if (m_agendaService.userLogIn(name, pass)) {
	            	std::cout << "[log in] succeed!" << std::endl;
	            	m_userName = name;
	            	m_userPassword = pass;
	            	int choice1 = 1;
                    while (choice1 != 0) {
                    	userLogIn();
                    	std::cout << "Agenda@" << m_userName << " : #" << std::endl;
                    	std::string temp = getOperation();
                    	while (temp != "0" && temp != "1" && temp != "2" && temp != "3" && temp != "4"
                    		&& temp != "5" && temp != "6" && temp != "7" && temp != "8" && temp != "9"
                    		&& temp != "10" && temp != "11" && temp != "12" && temp != "13") {
        				std::cout << "Your input is illegal, please enter again!" << std::endl;
        				std::cout << std::endl;
        				userLogIn();
        				std::cout << "Agenda@" << m_userName << " : #" << std::endl;
        				temp = getOperation();
        				}
                        std::stringstream ss;
                        ss << temp;
                        ss >> choice1;
                    	switch(choice1) {
                    		case 1: deleteUser(); choice1 = 0;break;
                            case 2: listAllUsers(); break;
                            case 3: createMeeting(); break;
                            case 4: listAllMeetings(); break;
                            case 5: listAllSponsorMeetings(); break;
                            case 6: listAllParticipateMeetings(); break;
                            case 7: queryMeetingByTitle(); break;
                            case 8: queryMeetingByTimeInterval(); break;
                            case 9: deleteMeetingByTitle(); break;
                            case 10: deleteAllMeetings(); break;
                            case 11: modifyPassword(); choice1 = 0;break;
                            case 12: modifyEmail();break;
                            case 13: modifyPhone();break;
                    	}
                    }
                    break;
	            } else {
                    std::cout << "[error] log in fail!" << std::endl;
                    break;
                }                
        	}
        	case 2: {
                userRegister();
                break;
        	}
        }
    }
    m_agendaService.quitAgenda();
    return;
}

void AgendaUI::startAgenda(void) {
     std::cout << "------------------Agenda----------------------"<< std::endl;
     std::cout <<  "Action" << std::endl;
     std::cout << " 1  --  log in Agenda by user name and passwod "<< std::endl;
     std::cout << " 2  --  register an Agenda account " << std::endl;
     std::cout << " 0  -- quit Agenda "<< std::endl;
     std::cout << "----------------------------------------------"<< std::endl;
     std::cout << std::endl;
     std::cout << " Agenda : ~$";
}

std::string AgendaUI::getOperation() {
    std::string choice;
    std::cin >> choice;
    return choice;
}

void AgendaUI::userLogIn(void) {
	std::cout << std::endl; 
	std::cout << "------------------Agenda----------------------"<< std::endl;
	std::cout << " Action :" << std::endl;
	std::cout << " 0   --  log out Agenda" << std::endl;
	std::cout << " 1   --  delete Agenda account" << std::endl;
	std::cout << " 2   --  list all Agenda user" << std::endl;
	std::cout << " 3   --  create a meeting" << std::endl;
	std::cout << " 4   --  list all meetings" << std::endl;
	std::cout << " 5   --  list all sponsor meetings" << std::endl;
	std::cout << " 6   --  list all participate meetings" << std::endl;
	std::cout << " 7   --  query meeting by title" << std::endl;
	std::cout << " 8   --  query meeting by time interval" << std::endl;
	std::cout << " 9   --  delete meeting by title" << std::endl;
	std::cout << " 10  --  delete all meetings" << std::endl;
	std::cout << " 11  --  modify password" << std::endl;
	std::cout << " 12  --  modify email" << std::endl;
	std::cout << " 13  --  modify phone" << std::endl;
	std::cout << "----------------------------------------------"<< std::endl;
	std::cout << std::endl;
}

void AgendaUI::userRegister(void) {
	std::cout << "[register] [user name] [password] [email] [phone]" << std::endl;
	std::cout << "[register] ";
	std::string name, password, email, phone;
	std::cin >> name >> password >> email >> phone;
	if (m_agendaService.userRegister(name, password, email, phone)) {
		std::cout << "[register] succeed!" << std::endl;
	} else {
		std::cout << "[error] register fail!" << std::endl;
	}
}

void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
	std::cout << " Thanks for using, goodby!" << std::endl;
}
void AgendaUI::userLogOut(void) {
}

void AgendaUI::deleteUser(void) {
	std::cout << "Are you sure to delete your account?" << std::endl;
	std::cout << "1: yes    2:no" << std::endl;
	std::string choice;
	std::cin >> choice;
	if (choice == "1") {
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
    	std::cout << "[delete agenda account] succeed!" << std::endl;
    	return;
    } else {
    	std::cout << "[delete agenda account] failed!" << std::endl;
    	return;
    }
	}
	if (choice == "2") {
		return;
	}
}

void AgendaUI::listAllUsers(void) {
	std::cout << std::left << std::setw(20) << "name" << std::left << std::setw(20) << "email" 
	<< std::left << std::setw(20) << "phone" << std::endl;
	std::list<User> show = m_agendaService.listAllUsers();
	std::list<User>::iterator p = show.begin();
	for (; p != show.end(); p++) {
		std::cout << std::left << std::setw(20) << p->getName() << std::left 
		<< std::setw(20) << p->getEmail() << std::left << std::setw(20) << p->getPhone() << std::endl;
	}
}

void AgendaUI::createMeeting(void) {
	int num;
	std::cout << "[create meeting] [the number of the participator]" << std::endl;
	std::cout << "[create meeting] ";
	std::cin >> num;
	if (num == 0) {
		std::cout << "[create meeting] error! there must be at least 1 participator" << std::endl;
	} else {
    std::vector<std::string> part;
	for (int i = 1; i < num + 1; i++) {
		std::cout << "[create meeting] [please enter the participator " << i << " ]" << std::endl;
		std::cout << "[create meeting] ";
		std::string pa;
		std::cin >> pa;
        part.push_back(pa);
	}
	std::cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
	std::cout << "[create meeting] ";
	std::string title, starttime, endtime;
	std::cin >> title >> starttime >> endtime;
	try {
		m_agendaService.createMeeting(m_userName, title, starttime, endtime, part);
		std::cout << "[create meeting] succeed!" << std::endl;
		}
	catch (MyException& a) {
		std::cout << a.what() << std::endl;
	}
	}
}

void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]" << std::endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}

void AgendaUI::listAllSponsorMeetings(void) {
	std::cout << "[list all sponsor meetings]" << std::endl;
    std::list<Meeting> show = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(show);
}

void AgendaUI::listAllParticipateMeetings(void) {
	std::cout << "[list all participator meetings]" << std::endl;
    std::list<Meeting> show = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(show);
}

void AgendaUI::queryMeetingByTitle(void) {
	std::cout << "[query meeting] [title]:" << std::endl;
	std::cout << "[query meeting] ";
	std::string name;
	std::cin >> name;
	std::list<Meeting> show = m_agendaService.meetingQuery(m_userName, name);
    printMeetings(show);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
	std::cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
	std::cout << "[query meetings] ";
	std::string start, end;
	std::cin >> start >> end;
	std::cout << "[query meetings]" << std::endl;
	std::cout << std::endl;
	std::list<Meeting> show = m_agendaService.meetingQuery(m_userName, start, end);
	printMeetings(show);
}

void AgendaUI::deleteMeetingByTitle(void) {
	std::cout << "[delete meeting] [title]" << std::endl;
	std::cout << "[delete meeting] ";
	std:: string title;
	std::cin >> title;
	std::cout << "Are you sure to delete the meeting?" << std::endl;
	std::cout << "1: yes    2:no" << std::endl;
	std::string choice;
	std::cin >> choice;
	if (choice == "1") {
	if (m_agendaService.deleteMeeting(m_userName, title)) {
		std::cout << "[delete meeting by title] succeed!" << std::endl;
		return;
	} else {
		std::cout << "[error] delete meeting fail!" << std::endl;
		return;
	} 
	}
	if (choice == "2") {
		return;
	}
}

void AgendaUI::deleteAllMeetings(void) {
	std::cout << "Are you sure to delete all meetings?" << std::endl;
	std::cout << "1: yes    2:no" << std::endl;
	std::string choice;
	std::cin >> choice;
	if ( choice == "1") {
	if (m_agendaService.deleteAllMeetings(m_userName)) {
		std::cout << "[delete all meetings] succeed!" << std::endl;
		return;
	} else {
		std::cout << "[error] fail!" << std::endl;
		return;
	}
	}
	if (choice == "2") {
		return;
	}
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
	std::cout << std::left << std::setw(20) << "title" << std::left << std::setw(20) 
	<< "sponsor" << std::left << std::setw(20) << "start time" << std::left << std::setw(20) 
	<< "end time" << std::left << std::setw(20) << "participators" << std::endl;
    std::list<Meeting>::iterator p = t_meetings.begin();
	for (; p != t_meetings.end(); p++) {
    	std::cout << std::left << std::setw(20) << p->getTitle() << std::left << std::setw(20) 
    	<< p->getSponsor() << std::left << std::setw(20) << Date::dateToString(p->getStartDate()) 
    	<< std::left << std::setw(20) << Date::dateToString(p->getEndDate());
    	std::string pa = p->getParticipator()[0];
    	for (int i = 1; i < p->getParticipator().size(); i++) {
    		pa += "," + p->getParticipator()[i];
    	}
    	std::cout << std::left << std::setw(20) << pa << std::endl;
    }
}



void AgendaUI::modifyPassword(void) {
	std::cout << "[modify password] Please enter your current password" << std::endl;
	std::string curPassword;
	std::cin >> curPassword;
	if (m_agendaService.userLogIn(m_userName, curPassword)) {
		std::cout << "[modify password] Please enter your new password" << std::endl;
		std::string newPassword;
		std::cin >> newPassword;
		std::cout << "[modify password] Please enter your new password again." << std::endl;
		std::string newPassword2;
		std::cin >> newPassword2;
		if (newPassword == newPassword2) {
			if (m_agendaService.modifyPassword(m_userName, newPassword)) {
				std::cout << "[modify password] modify password succeed!" << std::endl;
				return;
			} else {
				std::cout << "[modify password] modify password fail!" << std::endl;
				return;
			}
		} else {
			std::cout << "[modify password] Your second enter is different to your first enter." << std::endl;
			return;
		}
	} else {
		std::cout << "Your password is not correct!Please log in again!" << std::endl;
		return;
	}
}

void AgendaUI::modifyEmail(void) {
	std::cout << "[modify email] Please enter your new email address" << std::endl;
	std::string address;
	std::cin >> address;
	if (m_agendaService.modifyEmail(m_userName, address)) {
		std::cout << "[modify email] succeed!" << std::endl;
		return;
	}
	std::cout << "[modify address] fail!" << std::endl;
	return;
}

void AgendaUI::modifyPhone(void) {
	std::cout << "[modify phone] Please enter your new phone number" << std::endl;
	std::string phone;
	std::cin >> phone;
	if (m_agendaService.modifyPhone(m_userName, phone)) {
		std::cout << "[modify phone] succeed!" << std::endl;
		return;
	}
	std::cout << "[modify phone] fail!" << std::endl;
	return;
}