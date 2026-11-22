#include <gtest/gtest.h>
#include <string>

#include "GpsValidator.h"
#include "Student.h"
#include "Course.h"
#include "StudentController.h"

TEST(GpsValidatorTest, CalculatesDistanceAndRadius) {
    EXPECT_NEAR(GpsValidator::calculateDistance(10.8000, 106.7000, 10.8000, 106.7000), 0.0, 1e-6);
    EXPECT_TRUE(GpsValidator::isWithinRadius(10.8001, 106.7001, 10.8000, 106.7000, 100.0));
    EXPECT_FALSE(GpsValidator::isWithinRadius(10.8100, 106.7100, 10.8000, 106.7000, 100.0));
    EXPECT_FALSE(GpsValidator::isWithinRadius(10.8000, 106.7000, 10.8000, 106.7000, -1.0));
}

TEST(RoleWorkflowTest, StudentCourseAndLecturerSessionCheckIn) {
    Student s("ST001", "Nguyen Minh Anh", "minhanh.nguyen@university.edu.vn", "0900000000");
    Course c("PHI205", "Philosophy");
    StudentController ctrl;

    EXPECT_EQ(s.getStudentId(), "ST001");
    EXPECT_EQ(c.getCourseId(), "PHI205");

    // Giảng viên mở phiên ("Open") và sinh viên đủ điều kiện điểm danh
    EXPECT_TRUE(ctrl.canCheckIn("Open"));
    EXPECT_TRUE(ctrl.validateCheckIn(true, "Open", true, true, false));

    // Khi phiên đóng/kết thúc hoặc chưa đăng ký hoặc trùng điểm danh -> từ chối
    EXPECT_FALSE(ctrl.canCheckIn("Closed"));
    EXPECT_FALSE(ctrl.validateCheckIn(true, "Closed", true, true, false));
    EXPECT_FALSE(ctrl.validateCheckIn(false, "Open", true, true, false));
    EXPECT_FALSE(ctrl.validateCheckIn(true, "Open", true, true, true));

    // Tỷ lệ chuyên cần của sinh viên
    EXPECT_NEAR(ctrl.calculateAttendanceRate(8, 10), 80.0, 1e-6);
    EXPECT_NEAR(ctrl.calculateAttendanceRate(11, 10), 0.0, 1e-6);
}