#pragma once
#include "schedule.cpp"
#include "mail_sender.cpp"

class TestMailSender:public MailSender {
public:
	void sendMail(Schedule* schedule) override {
		//send mail
		countSendMailMethodIsCalled++;
	}

	int getCountSendMailMethodIsCalled() {
		return countSendMailMethodIsCalled;
	}
private:
	int countSendMailMethodIsCalled = 0;
};
