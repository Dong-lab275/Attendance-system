--
-- PostgreSQL database dump
--

\restrict pH8gzLH1MwCEbrq4wtyLsZtbnJp2MZoWvefaauhcaAMXAQG5Jv4S6oD0i8B2Ukq

-- Dumped from database version 18.4
-- Dumped by pg_dump version 18.4

-- Started on 2026-10-30 20:09:45

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 228 (class 1259 OID 16860)
-- Name: AttendanceChangeHistory; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."AttendanceChangeHistory" (
    history_id integer NOT NULL,
    old_status character varying(20),
    new_status character varying(20),
    modification_timestamp timestamp without time zone,
    reason text,
    user_id integer,
    attendance_id integer
);


ALTER TABLE public."AttendanceChangeHistory" OWNER TO postgres;

--
-- TOC entry 227 (class 1259 OID 16840)
-- Name: AttendanceRecords; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."AttendanceRecords" (
    attendance_id integer NOT NULL,
    attendance_timestamp timestamp without time zone,
    status character varying(20),
    notes text,
    student_id character varying(12),
    session_id integer
);


ALTER TABLE public."AttendanceRecords" OWNER TO postgres;

--
-- TOC entry 226 (class 1259 OID 16829)
-- Name: ClassSessions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."ClassSessions" (
    session_id integer NOT NULL,
    class_date date,
    start_time time without time zone,
    end_time time without time zone,
    classroom character varying(50),
    course_id integer
);


ALTER TABLE public."ClassSessions" OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 16811)
-- Name: CourseRegistrations; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."CourseRegistrations" (
    registration_id integer NOT NULL,
    registration_date timestamp without time zone,
    registration_status character varying(20),
    student_id character varying(12),
    course_id integer
);


ALTER TABLE public."CourseRegistrations" OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 16798)
-- Name: Courses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Courses" (
    course_id integer NOT NULL,
    "courseCode" character varying(20),
    course_name character varying(100),
    description text,
    credits integer,
    academic_year character varying(20),
    semester integer,
    lecturer_id character varying(12)
);


ALTER TABLE public."Courses" OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16756)
-- Name: Departments; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Departments" (
    department_id integer NOT NULL,
    department_name character varying(100)
);


ALTER TABLE public."Departments" OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16780)
-- Name: Lecturers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Lecturers" (
    lecturer_id character varying(12) NOT NULL,
    phone character varying(15),
    "lecturerCode" character varying(20),
    full_name character varying(30),
    email character varying(40),
    date_of_birth date,
    gender character varying(10),
    department_id integer,
    user_id integer
);


ALTER TABLE public."Lecturers" OWNER TO postgres;

--
-- TOC entry 229 (class 1259 OID 16878)
-- Name: PasswordResetRequests; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."PasswordResetRequests" (
    reset_id integer NOT NULL,
    created_at timestamp without time zone,
    user_id integer
);


ALTER TABLE public."PasswordResetRequests" OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 16750)
-- Name: StudentClasses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."StudentClasses" (
    class_id character varying(12) NOT NULL,
    class_name character varying(100),
    academic_year character varying(20)
);


ALTER TABLE public."StudentClasses" OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 16762)
-- Name: Students; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Students" (
    student_id character varying(12) NOT NULL,
    address character varying(100),
    "studentCode" character varying(20),
    full_name character varying(30),
    date_of_birth date,
    gender character varying(10),
    phone character varying(15),
    email character varying(40),
    class_id character varying(12),
    user_id integer
);


ALTER TABLE public."Students" OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 16744)
-- Name: User; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."User" (
    user_id integer NOT NULL,
    username character varying(30),
    "passWord" character varying(20),
    role character varying(20),
    email character varying(40),
    is_active boolean,
    created_at timestamp without time zone
);


ALTER TABLE public."User" OWNER TO postgres;

