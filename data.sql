
INSERT INTO "User" (user_id, username, "passWord", role, email, is_active, created_at) VALUES
    (1, 'Nguyễn Thành Đạt', 'Admin@123', 'Administrator', 'nguyen.thanhdat@uth.edu.vn', TRUE, '2026-09-01 08:00:00'),
    (2, 'Nguyễn Văn Thành An', 'An@123456', 'Lecturer', 'nguyen.vanan@uth.edu.vn', TRUE, '2026-09-01 08:10:00'),
    (3, 'Trần Thị Bích Ngọc', 'BichNgoc@123', 'Lecturer', 'tran.thibichngoc@uth.edu.vn', TRUE, '2026-09-01 08:15:00'),
    (4, 'Lê Hoàng Minh Cường', 'Cuong@123456', 'Lecturer', 'le.hoangminhcuong@uth.edu.vn', TRUE, '2026-09-01 08:20:00'),
    (5, 'Nguyễn Thị Minh Anh', 'Anh@123456', 'Student', 'nguyen.thiminhanh@student.uth.edu.vn', TRUE, '2026-09-02 08:00:00'),
    (6, 'Trần Văn Hoàng Nam', 'Nam@123456', 'Student', 'tran.vanhoangnam@student.uth.edu.vn', TRUE, '2026-09-02 08:05:00'),
    (7, 'Lê Nguyễn Thảo My', 'My@123456', 'Student', 'le.nguyenthaomy@student.uth.edu.vn', TRUE, '2026-09-02 08:10:00'),
    (8, 'Phạm Minh Quốc Huy', 'Huy@123456', 'Student', 'pham.minhquochuy@student.uth.edu.vn', TRUE, '2026-09-02 08:15:00'),
    (9, 'Võ Thị Gia Hân', 'Han@123456', 'Student', 'vo.thigiahAn@student.uth.edu.vn', TRUE, '2026-09-02 08:20:00'),
    (10, 'Đặng Nguyễn Đức Thành', 'Thanh@123456', 'Student', 'dang.nguyenducthanh@student.uth.edu.vn', TRUE, '2026-09-02 08:25:00'),
    (11, 'Bùi Thị Bảo Ngọc', 'Ngoc@123456', 'Student', 'bui.thibaongoc@student.uth.edu.vn', TRUE, '2026-09-02 08:30:00'),
    (12, 'Nguyễn Hoàng Tuấn Kiệt', 'Kiet@123456', 'Student', 'nguyen.hoangtuanKiet@student.uth.edu.vn', TRUE, '2026-09-02 08:35:00');


INSERT INTO "StudentClasses" (class_id, class_name, academic_year) VALUES
    ('012012210506', 'Công nghệ thông tin K22A', '2026-2027'),
    ('012012210507', 'Công nghệ thông tin K22B', '2026-2027'),
    ('012012220501', 'Kỹ thuật phần mềm K22A', '2026-2027');


INSERT INTO "Departments" (department_id, department_name) VALUES
    (1, 'Khoa Công nghệ thông tin'),
    (2, 'Khoa Kỹ thuật phần mềm'),
    (3, 'Khoa Khoa học máy tính');


