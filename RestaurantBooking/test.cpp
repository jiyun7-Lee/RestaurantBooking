#include "gmock/gmock.h"
#include "booking_scheduler.cpp"
#include "test_sms_sender.cpp"
#include "test_email_sender.cpp"

using namespace testing;

class BookingItest : public Test {
protected:
	void SetUp() override {
		ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);
		NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);

		bookingScheduler.setSmsSender(&testSmsSender);
		bookingScheduler.setMailSender(&testableMailSender);
	}
public:
	tm getTime(int year, int month, int day, int hour, int min)
	{
		tm result = { 0, min, hour, day, month, year - 1900, 0, 0 , -1 };

		mktime(&result);
		return result;
	}
	tm plus_hour(tm base, int hour)
	{
		base.tm_hour += 1;
		mktime(&base);
		return base;
	}

	tm ON_THE_HOUR;
	tm NOT_ON_THE_HOUR;
	Customer CUSTOMER{ "Customer1" , "010-111-1111" };
	Customer CUSTOMER_WITH_MAIL{ "Customer", "010-111-1111", "test@test.com" };


	const int UNDER_CAPACITY = 1;
	const int CAPA_PER_HOUR = 3;
	BookingScheduler bookingScheduler{ CAPA_PER_HOUR };
	TestableSmsSender testSmsSender;
	TestMailSender testableMailSender;
};

TEST_F(BookingItest, notOnTheHourReserve) {
	
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, 1, CUSTOMER };

	EXPECT_THROW({ bookingScheduler.addSchedule(schedule); }, std::runtime_error);
}

TEST_F(BookingItest, NormalTimeReserve) {

	Schedule* schedule = new Schedule{ ON_THE_HOUR, 1, CUSTOMER };
	
	bookingScheduler.addSchedule(schedule); // No Exception
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}

TEST_F(BookingItest, OverCapacitySameTime) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPA_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule); // No Exception

	try {
		Schedule* newSchedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
		bookingScheduler.addSchedule(newSchedule);
		FAIL();
	}
	catch (std::runtime_error& e)
	{
		EXPECT_EQ(string{ e.what() }, string{ "Number of people is over restaurant capacity per hour" });
	}

}

TEST_F(BookingItest, differentTime) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPA_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);

	tm differentHour = plus_hour(ON_THE_HOUR, 1);

	Schedule* newSchedule = new Schedule{ differentHour, UNDER_CAPACITY, CUSTOMER };
	bookingScheduler.addSchedule(newSchedule);

	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}

TEST_F(BookingItest, sendSMS) {
	
	Schedule* scheduler = new Schedule{ ON_THE_HOUR, CAPA_PER_HOUR, CUSTOMER };
	
	bookingScheduler.addSchedule(scheduler);
	EXPECT_EQ(true, testSmsSender.isSendMethodIsCalled());
}

TEST_F(BookingItest, notExistEmail) {

	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPA_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(0, testableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItest, ExistEmail) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPA_PER_HOUR, CUSTOMER_WITH_MAIL };

	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(1, testableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItest, Sunday) {

}

TEST_F(BookingItest, notSunday) {

}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
