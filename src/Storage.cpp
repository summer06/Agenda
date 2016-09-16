#include"Storage.hpp"
#include"Path.hpp"
#include <fstream>
#include <vector>
#include <iostream>

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
	readFromFile();
}
std::list<std::string> stringToList(std::string& data) {
	std::string str = "";
	std::list<std::string> List;
	for (auto each : data) {
		if (each != '\n') {
			str += each;
		} else {
			List.push_back(str);
			str = "";
		}
	}
	return List;
}
void PassInfo(std::string& data, std::string& info, int& startIndex) {
	for (; data[startIndex] != '\"' ; startIndex++) {
		info += data[startIndex];
	}
}

bool Storage::readFromFile(void) {
    std::fstream rfile(Path::userPath, std::fstream::in);
    if (!rfile) {
    	return false;
    } else {
    	std::string name, password, email, phone;
    	std::string data((std::istreambuf_iterator<char>(rfile)), std::istreambuf_iterator<char>());
    	std::list<std::string> UserList = stringToList(data);
    	for (auto each : UserList) {
    		std::string name, password, email, phone;
    		int startIndex = 1;
    		PassInfo(each, name, startIndex);
    		startIndex += 3;
    		PassInfo(each, password, startIndex);
    		startIndex += 3;
    		PassInfo(each, email, startIndex);
    		startIndex += 3;
    		PassInfo(each, phone, startIndex);
    		startIndex = 1;
    		m_userList.push_back(User(name, password, email, phone));
    	}
    }
    rfile.close();
    rfile.clear();
    rfile.open(Path::meetingPath, std::fstream::in);
    if (!rfile) {
    	return false;
    } else {
    	std::list<std::string> itemList;
    	while (!rfile.eof()) {
    		std::string item;
    		getline(rfile, item);
    		itemList.push_back(item);
    	}
    	itemList.pop_back();
    	for (auto each : itemList) {
    		std::string sponsor, participator, startDate, endDate, title;
    		int startIndex = 1;
    		PassInfo(each, sponsor, startIndex);
    		startIndex += 3;
    		PassInfo(each, participator, startIndex);
    		startIndex += 3;
    		PassInfo(each, startDate, startIndex);
    		startIndex += 3;
    		PassInfo(each, endDate, startIndex);
    		startIndex += 3;
    		PassInfo(each, title,startIndex);
    		startIndex = 1;
    		std::vector<std::string> paList;
    		std::string eachPa = "";
            for (int i = 0; i < participator.size(); i++) {
            	if (participator[i] != '&') {
                   eachPa += participator[i];
            	} else {
            		paList.push_back(eachPa);
            		eachPa = "";
            	}
            }
            paList.push_back(eachPa);
            Date start = Date::stringToDate(startDate);
            Date end = Date::stringToDate(endDate);
            m_meetingList.push_back(Meeting(sponsor, paList, start, end, title));
    	}
    	}
    return true;
}

bool Storage::writeToFile(void) {
	std::fstream wfile;
	wfile.open(Path::userPath, std::fstream::out);
	if (!wfile) {
		return false;
	}
	else {
		std::list<User>::iterator p = m_userList.begin();
		for (; p != m_userList.end(); p++) {
			wfile << "\"" + p->getName() + "\"," << "\"" + p->getPassword() + "\"," 
			<< "\"" + p->getEmail() + "\"," << "\"" + p->getPhone() + "\"" << std::endl;
		}
	}
	wfile.close();
	wfile.clear();
	wfile.open(Path::meetingPath, std::fstream::out);
	if (!wfile) {
		return false;
	} else {
		std::list<Meeting>::iterator p = m_meetingList.begin();
		for (; p != m_meetingList.end(); p++) {
			std::string s = Date::dateToString(p->getStartDate());
			std::string e = Date::dateToString(p->getEndDate());
			std::string name;
			name = p->getParticipator()[0];
			for (int i = 1; i < p->getParticipator().size(); i++) {
				name = name + "&" + p->getParticipator()[i];
			}
			wfile << "\"" + p->getSponsor() + "\"," << "\"" + name + "\"," 
			<< "\"" + s + "\"," << "\"" + e + "\"," << "\"" + p->getTitle() + "\"" << std::endl;
		}
	}
}

std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr) {
		m_instance = std::shared_ptr<Storage>(new Storage());
	}
	return m_instance;
}

Storage::~Storage() {
	writeToFile();
}

void Storage::createUser(const User& t_user) {
	m_userList.push_back(t_user);
}

std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) const {
	std::list<User> result;
	auto p = m_userList.begin();
	for (; p !=  m_userList.end(); p++) {
		if (filter(*p)) {
			result.push_back(*p);
		}
	 }
	return result;
}

int Storage::updateUser(std::function<bool(const User&)> filter, std::function<void(User&)> switcher) {
	int count = 0;
	std::list<User>::iterator p = m_userList.begin();
	for (; p != m_userList.end(); p++) {
		if (filter(*p)) {
			switcher(*p);
			count++;
		}
	}
	return count;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
	int count = 0;
	std::list<User>::iterator p = m_userList.begin();
	for (; p != m_userList.end(); ) {
		if (filter(*p)) {
			p = m_userList.erase(p);
			count++;
		} else {
			p++;

		}
	}
	return count;
}
void Storage::createMeeting(const Meeting& t_meeting) {
	m_meetingList.push_back(t_meeting);
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting&)> filter) const {
	std::list<Meeting> result;
	auto p = m_meetingList.begin();
	for (; p != m_meetingList.end(); p++) {
		if (filter(*p)) {
			result.push_back(*p);
		}
	}
	return result;
}

int Storage::updateMeeting(std::function<bool(const Meeting&)> filter, std::function<void(Meeting&)> switcher) {
	int count = 0;
	std::list<Meeting>::iterator p = m_meetingList.begin();
	for (; p != m_meetingList.end(); p++) {
		if (filter(*p)) {
			switcher(*p);
			count++;
		}
	}
	return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
	int count = 0;
	std::list<Meeting>::iterator p = m_meetingList.begin();
	for (; p != m_meetingList.end(); ) {
		if (filter(*p)) {
			p = m_meetingList.erase(p);
			count++;
		} else {
			p++;
		}
	}
	return count;
}

bool Storage::sync(void) {
	if (writeToFile()) {
		return true;
	}
	return false;
}