--
-- TOC entry 5094 (class 0 OID 16860)
-- Dependencies: 228
-- Data for Name: AttendanceChangeHistory; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."AttendanceChangeHistory" (history_id, old_status, new_status, modification_timestamp, reason, user_id, attendance_id) FROM stdin;
1	Absent	Present	2026-10-01 10:00:00	Giảng viên xác nhận sinh viên đã tham gia buổi học nhưng trước đó bị ghi nhận nhầm là vắng mặt	2	4
2	Present	Late	2026-10-02 12:00:00	Giảng viên điều chỉnh trạng thái sau khi kiểm tra thời gian điểm danh thực tế	2	10
3	Absent	Present	2026-10-03 16:00:00	Giảng viên xác nhận sinh viên có tham gia buổi học sau khi kiểm tra minh chứng	3	20
4	Present	Absent	2026-10-09 12:30:00	Giảng viên điều chỉnh trạng thái do phát hiện dữ liệu điểm danh ban đầu không chính xác	2	13
\.


--
-- TOC entry 5093 (class 0 OID 16840)
-- Dependencies: 227
-- Data for Name: AttendanceRecords; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."AttendanceRecords" (attendance_id, attendance_timestamp, status, notes, student_id, session_id) FROM stdin;
1	2026-10-01 07:25:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000001	1
2	2026-10-01 07:48:00	Late	Sinh viên đến lớp sau thời gian bắt đầu buổi học	079205000002	1
3	2026-10-01 07:28:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000003	1
4	2026-10-01 09:00:00	Present	Giảng viên xác nhận sinh viên có tham gia buổi học	079205000004	1
5	2026-10-08 07:20:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000001	2
6	2026-10-08 07:35:00	Present	Sinh viên có mặt trong thời gian điểm danh	079205000002	2
7	\N	Absent	Sinh viên không tham gia buổi học	079205000003	2
8	2026-10-08 07:55:00	Late	Sinh viên đến lớp muộn	079205000004	2
9	2026-10-02 09:40:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000001	3
10	2026-10-02 10:05:00	Late	Sinh viên đến lớp sau thời gian quy định	079205000002	3
11	2026-10-02 09:35:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000005	3
12	\N	Absent	Sinh viên không tham gia buổi học	079205000006	3
13	\N	Absent	Sinh viên không tham gia buổi học	079205000001	4
14	\N	Absent	Sinh viên không tham gia buổi học	079205000002	4
15	2026-10-09 10:00:00	Late	Sinh viên đến lớp muộn	079205000005	4
16	2026-10-09 09:42:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000006	4
17	2026-10-03 12:55:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000003	5
18	2026-10-03 13:20:00	Late	Sinh viên đến lớp muộn	079205000004	5
19	2026-10-03 12:58:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000007	5
20	2026-10-03 14:00:00	Present	Giảng viên xác nhận sinh viên có tham gia buổi học	079205000008	5
21	2026-10-10 12:55:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000003	6
22	\N	Absent	Sinh viên không tham gia buổi học	079205000004	6
23	2026-10-10 13:25:00	Late	Sinh viên đến lớp muộn	079205000007	6
24	2026-10-10 12:58:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000008	6
25	2026-10-04 07:25:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000001	7
26	2026-10-04 07:55:00	Late	Sinh viên đến lớp muộn	079205000005	7
27	\N	Absent	Sinh viên không tham gia buổi học	079205000007	7
28	2026-10-04 07:28:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000008	7
29	2026-10-11 07:25:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000001	8
30	2026-10-11 07:28:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000005	8
31	2026-10-11 07:55:00	Late	Sinh viên đến lớp muộn	079205000007	8
32	\N	Absent	Sinh viên không tham gia buổi học	079205000008	8
33	2026-10-05 13:25:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000002	9
34	2026-10-05 14:00:00	Late	Sinh viên đến lớp muộn	079205000003	9
35	\N	Absent	Sinh viên không tham gia buổi học	079205000006	9
36	2026-10-05 13:28:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000008	9
37	2026-10-12 13:25:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000002	10
38	2026-10-12 13:55:00	Late	Sinh viên đến lớp muộn	079205000003	10
39	2026-10-12 13:28:00	Present	Sinh viên có mặt trước giờ bắt đầu buổi học	079205000006	10
40	\N	Absent	Sinh viên không tham gia buổi học	079205000008	10
\.


