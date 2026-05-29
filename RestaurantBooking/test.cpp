#include "gmock/gmock.h"
#include "booking_scheduler.cpp"
using namespace testing;
class BookingItest : public Test {
public:
	tm getTime(int year, int month, int day, int hour, int min)
	{
		tm result = { 0, min, hour, day, month, year - 1900, 0, 0 , -1 };

		mktime(&result);
		return result;
	}
};

TEST_F(BookingItest, notOnTheHourReserve) {
	tm notOnTheHour =getTime (2021,3,26,9,5);

	Customer customer{ "Customer1" , "010-111-1111" };
	Schedule* schedule = new Schedule{ notOnTheHour, 1, customer };
	BookingScheduler bookingScheduler{ 3 };

	EXPECT_THROW({ bookingScheduler.addSchedule(schedule); }, std::runtime_error);
}

TEST_F(BookingItest, NormalTimeReserve) {

	tm onTheHour = getTime(2021, 3, 26, 9, 0);

	Customer customer{ "Customer1" , "010-111-1111" };
	Schedule* schedule = new Schedule{ onTheHour, 1, customer };
	BookingScheduler bookingScheduler{ 3 };

	bookingScheduler.addSchedule(schedule); // No Exception
	
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}
//
//TEST(BookingSchedulerTest, 시간대별인원제한이있다같은시간대에Capacity초과할경우예외발생) {
//
//}
//
//TEST(BookingSchedulerTest, 시간대별인원제한이있다같은시간대가다르면Capacity차있어도스케쥴추가성공) {
//
//}
//
//TEST(BookingSchedulerTest, 예약완료시SMS는무조건발송) {
//
//}
//
//TEST(BookingSchedulerTest, 이메일이없는경우에는이메일미발송) {
//
//}
//
//TEST(BookingSchedulerTest, 이메일이있는경우에는이메일발송) {
//
//}
//
//TEST(BookingSchedulerTest, 현재날짜가일요일인경우예약불가예외처리) {
//
//}
//
//TEST(BookingSchedulerTest, 현재날짜가일요일이아닌경우예약가능) {
//
//}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
