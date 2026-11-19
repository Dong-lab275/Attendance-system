
INSERT INTO departments (department_id, department_name) VALUES
('D0001', 'Information Technology'),
('D0002', 'Computer Science'),
('D0003', 'Business Administration');


INSERT INTO users
(user_id, username, password, role, updated_at, is_active, created_at)
VALUES
('U00000000001', 'admin01', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'Administrator', NULL, TRUE, '2026-01-05 08:00:00'),
('U00000000002', 'lecturer01', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'Lecturer', NULL, TRUE, '2026-01-05 08:10:00'),
('U00000000003', 'lecturer02', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'Lecturer', NULL, TRUE, '2026-01-05 08:15:00'),
('U00000000004', 'student01', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'Student', NULL, TRUE, '2026-01-06 08:00:00'),
('U00000000005', 'student02', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'Student', NULL, TRUE, '2026-01-06 08:05:00'),
('U00000000006', 'student03', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'Student', NULL, TRUE, '2026-01-06 08:10:00');

INSERT INTO administrators (admin_id, phone, full_name, email, user_id) VALUES
('079080000001', '0900000001', 'System Administrator', 'admin@attendance.edu.vn', 'U00000000001');

INSERT INTO lecturers
(lecturer_id, phone, full_name, email, date_of_birth, gender, department_id, user_id)
VALUES
('079082001234', '0900000002', 'Nguyen Van An', 'an.nguyen@attendance.edu.vn', '1982-04-12', 'Male', 'D0001', 'U00000000002'),
('079185002345', '0900000003', 'Tran Thi Binh', 'binh.tran@attendance.edu.vn', '1985-09-23', 'Female', 'D0002', 'U00000000003');

INSERT INTO students
(student_id, address, full_name, date_of_birth, gender, phone, email, user_id)
VALUES
('079205003456', 'Thu Duc, Ho Chi Minh City', 'Le Minh Khang', '2005-02-14', 'Male', '0910000001', 'khang.le@student.edu.vn', 'U00000000004'),
('079305004567', 'District 7, Ho Chi Minh City', 'Pham Ngoc Mai', '2005-07-08', 'Female', '0910000002', 'mai.pham@student.edu.vn', 'U00000000005'),
('079204005678', 'Binh Thanh, Ho Chi Minh City', 'Vo Quoc Huy', '2004-11-19', 'Male', '0910000003', 'huy.vo@student.edu.vn', 'U00000000006');

INSERT INTO courses
(course_id, course_name, description, academic_year, semester, lecturer_id)
VALUES
('C0001', 'Database Systems', 'Relational database design and SQL practice.', '2026-2027', 1, '079082001234'),
('C0002', 'Software Engineering', 'Software development processes and system design.', '2026-2027', 1, '079185002345');

INSERT INTO course_registrations
(registration_id, registration_date, registration_status, student_id, course_id)
VALUES
('R00000000001', '2026-08-20 09:00:00', 'Approved', '079205003456', 'C0001'),
('R00000000002', '2026-08-20 09:05:00', 'Approved', '079305004567', 'C0001'),
('R00000000003', '2026-08-20 09:10:00', 'Approved', '079204005678', 'C0001'),
('R00000000004', '2026-08-21 10:00:00', 'Approved', '079205003456', 'C0002'),
('R00000000005', '2026-08-21 10:05:00', 'Approved', '079305004567', 'C0002');

INSERT INTO class_sessions
(session_id, class_date, start_time, end_time, classroom,
 attendance_start_time, attendance_end_time, latitude, longitude,
 allowed_radius, status, course_id)
VALUES
('SE001', '2026-09-07', '07:30:00', '09:30:00', 'A.301',
 '2026-09-07 07:15:00', '2026-09-07 07:45:00',
 10.7626220, 106.6601720, 100.00, 'Completed', 'C0001'),
('SE002', '2026-09-14', '07:30:00', '09:30:00', 'A.301',
 '2026-09-14 07:15:00', '2026-09-14 07:45:00',
 10.7626220, 106.6601720, 100.00, 'Completed', 'C0001'),
('SE003', '2026-09-09', '13:00:00', '15:00:00', 'B.204',
 '2026-09-09 12:45:00', '2026-09-09 13:15:00',
 10.7626220, 106.6601720, 100.00, 'Completed', 'C0002');

INSERT INTO attendance_records
(attendance_id, attendance_timestamp, status, notes, created_at, updated_at,
 check_in_latitude, check_in_longitude, student_id, session_id)
VALUES
('AT0000000001', '2026-09-07 07:25:00', 'Present', NULL, '2026-09-07 07:25:00', NULL, 10.7626300, 106.6601800, '079205003456', 'SE001'),
('AT0000000002', '2026-09-07 07:50:00', 'Late', 'Checked in after the attendance period.', '2026-09-07 07:50:00', NULL, 10.7626400, 106.6601900, '079305004567', 'SE001'),
('AT0000000003', NULL, 'Absent', 'No attendance check-in.', '2026-09-07 09:45:00', NULL, NULL, NULL, '079204005678', 'SE001'),
('AT0000000004', '2026-09-14 07:28:00', 'Present', NULL, '2026-09-14 07:28:00', NULL, 10.7626250, 106.6601750, '079205003456', 'SE002'),
('AT0000000005', '2026-09-14 07:29:00', 'Present', NULL, '2026-09-14 07:29:00', NULL, 10.7626260, 106.6601760, '079305004567', 'SE002'),
('AT0000000006', '2026-09-14 07:40:00', 'Late', 'Late check-in.', '2026-09-14 07:40:00', NULL, 10.7626270, 106.6601770, '079204005678', 'SE002'),
('AT0000000007', '2026-09-09 12:55:00', 'Present', NULL, '2026-09-09 12:55:00', NULL, 10.7626300, 106.6601800, '079205003456', 'SE003'),
('AT0000000008', NULL, 'Absent', 'No attendance check-in.', '2026-09-09 15:15:00', NULL, NULL, NULL, '079305004567', 'SE003');

INSERT INTO password_changes
(password_change_id, changed_at, old_password_hash, new_password_hash, user_id)
VALUES
('P00000000001', '2026-09-01 10:00:00', '277e7cabad25f80b389c140867be5b72b7ea1e4292f90e55db5757327b1c7f5b', '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'U00000000004');