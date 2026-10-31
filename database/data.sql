

INSERT INTO studentclasses (class_id, class_name, academic_year)
VALUES
('CTK01', 'Công nghệ thông tin 1', '2025-2026'),
('CTK02', 'Công nghệ thông tin 2', '2025-2026'),
('QTKD1', 'Quản trị kinh doanh 1', '2025-2026');



INSERT INTO users (user_id, username, password, role, email, is_active)
VALUES
('079200000001', 'admin01', 'admin123', 'Administrator', 'quantri@ut.edu.vn', TRUE),
('079205000001', 'minhanh', 'student123', 'Student', 'nguyenthiminhanh@ut.edu.vn', TRUE),
('079205000002', 'ngocanh', 'student123', 'Student', 'tranthingocanh@ut.edu.vn', TRUE),
('079205000003', 'hoangnam', 'student123', 'Student', 'lenguyenhoangnam@ut.edu.vn', TRUE),
('079185000001', 'vanthanh', 'lecturer123', 'Lecturer', 'nguyenhoangvanthanh@ut.edu.vn', TRUE),
('079188000002', 'thihuong', 'lecturer123', 'Lecturer', 'trannguyenthihuong@ut.edu.vn', TRUE);



INSERT INTO passwordresetrequests (created_at, user_id)
VALUES
('2026-09-10 08:30:00', '079205000001'),
('2026-09-12 14:15:00', '079205000002');


INSERT INTO departments (department_id, department_name)
VALUES
(1, 'Khoa Công nghệ thông tin'),
(2, 'Khoa Quản trị kinh doanh');


INSERT INTO students
(student_id, address, full_name, date_of_birth, gender, phone, email, class_id, user_id)
VALUES
(
    '079205000001',
    'Quận 1, TP. Hồ Chí Minh',
    'Nguyễn Thị Minh Anh',
    '2005-03-15',
    'Nữ',
    '0901234567',
    'nguyenthiminhanh@ut.edu.vn',
    'CTK01',
    '079205000001'
),
(
    '079205000002',
    'Quận 3, TP. Hồ Chí Minh',
    'Trần Thị Ngọc Ánh',
    '2005-07-20',
    'Nữ',
    '0902345678',
    'tranthingocanh@ut.edu.vn',
    'CTK02',
    '079205000002'
),
(
    '079205000003',
    'Quận 7, TP. Hồ Chí Minh',
    'Lê Nguyễn Hoàng Nam',
    '2004-11-05',
    'Nam',
    '0903456789',
    'lenguyenhoangnam@ut.edu.vn',
    'QTKD1',
    '079205000003'
);



INSERT INTO lecturers
(lecturer_id, phone, full_name, email, date_of_birth, gender, department_id, user_id)
VALUES
(
    '079185000001',
    '0912345678',
    'Nguyễn Hoàng Văn Thành',
    'nguyenhoangvanthanh@ut.edu.vn',
    '1985-04-12',
    'Nam',
    1,
    '079185000001'
),
(
    '079188000002',
    '0923456789',
    'Trần Nguyễn Thị Hương',
    'trannguyenthihuong@ut.edu.vn',
    '1988-09-25',
    'Nữ',
    2,
    '079188000002'
);


INSERT INTO courses
(course_id, course_code, course_name, description, credits, academic_year, semester, lecturer_id)
VALUES
(
    'C0001',
    'IT101',
    'Lập trình căn bản',
    'Giới thiệu các kiến thức lập trình cơ bản bằng ngôn ngữ Java.',
    3,
    '2025-2026',
    1,
    '079185000001'
),
(
    'C0002',
    'IT102',
    'Quản trị cơ sở dữ liệu',
    'Tìm hiểu thiết kế cơ sở dữ liệu và ngôn ngữ truy vấn SQL.',
    3,
    '2025-2026',
    1,
    '079185000001'
),
(
    'C0003',
    'BA101',
    'Quản trị kinh doanh',
    'Cung cấp kiến thức cơ bản về quản lý và điều hành doanh nghiệp.',
    3,
    '2025-2026',
    1,
    '079188000002'
);


INSERT INTO courseregistrations
(registration_date, registration_status, student_id, course_id)
VALUES
('2026-01-05 08:00:00', 'Approved', '079205000001', 'C0001'),
('2026-01-05 08:10:00', 'Approved', '079205000002', 'C0001'),
('2026-01-05 08:20:00', 'Approved', '079205000003', 'C0001'),
('2026-01-06 09:00:00', 'Approved', '079205000001', 'C0002'),
('2026-01-06 09:15:00', 'Approved', '079205000002', 'C0002'),
('2026-01-07 10:00:00', 'Approved', '079205000003', 'C0003');


INSERT INTO classsessions
(class_date, start_time, end_time, classroom, course_id)
VALUES
('2026-02-02', '07:30:00', '09:30:00', 'A101', 'C0001'),
('2026-02-09', '07:30:00', '09:30:00', 'A101', 'C0001'),
('2026-02-16', '07:30:00', '09:30:00', 'A101', 'C0001'),
('2026-02-03', '09:45:00', '11:45:00', 'B202', 'C0002'),
('2026-02-10', '09:45:00', '11:45:00', 'B202', 'C0002'),
('2026-02-04', '13:00:00', '15:00:00', 'C303', 'C0003');


INSERT INTO attendancerecords
(attendance_timestamp, status, notes, student_id, session_id)
VALUES
('2026-02-02 07:25:00', 'Present', 'Có mặt đúng giờ', '079205000001', 1),
('2026-02-02 07:40:00', 'Late', 'Đến muộn 10 phút', '079205000002', 1),
('2026-02-02 07:30:00', 'Present', 'Được giảng viên xác nhận có mặt', '079205000003', 1),

('2026-02-09 07:20:00', 'Present', 'Có mặt đúng giờ', '079205000001', 2),
('2026-02-09 07:50:00', 'Late', 'Đến muộn 20 phút', '079205000002', 2),
('2026-02-09 07:45:00', 'Late', 'Đến muộn 15 phút', '079205000003', 2),

('2026-02-16 07:25:00', 'Present', 'Có mặt đúng giờ', '079205000001', 3),
('2026-02-16 07:35:00', 'Present', 'Được giảng viên xác nhận có mặt', '079205000002', 3),

('2026-02-03 09:40:00', 'Present', 'Có mặt đúng giờ', '079205000001', 4),
('2026-02-03 10:05:00', 'Late', 'Đến muộn 20 phút', '079205000002', 4),

(NULL, 'Absent', 'Không tham gia buổi học', '079205000003', 5),
('2026-02-04 12:55:00', 'Present', 'Có mặt đúng giờ', '079205000003', 6);


INSERT INTO attendancechangehistory
(old_status, new_status, modification_timestamp, reason, user_id, attendance_id)
VALUES
(
    'Absent',
    'Present',
    '2026-02-02 10:00:00',
    'Giảng viên xác nhận sinh viên có mặt',
    '079185000001',
    3
),
(
    'Present',
    'Late',
    '2026-02-09 10:30:00',
    'Điều chỉnh trạng thái sau khi kiểm tra thời gian điểm danh',
    '079185000001',
    4
),
(
    'Absent',
    'Present',
    '2026-02-16 10:00:00',
    'Cập nhật kết quả điểm danh theo xác nhận của giảng viên',
    '079185000001',
    8
);