--
-- TOC entry 5092 (class 0 OID 16829)
-- Dependencies: 226
-- Data for Name: ClassSessions; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."ClassSessions" (session_id, class_date, start_time, end_time, classroom, course_id) FROM stdin;
1	2026-10-01	07:30:00	09:30:00	Phòng A101	1
2	2026-10-08	07:30:00	09:30:00	Phòng A101	1
3	2026-10-02	09:45:00	11:45:00	Phòng B202	2
4	2026-10-09	09:45:00	11:45:00	Phòng B202	2
5	2026-10-03	13:00:00	15:00:00	Phòng C303	3
6	2026-10-10	13:00:00	15:00:00	Phòng C303	3
7	2026-10-04	07:30:00	10:30:00	Phòng D404	4
8	2026-10-11	07:30:00	10:30:00	Phòng D404	4
9	2026-10-05	13:30:00	15:30:00	Phòng E505	5
10	2026-10-12	13:30:00	15:30:00	Phòng E505	5
\.


--
-- TOC entry 5091 (class 0 OID 16811)
-- Dependencies: 225
-- Data for Name: CourseRegistrations; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."CourseRegistrations" (registration_id, registration_date, registration_status, student_id, course_id) FROM stdin;
1	2026-09-05 08:00:00	Registered	079205000001	1
2	2026-09-05 08:05:00	Registered	079205000002	1
3	2026-09-05 08:10:00	Registered	079205000003	1
4	2026-09-05 08:15:00	Registered	079205000004	1
5	2026-09-05 09:00:00	Registered	079205000001	2
6	2026-09-05 09:05:00	Registered	079205000002	2
7	2026-09-05 09:10:00	Registered	079205000005	2
8	2026-09-05 09:15:00	Registered	079205000006	2
9	2026-09-06 08:00:00	Registered	079205000003	3
10	2026-09-06 08:05:00	Registered	079205000004	3
11	2026-09-06 08:10:00	Registered	079205000007	3
12	2026-09-06 08:15:00	Registered	079205000008	3
13	2026-09-06 09:00:00	Registered	079205000001	4
14	2026-09-06 09:05:00	Registered	079205000005	4
15	2026-09-06 09:10:00	Registered	079205000007	4
16	2026-09-06 09:15:00	Registered	079205000008	4
17	2026-09-07 08:00:00	Registered	079205000002	5
18	2026-09-07 08:05:00	Registered	079205000003	5
19	2026-09-07 08:10:00	Registered	079205000006	5
20	2026-09-07 08:15:00	Registered	079205000008	5
\.


--
-- TOC entry 5090 (class 0 OID 16798)
-- Dependencies: 224
-- Data for Name: Courses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Courses" (course_id, "courseCode", course_name, description, credits, academic_year, semester, lecturer_id) FROM stdin;
1	CS101	Lập trình căn bản	Môn học cung cấp kiến thức cơ bản về ngôn ngữ lập trình, biến, kiểu dữ liệu, cấu trúc điều khiển, vòng lặp, hàm và tư duy giải quyết vấn đề bằng thuật toán.	3	2026-2027	1	079082000001
2	DB202	Cơ sở dữ liệu	Môn học cung cấp kiến thức về mô hình dữ liệu quan hệ, thiết kế cơ sở dữ liệu, khóa chính, khóa ngoại, ràng buộc dữ liệu và ngôn ngữ truy vấn SQL.	3	2026-2027	1	079082000001
3	SE303	Công nghệ phần mềm	Môn học giới thiệu quy trình phát triển phần mềm, thu thập yêu cầu, phân tích hệ thống, thiết kế phần mềm, kiểm thử và bảo trì phần mềm.	3	2026-2027	1	079085000002
4	WD404	Lập trình Web	Môn học cung cấp kiến thức về xây dựng giao diện và phát triển ứng dụng Web bằng HTML, CSS, JavaScript và các công nghệ Web liên quan.	4	2026-2027	1	079085000002
5	NT505	Mạng máy tính	Môn học cung cấp kiến thức về kiến trúc mạng máy tính, mô hình OSI, mô hình TCP/IP, địa chỉ IP, giao thức truyền thông và các nguyên lý hoạt động của mạng.	3	2026-2027	1	079080000003
\.