INSERT INTO "Students" (student_id, address, "studentCode", full_name, date_of_birth, gender, phone, email, class_id, user_id) VALUES
    ('079205000001', '12 Nguyễn Trãi, Quận 1, Thành phố Hồ Chí Minh', 'SV001', 'Nguyễn Thị Minh Anh', '2005-03-12', 'Female', '0901000001', 'nguyen.thiminhanh@student.uth.edu.vn', '012012210506', 5),
    ('079205000002', '25 Lê Lợi, Quận 3, Thành phố Hồ Chí Minh', 'SV002', 'Trần Văn Hoàng Nam', '2005-07-20', 'Male', '0901000002', 'tran.vanhoangnam@student.uth.edu.vn', '012012210506', 6),
    ('079205000003', '8 Võ Văn Tần, Quận 5, Thành phố Hồ Chí Minh', 'SV003', 'Lê Nguyễn Thảo My', '2005-11-05', 'Female', '0901000003', 'le.nguyenthaomy@student.uth.edu.vn', '012012210506', 7),
    ('079205000004', '41 Cách Mạng Tháng 8, Quận 10, Thành phố Hồ Chí Minh', 'SV004', 'Phạm Minh Quốc Huy', '2004-09-18', 'Male', '0901000004', 'pham.minhquochuy@student.uth.edu.vn', '012012210506', 8),
    ('079205000005', '15 Nguyễn Huệ, Quận Bình Thạnh, Thành phố Hồ Chí Minh', 'SV005', 'Võ Thị Gia Hân', '2005-01-25', 'Female', '0901000005', 'vo.thigiahAn@student.uth.edu.vn', '012012210507', 9),
    ('079205000006', '30 Điện Biên Phủ, Quận Phú Nhuận, Thành phố Hồ Chí Minh', 'SV006', 'Đặng Nguyễn Đức Thành', '2004-12-10', 'Male', '0901000006', 'dang.nguyenducthanh@student.uth.edu.vn', '012012210507', 10),
    ('079205000007', '55 Lý Thường Kiệt, Thành phố Thủ Đức, Thành phố Hồ Chí Minh', 'SV007', 'Bùi Thị Bảo Ngọc', '2005-05-16', 'Female', '0901000007', 'bui.thibaongoc@student.uth.edu.vn', '012012220501', 11),
    ('079205000008', '19 Trần Hưng Đạo, Quận Gò Vấp, Thành phố Hồ Chí Minh', 'SV008', 'Nguyễn Hoàng Tuấn Kiệt', '2005-08-30', 'Male', '0901000008', 'nguyen.hoangtuanKiet@student.uth.edu.vn', '012012220501', 12);


INSERT INTO "Lecturers" (lecturer_id, phone, "lecturerCode", full_name, email, date_of_birth, gender, department_id, user_id) VALUES
    ('079082000001', '0912000001', 'GV001', 'Nguyễn Văn Thành An', 'nguyen.vanan@uth.edu.vn', '1982-04-15', 'Male', 1, 2),
    ('079085000002', '0912000002', 'GV002', 'Trần Thị Bích Ngọc', 'tran.thibichngoc@uth.edu.vn', '1985-08-22', 'Female', 2, 3),
    ('079080000003', '0912000003', 'GV003', 'Lê Hoàng Minh Cường', 'le.hoangminhcuong@uth.edu.vn', '1980-02-10', 'Male', 3, 4);


INSERT INTO "Courses" (course_id, "courseCode", course_name, description, credits, academic_year, semester, lecturer_id) VALUES
    (1, 'CS101', 'Lập trình căn bản', 'Môn học cung cấp kiến thức cơ bản về ngôn ngữ lập trình, biến, kiểu dữ liệu, cấu trúc điều khiển, vòng lặp, hàm và tư duy giải quyết vấn đề bằng thuật toán.', 3, '2026-2027', 1, '079082000001'),
    (2, 'DB202', 'Cơ sở dữ liệu', 'Môn học cung cấp kiến thức về mô hình dữ liệu quan hệ, thiết kế cơ sở dữ liệu, khóa chính, khóa ngoại, ràng buộc dữ liệu và ngôn ngữ truy vấn SQL.', 3, '2026-2027', 1, '079082000001'),
    (3, 'SE303', 'Công nghệ phần mềm', 'Môn học giới thiệu quy trình phát triển phần mềm, thu thập yêu cầu, phân tích hệ thống, thiết kế phần mềm, kiểm thử và bảo trì phần mềm.', 3, '2026-2027', 1, '079085000002'),
    (4, 'WD404', 'Lập trình Web', 'Môn học cung cấp kiến thức về xây dựng giao diện và phát triển ứng dụng Web bằng HTML, CSS, JavaScript và các công nghệ Web liên quan.', 4, '2026-2027', 1, '079085000002'),
    (5, 'NT505', 'Mạng máy tính', 'Môn học cung cấp kiến thức về kiến trúc mạng máy tính, mô hình OSI, mô hình TCP/IP, địa chỉ IP, giao thức truyền thông và các nguyên lý hoạt động của mạng.', 3, '2026-2027', 1, '079080000003');


INSERT INTO "CourseRegistrations" (registration_id, registration_date, registration_status, student_id, course_id) VALUES
    (1, '2026-09-05 08:00:00', 'Registered', '079205000001', 1),
    (2, '2026-09-05 08:05:00', 'Registered', '079205000002', 1),
    (3, '2026-09-05 08:10:00', 'Registered', '079205000003', 1),
    (4, '2026-09-05 08:15:00', 'Registered', '079205000004', 1),
    (5, '2026-09-05 09:00:00', 'Registered', '079205000001', 2),
    (6, '2026-09-05 09:05:00', 'Registered', '079205000002', 2),
    (7, '2026-09-05 09:10:00', 'Registered', '079205000005', 2),
    (8, '2026-09-05 09:15:00', 'Registered', '079205000006', 2),
    (9, '2026-09-06 08:00:00', 'Registered', '079205000003', 3),
    (10, '2026-09-06 08:05:00', 'Registered', '079205000004', 3),
    (11, '2026-09-06 08:10:00', 'Registered', '079205000007', 3),
    (12, '2026-09-06 08:15:00', 'Registered', '079205000008', 3),
    (13, '2026-09-06 09:00:00', 'Registered', '079205000001', 4),
    (14, '2026-09-06 09:05:00', 'Registered', '079205000005', 4),
    (15, '2026-09-06 09:10:00', 'Registered', '079205000007', 4),
    (16, '2026-09-06 09:15:00', 'Registered', '079205000008', 4),
    (17, '2026-09-07 08:00:00', 'Registered', '079205000002', 5),
    (18, '2026-09-07 08:05:00', 'Registered', '079205000003', 5),
    (19, '2026-09-07 08:10:00', 'Registered', '079205000006', 5),
    (20, '2026-09-07 08:15:00', 'Registered', '079205000008', 5);


INSERT INTO "ClassSessions" (session_id, class_date, start_time, end_time, classroom, course_id) VALUES
    (1, '2026-10-01', '07:30:00', '09:30:00', 'Phòng A101', 1),
    (2, '2026-10-08', '07:30:00', '09:30:00', 'Phòng A101', 1),
    (3, '2026-10-02', '09:45:00', '11:45:00', 'Phòng B202', 2),
    (4, '2026-10-09', '09:45:00', '11:45:00', 'Phòng B202', 2),
    (5, '2026-10-03', '13:00:00', '15:00:00', 'Phòng C303', 3),
    (6, '2026-10-10', '13:00:00', '15:00:00', 'Phòng C303', 3),
    (7, '2026-10-04', '07:30:00', '10:30:00', 'Phòng D404', 4),
    (8, '2026-10-11', '07:30:00', '10:30:00', 'Phòng D404', 4),
    (9, '2026-10-05', '13:30:00', '15:30:00', 'Phòng E505', 5),
    (10, '2026-10-12', '13:30:00', '15:30:00', 'Phòng E505', 5);