--
-- TOC entry 5087 (class 0 OID 16756)
-- Dependencies: 221
-- Data for Name: Departments; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Departments" (department_id, department_name) FROM stdin;
1	Khoa Công nghệ thông tin
2	Khoa Kỹ thuật phần mềm
3	Khoa Khoa học máy tính
\.


--
-- TOC entry 5089 (class 0 OID 16780)
-- Dependencies: 223
-- Data for Name: Lecturers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Lecturers" (lecturer_id, phone, "lecturerCode", full_name, email, date_of_birth, gender, department_id, user_id) FROM stdin;
079082000001	0912000001	GV001	Nguyễn Văn Thành An	nguyen.vanan@uth.edu.vn	1982-04-15	Male	1	2
079085000002	0912000002	GV002	Trần Thị Bích Ngọc	tran.thibichngoc@uth.edu.vn	1985-08-22	Female	2	3
079080000003	0912000003	GV003	Lê Hoàng Minh Cường	le.hoangminhcuong@uth.edu.vn	1980-02-10	Male	3	4
\.


--
-- TOC entry 5095 (class 0 OID 16878)
-- Dependencies: 229
-- Data for Name: PasswordResetRequests; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."PasswordResetRequests" (reset_id, created_at, user_id) FROM stdin;
1	2026-10-10 08:00:00	5
2	2026-10-11 09:00:00	6
3	2026-10-12 10:00:00	8
4	2026-10-13 14:00:00	2
5	2026-10-14 15:00:00	10
\.


--
-- TOC entry 5086 (class 0 OID 16750)
-- Dependencies: 220
-- Data for Name: StudentClasses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."StudentClasses" (class_id, class_name, academic_year) FROM stdin;
012012210506	Công nghệ thông tin K22A	2026-2027
012012210507	Công nghệ thông tin K22B	2026-2027
012012220501	Kỹ thuật phần mềm K22A	2026-2027
\.


--
-- TOC entry 5088 (class 0 OID 16762)
-- Dependencies: 222
-- Data for Name: Students; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Students" (student_id, address, "studentCode", full_name, date_of_birth, gender, phone, email, class_id, user_id) FROM stdin;
079205000001	12 Nguyễn Trãi, Quận 1, Thành phố Hồ Chí Minh	SV001	Nguyễn Thị Minh Anh	2005-03-12	Female	0901000001	nguyen.thiminhanh@student.uth.edu.vn	012012210506	5
079205000002	25 Lê Lợi, Quận 3, Thành phố Hồ Chí Minh	SV002	Trần Văn Hoàng Nam	2005-07-20	Male	0901000002	tran.vanhoangnam@student.uth.edu.vn	012012210506	6
079205000003	8 Võ Văn Tần, Quận 5, Thành phố Hồ Chí Minh	SV003	Lê Nguyễn Thảo My	2005-11-05	Female	0901000003	le.nguyenthaomy@student.uth.edu.vn	012012210506	7
079205000004	41 Cách Mạng Tháng 8, Quận 10, Thành phố Hồ Chí Minh	SV004	Phạm Minh Quốc Huy	2004-09-18	Male	0901000004	pham.minhquochuy@student.uth.edu.vn	012012210506	8
079205000005	15 Nguyễn Huệ, Quận Bình Thạnh, Thành phố Hồ Chí Minh	SV005	Võ Thị Gia Hân	2005-01-25	Female	0901000005	vo.thigiahAn@student.uth.edu.vn	012012210507	9
079205000006	30 Điện Biên Phủ, Quận Phú Nhuận, Thành phố Hồ Chí Minh	SV006	Đặng Nguyễn Đức Thành	2004-12-10	Male	0901000006	dang.nguyenducthanh@student.uth.edu.vn	012012210507	10
079205000007	55 Lý Thường Kiệt, Thành phố Thủ Đức, Thành phố Hồ Chí Minh	SV007	Bùi Thị Bảo Ngọc	2005-05-16	Female	0901000007	bui.thibaongoc@student.uth.edu.vn	012012220501	11
079205000008	19 Trần Hưng Đạo, Quận Gò Vấp, Thành phố Hồ Chí Minh	SV008	Nguyễn Hoàng Tuấn Kiệt	2005-08-30	Male	0901000008	nguyen.hoangtuanKiet@student.uth.edu.vn	012012220501	12
\.