INSERT INTO "AttendanceRecords" (attendance_id, attendance_timestamp, status, notes, student_id, session_id) VALUES
    (1, '2026-10-01 07:25:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000001', 1),
    (2, '2026-10-01 07:48:00', 'Late', 'Sinh viên đến lớp sau thời gian bắt đầu buổi học', '079205000002', 1),
    (3, '2026-10-01 07:28:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000003', 1),
    (4, '2026-10-01 09:00:00', 'Present', 'Giảng viên xác nhận sinh viên có tham gia buổi học', '079205000004', 1),
    (5, '2026-10-08 07:20:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000001', 2),
    (6, '2026-10-08 07:35:00', 'Present', 'Sinh viên có mặt trong thời gian điểm danh', '079205000002', 2),
    (7, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000003', 2),
    (8, '2026-10-08 07:55:00', 'Late', 'Sinh viên đến lớp muộn', '079205000004', 2),
    (9, '2026-10-02 09:40:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000001', 3),
    (10, '2026-10-02 10:05:00', 'Late', 'Sinh viên đến lớp sau thời gian quy định', '079205000002', 3),
    (11, '2026-10-02 09:35:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000005', 3),
    (12, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000006', 3),
    (13, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000001', 4),
    (14, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000002', 4),
    (15, '2026-10-09 10:00:00', 'Late', 'Sinh viên đến lớp muộn', '079205000005', 4),
    (16, '2026-10-09 09:42:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000006', 4),
    (17, '2026-10-03 12:55:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000003', 5),
    (18, '2026-10-03 13:20:00', 'Late', 'Sinh viên đến lớp muộn', '079205000004', 5),
    (19, '2026-10-03 12:58:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000007', 5),
    (20, '2026-10-03 14:00:00', 'Present', 'Giảng viên xác nhận sinh viên có tham gia buổi học', '079205000008', 5),
    (21, '2026-10-10 12:55:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000003', 6),
    (22, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000004', 6),
    (23, '2026-10-10 13:25:00', 'Late', 'Sinh viên đến lớp muộn', '079205000007', 6),
    (24, '2026-10-10 12:58:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000008', 6),
    (25, '2026-10-04 07:25:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000001', 7),
    (26, '2026-10-04 07:55:00', 'Late', 'Sinh viên đến lớp muộn', '079205000005', 7),
    (27, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000007', 7),
    (28, '2026-10-04 07:28:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000008', 7),
    (29, '2026-10-11 07:25:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000001', 8),
    (30, '2026-10-11 07:28:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000005', 8),
    (31, '2026-10-11 07:55:00', 'Late', 'Sinh viên đến lớp muộn', '079205000007', 8),
    (32, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000008', 8),
    (33, '2026-10-05 13:25:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000002', 9),
    (34, '2026-10-05 14:00:00', 'Late', 'Sinh viên đến lớp muộn', '079205000003', 9),
    (35, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000006', 9),
	(36, '2026-10-05 13:28:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000008', 9),
    (37, '2026-10-12 13:25:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000002', 10),
    (38, '2026-10-12 13:55:00', 'Late', 'Sinh viên đến lớp muộn', '079205000003', 10),
    (39, '2026-10-12 13:28:00', 'Present', 'Sinh viên có mặt trước giờ bắt đầu buổi học', '079205000006', 10),
    (40, NULL, 'Absent', 'Sinh viên không tham gia buổi học', '079205000008', 10);


INSERT INTO "AttendanceChangeHistory" (history_id, old_status, new_status, modification_timestamp, reason, user_id, attendance_id) VALUES
    (1, 'Absent', 'Present', '2026-10-01 10:00:00', 'Giảng viên xác nhận sinh viên đã tham gia buổi học nhưng trước đó bị ghi nhận nhầm là vắng mặt', 2, 4),
    (2, 'Present', 'Late', '2026-10-02 12:00:00', 'Giảng viên điều chỉnh trạng thái sau khi kiểm tra thời gian điểm danh thực tế', 2, 10),
    (3, 'Absent', 'Present', '2026-10-03 16:00:00', 'Giảng viên xác nhận sinh viên có tham gia buổi học sau khi kiểm tra minh chứng', 3, 20),
    (4, 'Present', 'Absent', '2026-10-09 12:30:00', 'Giảng viên điều chỉnh trạng thái do phát hiện dữ liệu điểm danh ban đầu không chính xác', 2, 13);


INSERT INTO "PasswordResetRequests" (reset_id, created_at, user_id) VALUES
    (1, '2026-10-10 08:00:00', 5),
    (2, '2026-10-11 09:00:00', 6),
    (3, '2026-10-12 10:00:00', 8),
    (4, '2026-10-13 14:00:00', 2),
    (5, '2026-10-14 15:00:00', 10);


SELECT 'User' AS table_name, COUNT(*) AS total FROM "User"
UNION ALL SELECT 'StudentClasses', COUNT(*) FROM "StudentClasses"
UNION ALL SELECT 'Departments', COUNT(*) FROM "Departments"
UNION ALL SELECT 'Students', COUNT(*) FROM "Students"
UNION ALL SELECT 'Lecturers', COUNT(*) FROM "Lecturers"
UNION ALL SELECT 'Courses', COUNT(*) FROM "Courses"
UNION ALL SELECT 'CourseRegistrations', COUNT(*) FROM "CourseRegistrations"
UNION ALL SELECT 'ClassSessions', COUNT(*) FROM "ClassSessions"
UNION ALL SELECT 'AttendanceRecords', COUNT(*) FROM "AttendanceRecords"
UNION ALL SELECT 'AttendanceChangeHistory', COUNT(*) FROM "AttendanceChangeHistory"