--
-- TOC entry 5085 (class 0 OID 16744)
-- Dependencies: 219
-- Data for Name: User; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."User" (user_id, username, "passWord", role, email, is_active, created_at) FROM stdin;
1	Nguyễn Thành Đạt	Admin@123	Administrator	nguyen.thanhdat@uth.edu.vn	t	2026-09-01 08:00:00
2	Nguyễn Văn Thành An	An@123456	Lecturer	nguyen.vanan@uth.edu.vn	t	2026-09-01 08:10:00
3	Trần Thị Bích Ngọc	BichNgoc@123	Lecturer	tran.thibichngoc@uth.edu.vn	t	2026-09-01 08:15:00
4	Lê Hoàng Minh Cường	Cuong@123456	Lecturer	le.hoangminhcuong@uth.edu.vn	t	2026-09-01 08:20:00
5	Nguyễn Thị Minh Anh	Anh@123456	Student	nguyen.thiminhanh@student.uth.edu.vn	t	2026-09-02 08:00:00
6	Trần Văn Hoàng Nam	Nam@123456	Student	tran.vanhoangnam@student.uth.edu.vn	t	2026-09-02 08:05:00
7	Lê Nguyễn Thảo My	My@123456	Student	le.nguyenthaomy@student.uth.edu.vn	t	2026-09-02 08:10:00
8	Phạm Minh Quốc Huy	Huy@123456	Student	pham.minhquochuy@student.uth.edu.vn	t	2026-09-02 08:15:00
9	Võ Thị Gia Hân	Han@123456	Student	vo.thigiahAn@student.uth.edu.vn	t	2026-09-02 08:20:00
10	Đặng Nguyễn Đức Thành	Thanh@123456	Student	dang.nguyenducthanh@student.uth.edu.vn	t	2026-09-02 08:25:00
11	Bùi Thị Bảo Ngọc	Ngoc@123456	Student	bui.thibaongoc@student.uth.edu.vn	t	2026-09-02 08:30:00
12	Nguyễn Hoàng Tuấn Kiệt	Kiet@123456	Student	nguyen.hoangtuanKiet@student.uth.edu.vn	t	2026-09-02 08:35:00
\.


--
-- TOC entry 4922 (class 2606 OID 16867)
-- Name: AttendanceChangeHistory AttendanceChangeHistory_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceChangeHistory"
    ADD CONSTRAINT "AttendanceChangeHistory_pkey" PRIMARY KEY (history_id);


--
-- TOC entry 4918 (class 2606 OID 16847)
-- Name: AttendanceRecords AttendanceRecords_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_pkey" PRIMARY KEY (attendance_id);


--
-- TOC entry 4920 (class 2606 OID 16849)
-- Name: AttendanceRecords AttendanceRecords_student_id_session_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_student_id_session_id_key" UNIQUE (student_id, session_id);


--
-- TOC entry 4916 (class 2606 OID 16834)
-- Name: ClassSessions ClassSessions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."ClassSessions"
    ADD CONSTRAINT "ClassSessions_pkey" PRIMARY KEY (session_id);


--
-- TOC entry 4912 (class 2606 OID 16816)
-- Name: CourseRegistrations CourseRegistrations_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_pkey" PRIMARY KEY (registration_id);


--
-- TOC entry 4914 (class 2606 OID 16818)
-- Name: CourseRegistrations CourseRegistrations_student_id_course_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_student_id_course_id_key" UNIQUE (student_id, course_id);


--
-- TOC entry 4910 (class 2606 OID 16805)
-- Name: Courses Courses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Courses"
    ADD CONSTRAINT "Courses_pkey" PRIMARY KEY (course_id);


--
-- TOC entry 4900 (class 2606 OID 16761)
-- Name: Departments Departments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Departments"
    ADD CONSTRAINT "Departments_pkey" PRIMARY KEY (department_id);


--
-- TOC entry 4906 (class 2606 OID 16785)
-- Name: Lecturers Lecturers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_pkey" PRIMARY KEY (lecturer_id);


--
-- TOC entry 4908 (class 2606 OID 16787)
-- Name: Lecturers Lecturers_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_user_id_key" UNIQUE (user_id);


--
-- TOC entry 4924 (class 2606 OID 16883)
-- Name: PasswordResetRequests PasswordResetRequests_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."PasswordResetRequests"
    ADD CONSTRAINT "PasswordResetRequests_pkey" PRIMARY KEY (reset_id);


--
-- TOC entry 4898 (class 2606 OID 16755)
-- Name: StudentClasses StudentClasses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."StudentClasses"
    ADD CONSTRAINT "StudentClasses_pkey" PRIMARY KEY (class_id);


--
-- TOC entry 4902 (class 2606 OID 16767)
-- Name: Students Students_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_pkey" PRIMARY KEY (student_id);


--
-- TOC entry 4904 (class 2606 OID 16769)
-- Name: Students Students_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_user_id_key" UNIQUE (user_id);


--
-- TOC entry 4896 (class 2606 OID 16749)
-- Name: User User_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."User"
    ADD CONSTRAINT "User_pkey" PRIMARY KEY (user_id);


--
-- TOC entry 4935 (class 2606 OID 16873)
-- Name: AttendanceChangeHistory AttendanceChangeHistory_attendance_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceChangeHistory"
    ADD CONSTRAINT "AttendanceChangeHistory_attendance_id_fkey" FOREIGN KEY (attendance_id) REFERENCES public."AttendanceRecords"(attendance_id);


--
-- TOC entry 4936 (class 2606 OID 16868)
-- Name: AttendanceChangeHistory AttendanceChangeHistory_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceChangeHistory"
    ADD CONSTRAINT "AttendanceChangeHistory_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 4933 (class 2606 OID 16855)
-- Name: AttendanceRecords AttendanceRecords_session_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_session_id_fkey" FOREIGN KEY (session_id) REFERENCES public."ClassSessions"(session_id);


--
-- TOC entry 4934 (class 2606 OID 16850)
-- Name: AttendanceRecords AttendanceRecords_student_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_student_id_fkey" FOREIGN KEY (student_id) REFERENCES public."Students"(student_id);


--
-- TOC entry 4932 (class 2606 OID 16835)
-- Name: ClassSessions ClassSessions_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."ClassSessions"
    ADD CONSTRAINT "ClassSessions_course_id_fkey" FOREIGN KEY (course_id) REFERENCES public."Courses"(course_id);


--
-- TOC entry 4930 (class 2606 OID 16824)
-- Name: CourseRegistrations CourseRegistrations_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_course_id_fkey" FOREIGN KEY (course_id) REFERENCES public."Courses"(course_id);


--
-- TOC entry 4931 (class 2606 OID 16819)
-- Name: CourseRegistrations CourseRegistrations_student_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_student_id_fkey" FOREIGN KEY (student_id) REFERENCES public."Students"(student_id);


--
-- TOC entry 4929 (class 2606 OID 16806)
-- Name: Courses Courses_lecturer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Courses"
    ADD CONSTRAINT "Courses_lecturer_id_fkey" FOREIGN KEY (lecturer_id) REFERENCES public."Lecturers"(lecturer_id);


--
-- TOC entry 4927 (class 2606 OID 16788)
-- Name: Lecturers Lecturers_department_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_department_id_fkey" FOREIGN KEY (department_id) REFERENCES public."Departments"(department_id);


--
-- TOC entry 4928 (class 2606 OID 16793)
-- Name: Lecturers Lecturers_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 4937 (class 2606 OID 16884)
-- Name: PasswordResetRequests PasswordResetRequests_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."PasswordResetRequests"
    ADD CONSTRAINT "PasswordResetRequests_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 4925 (class 2606 OID 16770)
-- Name: Students Students_class_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_class_id_fkey" FOREIGN KEY (class_id) REFERENCES public."StudentClasses"(class_id);


--
-- TOC entry 4926 (class 2606 OID 16775)
-- Name: Students Students_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


-- Completed on 2026-10-30 20:09:45

--
-- PostgreSQL database dump complete
--

\unrestrict pH8gzLH1MwCEbrq4wtyLsZtbnJp2MZoWvefaauhcaAMXAQG5Jv4S6oD0i8B2Ukq

