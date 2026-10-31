--
-- PostgreSQL database dump
--

\restrict fCftaiyonsL6iYBy8e82l9p6KN7I3QXXf4WS7OKEYzDtwscmNweAeBOuoaq7ejj

-- Dumped from database version 18.4
-- Dumped by pg_dump version 18.4

-- Started on 2026-10-31 13:57:34

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
-- TOC entry 228 (class 1259 OID 17006)
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
-- TOC entry 227 (class 1259 OID 16986)
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
-- TOC entry 226 (class 1259 OID 16975)
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
-- TOC entry 225 (class 1259 OID 16957)
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
-- TOC entry 224 (class 1259 OID 16944)
-- Name: Courses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Courses" (
    course_id integer NOT NULL,
    course_name character varying(100),
    "courseCode" character varying(50),
    description text,
    credits integer,
    academic_year character varying(20),
    semester integer,
    lecturer_id character varying(12)
);


ALTER TABLE public."Courses" OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16902)
-- Name: Departments; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Departments" (
    department_id integer NOT NULL,
    department_name character varying(100)
);


ALTER TABLE public."Departments" OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16926)
-- Name: Lecturers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Lecturers" (
    lecturer_id character varying(12) NOT NULL,
    phone character varying(15),
    full_name character varying(30),
    email character varying(40),
    date_of_birth date,
    gender character varying(10),
    department_id integer,
    user_id integer
);


ALTER TABLE public."Lecturers" OWNER TO postgres;

--
-- TOC entry 229 (class 1259 OID 17024)
-- Name: PasswordResetRequests; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."PasswordResetRequests" (
    reset_id integer NOT NULL,
    created_at timestamp without time zone,
    user_id integer
);


ALTER TABLE public."PasswordResetRequests" OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 16896)
-- Name: StudentClasses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."StudentClasses" (
    class_id character varying(12) NOT NULL,
    class_name character varying(100),
    academic_year character varying(20)
);


ALTER TABLE public."StudentClasses" OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 16908)
-- Name: Students; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Students" (
    student_id character varying(12) NOT NULL,
    address character varying(100),
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
-- TOC entry 219 (class 1259 OID 16890)
-- Name: User; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."User" (
    user_id integer NOT NULL,
    username character varying(30),
    "passWord" character varying(255),
    role character varying(20),
    email character varying(40),
    is_active boolean,
    created_at timestamp without time zone
);


ALTER TABLE public."User" OWNER TO postgres;

--
-- TOC entry 246 (class 1259 OID 17214)
-- Name: attendancechangehistory; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.attendancechangehistory (
    history_id integer NOT NULL,
    old_status character varying(20),
    new_status character varying(20) NOT NULL,
    modification_timestamp timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    reason text,
    user_id character(12) NOT NULL,
    attendance_id integer NOT NULL
);


ALTER TABLE public.attendancechangehistory OWNER TO postgres;

--
-- TOC entry 245 (class 1259 OID 17213)
-- Name: attendancechangehistory_history_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.attendancechangehistory ALTER COLUMN history_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.attendancechangehistory_history_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 244 (class 1259 OID 17189)
-- Name: attendancerecords; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.attendancerecords (
    attendance_id integer NOT NULL,
    attendance_timestamp timestamp without time zone,
    status character varying(20) NOT NULL,
    notes text,
    student_id character(12) NOT NULL,
    session_id integer NOT NULL,
    CONSTRAINT chk_attendance_status CHECK (((status)::text = ANY ((ARRAY['Present'::character varying, 'Absent'::character varying, 'Late'::character varying])::text[])))
);


ALTER TABLE public.attendancerecords OWNER TO postgres;

--
-- TOC entry 243 (class 1259 OID 17188)
-- Name: attendancerecords_attendance_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.attendancerecords ALTER COLUMN attendance_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.attendancerecords_attendance_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 242 (class 1259 OID 17171)
-- Name: classsessions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.classsessions (
    session_id integer NOT NULL,
    class_date date NOT NULL,
    start_time time without time zone NOT NULL,
    end_time time without time zone NOT NULL,
    classroom character varying(50) NOT NULL,
    course_id character(5) NOT NULL,
    CONSTRAINT chk_session_time CHECK ((end_time > start_time))
);


ALTER TABLE public.classsessions OWNER TO postgres;

--
-- TOC entry 241 (class 1259 OID 17170)
-- Name: classsessions_session_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.classsessions ALTER COLUMN session_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.classsessions_session_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 240 (class 1259 OID 17147)
-- Name: courseregistrations; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.courseregistrations (
    registration_id integer NOT NULL,
    registration_date timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    registration_status character varying(20) NOT NULL,
    student_id character(12) NOT NULL,
    course_id character(5) NOT NULL
);


ALTER TABLE public.courseregistrations OWNER TO postgres;

--
-- TOC entry 239 (class 1259 OID 17146)
-- Name: courseregistrations_registration_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.courseregistrations ALTER COLUMN registration_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.courseregistrations_registration_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 238 (class 1259 OID 17127)
-- Name: courses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.courses (
    course_id character(5) NOT NULL,
    course_code character varying(20) NOT NULL,
    course_name character varying(100) NOT NULL,
    description text,
    credits integer NOT NULL,
    academic_year character varying(20) NOT NULL,
    semester integer NOT NULL,
    lecturer_id character(12) NOT NULL
);


ALTER TABLE public.courses OWNER TO postgres;

--
-- TOC entry 235 (class 1259 OID 17078)
-- Name: departments; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.departments (
    department_id integer NOT NULL,
    department_name character varying(100) NOT NULL
);


ALTER TABLE public.departments OWNER TO postgres;

--
-- TOC entry 234 (class 1259 OID 17077)
-- Name: departments_department_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.departments ALTER COLUMN department_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.departments_department_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 237 (class 1259 OID 17106)
-- Name: lecturers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.lecturers (
    lecturer_id character(12) NOT NULL,
    phone character varying(15),
    full_name character varying(100) NOT NULL,
    email character varying(100),
    date_of_birth date,
    gender character varying(10),
    department_id integer NOT NULL,
    user_id character(12) NOT NULL
);


ALTER TABLE public.lecturers OWNER TO postgres;

--
-- TOC entry 233 (class 1259 OID 17063)
-- Name: passwordresetrequests; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.passwordresetrequests (
    reset_id integer NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    user_id character(12) NOT NULL
);


ALTER TABLE public.passwordresetrequests OWNER TO postgres;

--
-- TOC entry 232 (class 1259 OID 17062)
-- Name: passwordresetrequests_reset_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.passwordresetrequests ALTER COLUMN reset_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.passwordresetrequests_reset_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 230 (class 1259 OID 17035)
-- Name: studentclasses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.studentclasses (
    class_id character(5) NOT NULL,
    class_name character varying(100) NOT NULL,
    academic_year character varying(20) NOT NULL
);


ALTER TABLE public.studentclasses OWNER TO postgres;

--
-- TOC entry 236 (class 1259 OID 17085)
-- Name: students; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.students (
    student_id character(12) NOT NULL,
    address character varying(100),
    full_name character varying(100) NOT NULL,
    date_of_birth date,
    gender character varying(10),
    phone character varying(15),
    email character varying(100),
    class_id character(5) NOT NULL,
    user_id character(12) NOT NULL
);


ALTER TABLE public.students OWNER TO postgres;

--
-- TOC entry 231 (class 1259 OID 17043)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    user_id character(12) NOT NULL,
    username character varying(100) NOT NULL,
    password character varying(255) NOT NULL,
    role character varying(20) NOT NULL,
    email character varying(100) NOT NULL,
    is_active boolean DEFAULT true NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    CONSTRAINT chk_user_role CHECK (((role)::text = ANY ((ARRAY['Administrator'::character varying, 'Student'::character varying, 'Lecturer'::character varying])::text[])))
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 5199 (class 0 OID 17006)
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
-- TOC entry 5198 (class 0 OID 16986)
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
-- TOC entry 5197 (class 0 OID 16975)
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
-- TOC entry 5196 (class 0 OID 16957)
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
-- TOC entry 5195 (class 0 OID 16944)
-- Dependencies: 224
-- Data for Name: Courses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Courses" (course_id, course_name, "courseCode", description, credits, academic_year, semester, lecturer_id) FROM stdin;
1	Lập trình căn bản	CS101	Môn học cung cấp kiến thức cơ bản về ngôn ngữ lập trình, biến, kiểu dữ liệu, cấu trúc điều khiển, vòng lặp, hàm và tư duy giải quyết vấn đề bằng thuật toán.	3	2026-2027	1	079082000001
2	Cơ sở dữ liệu	DB202	Môn học cung cấp kiến thức về mô hình dữ liệu quan hệ, thiết kế cơ sở dữ liệu, khóa chính, khóa ngoại, ràng buộc dữ liệu và ngôn ngữ truy vấn SQL.	3	2026-2027	1	079082000001
3	Công nghệ phần mềm	SE303	Môn học giới thiệu quy trình phát triển phần mềm, thu thập yêu cầu, phân tích hệ thống, thiết kế phần mềm, kiểm thử và bảo trì phần mềm.	3	2026-2027	1	079085000002
4	Lập trình Web	WD404	Môn học cung cấp kiến thức về xây dựng giao diện và phát triển ứng dụng Web bằng HTML, CSS, JavaScript và các công nghệ Web liên quan.	4	2026-2027	1	079085000002
5	Mạng máy tính	NT505	Môn học cung cấp kiến thức về kiến trúc mạng máy tính, mô hình OSI, mô hình TCP/IP, địa chỉ IP, giao thức truyền thông và các nguyên lý hoạt động của mạng.	3	2026-2027	1	079080000003
\.


--
-- TOC entry 5192 (class 0 OID 16902)
-- Dependencies: 221
-- Data for Name: Departments; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Departments" (department_id, department_name) FROM stdin;
1	Khoa Công nghệ thông tin
2	Khoa Kỹ thuật phần mềm
3	Khoa Khoa học máy tính
\.


--
-- TOC entry 5194 (class 0 OID 16926)
-- Dependencies: 223
-- Data for Name: Lecturers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Lecturers" (lecturer_id, phone, full_name, email, date_of_birth, gender, department_id, user_id) FROM stdin;
079082000001	0912000001	Nguyễn Văn Thành An	nguyenvanan@uth.edu.vn	1982-04-15	Male	1	2
079085000002	0912000002	Trần Thị Bích Ngọc	tranthibichngoc@uth.edu.vn	1985-08-22	Female	2	3
079080000003	0912000003	Lê Hoàng Minh Cường	lehoangminhcuong@uth.edu.vn	1980-02-10	Male	3	4
\.


--
-- TOC entry 5200 (class 0 OID 17024)
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
-- TOC entry 5191 (class 0 OID 16896)
-- Dependencies: 220
-- Data for Name: StudentClasses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."StudentClasses" (class_id, class_name, academic_year) FROM stdin;
012012210506	Công nghệ thông tin K22A	2026-2027
012012210507	Công nghệ thông tin K22B	2026-2027
012012220501	Kỹ thuật phần mềm K22A	2026-2027
\.


--
-- TOC entry 5193 (class 0 OID 16908)
-- Dependencies: 222
-- Data for Name: Students; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Students" (student_id, address, full_name, date_of_birth, gender, phone, email, class_id, user_id) FROM stdin;
079205000001	12 Nguyễn Trãi, Quận 1, Thành phố Hồ Chí Minh	Nguyễn Thị Minh Anh	2005-03-12	Female	0901000001	nguyenthiminhanh@student.uth.edu.vn	012012210506	5
079205000002	25 Lê Lợi, Quận 3, Thành phố Hồ Chí Minh	Trần Văn Hoàng Nam	2005-07-20	Male	0901000002	tranvanhoangnam@student.uth.edu.vn	012012210506	6
079205000003	8 Võ Văn Tần, Quận 5, Thành phố Hồ Chí Minh	Lê Nguyễn Thảo My	2005-11-05	Female	0901000003	lenguyenthaomy@student.uth.edu.vn	012012210506	7
079205000004	41 Cách Mạng Tháng 8, Quận 10, Thành phố Hồ Chí Minh	Phạm Minh Quốc Huy	2004-09-18	Male	0901000004	phamminhquochuy@student.uth.edu.vn	012012210506	8
079205000005	15 Nguyễn Huệ, Quận Bình Thạnh, Thành phố Hồ Chí Minh	Võ Thị Gia Hân	2005-01-25	Female	0901000005	vothigiahAn@student.uth.edu.vn	012012210507	9
079205000006	30 Điện Biên Phủ, Quận Phú Nhuận, Thành phố Hồ Chí Minh	Đặng Nguyễn Đức Thành	2004-12-10	Male	0901000006	dangnguyenducthanh@student.uth.edu.vn	012012210507	10
079205000007	55 Lý Thường Kiệt, Thành phố Thủ Đức, Thành phố Hồ Chí Minh	Bùi Thị Bảo Ngọc	2005-05-16	Female	0901000007	buithibaongoc@student.uth.edu.vn	012012220501	11
079205000008	19 Trần Hưng Đạo, Quận Gò Vấp, Thành phố Hồ Chí Minh	Nguyễn Hoàng Tuấn Kiệt	2005-08-30	Male	0901000008	nguyenhoangtuanKiet@student.uth.edu.vn	012012220501	12
\.


--
-- TOC entry 5190 (class 0 OID 16890)
-- Dependencies: 219
-- Data for Name: User; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."User" (user_id, username, "passWord", role, email, is_active, created_at) FROM stdin;
1	Nguyễn Thành Đạt	Admin@123	Administrator	nguyenthanhdat@uth.edu.vn	t	2026-09-01 08:00:00
2	Nguyễn Văn Thành An	An@123456	Lecturer	nguyenvanan@uth.edu.vn	t	2026-09-01 08:10:00
3	Trần Thị Bích Ngọc	BichNgoc@123	Lecturer	tranthibichngoc@uth.edu.vn	t	2026-09-01 08:15:00
4	Lê Hoàng Minh Cường	Cuong@123456	Lecturer	lehoangminhcuong@uth.edu.vn	t	2026-09-01 08:20:00
5	Nguyễn Thị Minh Anh	Anh@123456	Student	nguyenthiminhanh@student.uth.edu.vn	t	2026-09-02 08:00:00
6	Trần Văn Hoàng Nam	Nam@123456	Student	tranvanhoangnam@student.uth.edu.vn	t	2026-09-02 08:05:00
7	Lê Nguyễn Thảo My	My@123456	Student	lenguyenthaomy@student.uth.edu.vn	t	2026-09-02 08:10:00
8	Phạm Minh Quốc Huy	Huy@123456	Student	phamminhquochuy@student.uth.edu.vn	t	2026-09-02 08:15:00
9	Võ Thị Gia Hân	Han@123456	Student	vothigiahAn@student.uth.edu.vn	t	2026-09-02 08:20:00
10	Đặng Nguyễn Đức Thành	Thanh@123456	Student	dangnguyenducthanh@student.uth.edu.vn	t	2026-09-02 08:25:00
11	Bùi Thị Bảo Ngọc	Ngoc@123456	Student	buithibaongoc@student.uth.edu.vn	t	2026-09-02 08:30:00
12	Nguyễn Hoàng Tuấn Kiệt	Kiet@123456	Student	nguyenhoangtuanKiet@student.uth.edu.vn	t	2026-09-02 08:35:00
\.


--
-- TOC entry 5217 (class 0 OID 17214)
-- Dependencies: 246
-- Data for Name: attendancechangehistory; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.attendancechangehistory (history_id, old_status, new_status, modification_timestamp, reason, user_id, attendance_id) FROM stdin;
1	Absent	Present	2026-02-02 10:00:00	Giảng viên xác nhận sinh viên có mặt	079185000001	3
2	Present	Late	2026-02-09 10:30:00	Điều chỉnh trạng thái sau khi kiểm tra thời gian điểm danh	079185000001	4
3	Absent	Present	2026-02-16 10:00:00	Cập nhật kết quả điểm danh theo xác nhận của giảng viên	079185000001	8
\.


--
-- TOC entry 5215 (class 0 OID 17189)
-- Dependencies: 244
-- Data for Name: attendancerecords; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.attendancerecords (attendance_id, attendance_timestamp, status, notes, student_id, session_id) FROM stdin;
1	2026-02-02 07:25:00	Present	Có mặt đúng giờ	079205000001	1
2	2026-02-02 07:40:00	Late	Đến muộn 10 phút	079205000002	1
3	2026-02-02 07:30:00	Present	Được giảng viên xác nhận có mặt	079205000003	1
4	2026-02-09 07:20:00	Present	Có mặt đúng giờ	079205000001	2
5	2026-02-09 07:50:00	Late	Đến muộn 20 phút	079205000002	2
6	2026-02-09 07:45:00	Late	Đến muộn 15 phút	079205000003	2
7	2026-02-16 07:25:00	Present	Có mặt đúng giờ	079205000001	3
8	2026-02-16 07:35:00	Present	Được giảng viên xác nhận có mặt	079205000002	3
9	2026-02-03 09:40:00	Present	Có mặt đúng giờ	079205000001	4
10	2026-02-03 10:05:00	Late	Đến muộn 20 phút	079205000002	4
11	\N	Absent	Không tham gia buổi học	079205000003	5
12	2026-02-04 12:55:00	Present	Có mặt đúng giờ	079205000003	6
\.


--
-- TOC entry 5213 (class 0 OID 17171)
-- Dependencies: 242
-- Data for Name: classsessions; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.classsessions (session_id, class_date, start_time, end_time, classroom, course_id) FROM stdin;
1	2026-02-02	07:30:00	09:30:00	A101	C0001
2	2026-02-09	07:30:00	09:30:00	A101	C0001
3	2026-02-16	07:30:00	09:30:00	A101	C0001
4	2026-02-03	09:45:00	11:45:00	B202	C0002
5	2026-02-10	09:45:00	11:45:00	B202	C0002
6	2026-02-04	13:00:00	15:00:00	C303	C0003
\.


--
-- TOC entry 5211 (class 0 OID 17147)
-- Dependencies: 240
-- Data for Name: courseregistrations; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.courseregistrations (registration_id, registration_date, registration_status, student_id, course_id) FROM stdin;
1	2026-01-05 08:00:00	Approved	079205000001	C0001
2	2026-01-05 08:10:00	Approved	079205000002	C0001
3	2026-01-05 08:20:00	Approved	079205000003	C0001
4	2026-01-06 09:00:00	Approved	079205000001	C0002
5	2026-01-06 09:15:00	Approved	079205000002	C0002
6	2026-01-07 10:00:00	Approved	079205000003	C0003
\.


--
-- TOC entry 5209 (class 0 OID 17127)
-- Dependencies: 238
-- Data for Name: courses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.courses (course_id, course_code, course_name, description, credits, academic_year, semester, lecturer_id) FROM stdin;
C0001	IT101	Lập trình căn bản	Giới thiệu các kiến thức lập trình cơ bản bằng ngôn ngữ Java.	3	2025-2026	1	079185000001
C0002	IT102	Quản trị cơ sở dữ liệu	Tìm hiểu thiết kế cơ sở dữ liệu và ngôn ngữ truy vấn SQL.	3	2025-2026	1	079185000001
C0003	BA101	Quản trị kinh doanh	Cung cấp kiến thức cơ bản về quản lý và điều hành doanh nghiệp.	3	2025-2026	1	079188000002
\.


--
-- TOC entry 5206 (class 0 OID 17078)
-- Dependencies: 235
-- Data for Name: departments; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.departments (department_id, department_name) FROM stdin;
1	Khoa Công nghệ thông tin
2	Khoa Quản trị kinh doanh
\.


--
-- TOC entry 5208 (class 0 OID 17106)
-- Dependencies: 237
-- Data for Name: lecturers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.lecturers (lecturer_id, phone, full_name, email, date_of_birth, gender, department_id, user_id) FROM stdin;
079185000001	0912345678	Nguyễn Hoàng Văn Thành	nguyenhoangvanthanh@ut.edu.vn	1985-04-12	Nam	1	079185000001
079188000002	0923456789	Trần Nguyễn Thị Hương	trannguyenthihuong@ut.edu.vn	1988-09-25	Nữ	2	079188000002
\.


--
-- TOC entry 5204 (class 0 OID 17063)
-- Dependencies: 233
-- Data for Name: passwordresetrequests; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.passwordresetrequests (reset_id, created_at, user_id) FROM stdin;
1	2026-09-10 08:30:00	079205000001
2	2026-09-12 14:15:00	079205000002
\.


--
-- TOC entry 5201 (class 0 OID 17035)
-- Dependencies: 230
-- Data for Name: studentclasses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.studentclasses (class_id, class_name, academic_year) FROM stdin;
CTK01	Công nghệ thông tin 1	2025-2026
CTK02	Công nghệ thông tin 2	2025-2026
QTKD1	Quản trị kinh doanh 1	2025-2026
\.


--
-- TOC entry 5207 (class 0 OID 17085)
-- Dependencies: 236
-- Data for Name: students; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.students (student_id, address, full_name, date_of_birth, gender, phone, email, class_id, user_id) FROM stdin;
079205000001	Quận 1, TP. Hồ Chí Minh	Nguyễn Thị Minh Anh	2005-03-15	Nữ	0901234567	nguyenthiminhanh@ut.edu.vn	CTK01	079205000001
079205000002	Quận 3, TP. Hồ Chí Minh	Trần Thị Ngọc Ánh	2005-07-20	Nữ	0902345678	tranthingocanh@ut.edu.vn	CTK02	079205000002
079205000003	Quận 7, TP. Hồ Chí Minh	Lê Nguyễn Hoàng Nam	2004-11-05	Nam	0903456789	lenguyenhoangnam@ut.edu.vn	QTKD1	079205000003
\.


--
-- TOC entry 5202 (class 0 OID 17043)
-- Dependencies: 231
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (user_id, username, password, role, email, is_active, created_at) FROM stdin;
079200000001	admin01	admin123	Administrator	quantri@ut.edu.vn	t	2026-10-31 13:53:38.478305
079205000001	minhanh	student123	Student	nguyenthiminhanh@ut.edu.vn	t	2026-10-31 13:53:38.478305
079205000002	ngocanh	student123	Student	tranthingocanh@ut.edu.vn	t	2026-10-31 13:53:38.478305
079205000003	hoangnam	student123	Student	lenguyenhoangnam@ut.edu.vn	t	2026-10-31 13:53:38.478305
079185000001	vanthanh	lecturer123	Lecturer	nguyenhoangvanthanh@ut.edu.vn	t	2026-10-31 13:53:38.478305
079188000002	thihuong	lecturer123	Lecturer	trannguyenthihuong@ut.edu.vn	t	2026-10-31 13:53:38.478305
\.


--
-- TOC entry 5223 (class 0 OID 0)
-- Dependencies: 245
-- Name: attendancechangehistory_history_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.attendancechangehistory_history_id_seq', 3, true);


--
-- TOC entry 5224 (class 0 OID 0)
-- Dependencies: 243
-- Name: attendancerecords_attendance_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.attendancerecords_attendance_id_seq', 12, true);


--
-- TOC entry 5225 (class 0 OID 0)
-- Dependencies: 241
-- Name: classsessions_session_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.classsessions_session_id_seq', 6, true);


--
-- TOC entry 5226 (class 0 OID 0)
-- Dependencies: 239
-- Name: courseregistrations_registration_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.courseregistrations_registration_id_seq', 6, true);


--
-- TOC entry 5227 (class 0 OID 0)
-- Dependencies: 234
-- Name: departments_department_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.departments_department_id_seq', 1, false);


--
-- TOC entry 5228 (class 0 OID 0)
-- Dependencies: 232
-- Name: passwordresetrequests_reset_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.passwordresetrequests_reset_id_seq', 2, true);


--
-- TOC entry 4980 (class 2606 OID 17013)
-- Name: AttendanceChangeHistory AttendanceChangeHistory_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceChangeHistory"
    ADD CONSTRAINT "AttendanceChangeHistory_pkey" PRIMARY KEY (history_id);


--
-- TOC entry 4976 (class 2606 OID 16993)
-- Name: AttendanceRecords AttendanceRecords_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_pkey" PRIMARY KEY (attendance_id);


--
-- TOC entry 4978 (class 2606 OID 16995)
-- Name: AttendanceRecords AttendanceRecords_student_id_session_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_student_id_session_id_key" UNIQUE (student_id, session_id);


--
-- TOC entry 4974 (class 2606 OID 16980)
-- Name: ClassSessions ClassSessions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."ClassSessions"
    ADD CONSTRAINT "ClassSessions_pkey" PRIMARY KEY (session_id);


--
-- TOC entry 4970 (class 2606 OID 16962)
-- Name: CourseRegistrations CourseRegistrations_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_pkey" PRIMARY KEY (registration_id);


--
-- TOC entry 4972 (class 2606 OID 16964)
-- Name: CourseRegistrations CourseRegistrations_student_id_course_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_student_id_course_id_key" UNIQUE (student_id, course_id);


--
-- TOC entry 4968 (class 2606 OID 16951)
-- Name: Courses Courses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Courses"
    ADD CONSTRAINT "Courses_pkey" PRIMARY KEY (course_id);


--
-- TOC entry 4958 (class 2606 OID 16907)
-- Name: Departments Departments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Departments"
    ADD CONSTRAINT "Departments_pkey" PRIMARY KEY (department_id);


--
-- TOC entry 4964 (class 2606 OID 16931)
-- Name: Lecturers Lecturers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_pkey" PRIMARY KEY (lecturer_id);


--
-- TOC entry 4966 (class 2606 OID 16933)
-- Name: Lecturers Lecturers_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_user_id_key" UNIQUE (user_id);


--
-- TOC entry 4982 (class 2606 OID 17029)
-- Name: PasswordResetRequests PasswordResetRequests_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."PasswordResetRequests"
    ADD CONSTRAINT "PasswordResetRequests_pkey" PRIMARY KEY (reset_id);


--
-- TOC entry 4956 (class 2606 OID 16901)
-- Name: StudentClasses StudentClasses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."StudentClasses"
    ADD CONSTRAINT "StudentClasses_pkey" PRIMARY KEY (class_id);


--
-- TOC entry 4960 (class 2606 OID 16913)
-- Name: Students Students_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_pkey" PRIMARY KEY (student_id);


--
-- TOC entry 4962 (class 2606 OID 16915)
-- Name: Students Students_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_user_id_key" UNIQUE (user_id);


--
-- TOC entry 4954 (class 2606 OID 16895)
-- Name: User User_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."User"
    ADD CONSTRAINT "User_pkey" PRIMARY KEY (user_id);


--
-- TOC entry 5016 (class 2606 OID 17226)
-- Name: attendancechangehistory attendancechangehistory_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancechangehistory
    ADD CONSTRAINT attendancechangehistory_pkey PRIMARY KEY (history_id);


--
-- TOC entry 5012 (class 2606 OID 17200)
-- Name: attendancerecords attendancerecords_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancerecords
    ADD CONSTRAINT attendancerecords_pkey PRIMARY KEY (attendance_id);


--
-- TOC entry 5010 (class 2606 OID 17182)
-- Name: classsessions classsessions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.classsessions
    ADD CONSTRAINT classsessions_pkey PRIMARY KEY (session_id);


--
-- TOC entry 5006 (class 2606 OID 17157)
-- Name: courseregistrations courseregistrations_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courseregistrations
    ADD CONSTRAINT courseregistrations_pkey PRIMARY KEY (registration_id);


--
-- TOC entry 5004 (class 2606 OID 17140)
-- Name: courses courses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courses
    ADD CONSTRAINT courses_pkey PRIMARY KEY (course_id);


--
-- TOC entry 4994 (class 2606 OID 17084)
-- Name: departments departments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.departments
    ADD CONSTRAINT departments_pkey PRIMARY KEY (department_id);


--
-- TOC entry 5000 (class 2606 OID 17114)
-- Name: lecturers lecturers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.lecturers
    ADD CONSTRAINT lecturers_pkey PRIMARY KEY (lecturer_id);


--
-- TOC entry 5002 (class 2606 OID 17116)
-- Name: lecturers lecturers_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.lecturers
    ADD CONSTRAINT lecturers_user_id_key UNIQUE (user_id);


--
-- TOC entry 4992 (class 2606 OID 17071)
-- Name: passwordresetrequests passwordresetrequests_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.passwordresetrequests
    ADD CONSTRAINT passwordresetrequests_pkey PRIMARY KEY (reset_id);


--
-- TOC entry 4984 (class 2606 OID 17042)
-- Name: studentclasses studentclasses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.studentclasses
    ADD CONSTRAINT studentclasses_pkey PRIMARY KEY (class_id);


--
-- TOC entry 4996 (class 2606 OID 17093)
-- Name: students students_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.students
    ADD CONSTRAINT students_pkey PRIMARY KEY (student_id);


--
-- TOC entry 4998 (class 2606 OID 17095)
-- Name: students students_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.students
    ADD CONSTRAINT students_user_id_key UNIQUE (user_id);


--
-- TOC entry 5008 (class 2606 OID 17159)
-- Name: courseregistrations uq_student_course; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courseregistrations
    ADD CONSTRAINT uq_student_course UNIQUE (student_id, course_id);


--
-- TOC entry 5014 (class 2606 OID 17202)
-- Name: attendancerecords uq_student_session; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancerecords
    ADD CONSTRAINT uq_student_session UNIQUE (student_id, session_id);


--
-- TOC entry 4986 (class 2606 OID 17061)
-- Name: users users_email_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_email_key UNIQUE (email);


--
-- TOC entry 4988 (class 2606 OID 17057)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);


--
-- TOC entry 4990 (class 2606 OID 17059)
-- Name: users users_username_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_username_key UNIQUE (username);


--
-- TOC entry 5027 (class 2606 OID 17019)
-- Name: AttendanceChangeHistory AttendanceChangeHistory_attendance_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceChangeHistory"
    ADD CONSTRAINT "AttendanceChangeHistory_attendance_id_fkey" FOREIGN KEY (attendance_id) REFERENCES public."AttendanceRecords"(attendance_id);


--
-- TOC entry 5028 (class 2606 OID 17014)
-- Name: AttendanceChangeHistory AttendanceChangeHistory_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceChangeHistory"
    ADD CONSTRAINT "AttendanceChangeHistory_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 5025 (class 2606 OID 17001)
-- Name: AttendanceRecords AttendanceRecords_session_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_session_id_fkey" FOREIGN KEY (session_id) REFERENCES public."ClassSessions"(session_id);


--
-- TOC entry 5026 (class 2606 OID 16996)
-- Name: AttendanceRecords AttendanceRecords_student_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."AttendanceRecords"
    ADD CONSTRAINT "AttendanceRecords_student_id_fkey" FOREIGN KEY (student_id) REFERENCES public."Students"(student_id);


--
-- TOC entry 5024 (class 2606 OID 16981)
-- Name: ClassSessions ClassSessions_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."ClassSessions"
    ADD CONSTRAINT "ClassSessions_course_id_fkey" FOREIGN KEY (course_id) REFERENCES public."Courses"(course_id);


--
-- TOC entry 5022 (class 2606 OID 16970)
-- Name: CourseRegistrations CourseRegistrations_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_course_id_fkey" FOREIGN KEY (course_id) REFERENCES public."Courses"(course_id);


--
-- TOC entry 5023 (class 2606 OID 16965)
-- Name: CourseRegistrations CourseRegistrations_student_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."CourseRegistrations"
    ADD CONSTRAINT "CourseRegistrations_student_id_fkey" FOREIGN KEY (student_id) REFERENCES public."Students"(student_id);


--
-- TOC entry 5021 (class 2606 OID 16952)
-- Name: Courses Courses_lecturer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Courses"
    ADD CONSTRAINT "Courses_lecturer_id_fkey" FOREIGN KEY (lecturer_id) REFERENCES public."Lecturers"(lecturer_id);


--
-- TOC entry 5019 (class 2606 OID 16934)
-- Name: Lecturers Lecturers_department_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_department_id_fkey" FOREIGN KEY (department_id) REFERENCES public."Departments"(department_id);


--
-- TOC entry 5020 (class 2606 OID 16939)
-- Name: Lecturers Lecturers_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Lecturers"
    ADD CONSTRAINT "Lecturers_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 5029 (class 2606 OID 17030)
-- Name: PasswordResetRequests PasswordResetRequests_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."PasswordResetRequests"
    ADD CONSTRAINT "PasswordResetRequests_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 5017 (class 2606 OID 16916)
-- Name: Students Students_class_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_class_id_fkey" FOREIGN KEY (class_id) REFERENCES public."StudentClasses"(class_id);


--
-- TOC entry 5018 (class 2606 OID 16921)
-- Name: Students Students_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Students"
    ADD CONSTRAINT "Students_user_id_fkey" FOREIGN KEY (user_id) REFERENCES public."User"(user_id);


--
-- TOC entry 5039 (class 2606 OID 17208)
-- Name: attendancerecords fk_attendance_session; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancerecords
    ADD CONSTRAINT fk_attendance_session FOREIGN KEY (session_id) REFERENCES public.classsessions(session_id);


--
-- TOC entry 5040 (class 2606 OID 17203)
-- Name: attendancerecords fk_attendance_student; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancerecords
    ADD CONSTRAINT fk_attendance_student FOREIGN KEY (student_id) REFERENCES public.students(student_id);


--
-- TOC entry 5035 (class 2606 OID 17141)
-- Name: courses fk_courses_lecturer; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courses
    ADD CONSTRAINT fk_courses_lecturer FOREIGN KEY (lecturer_id) REFERENCES public.lecturers(lecturer_id);


--
-- TOC entry 5041 (class 2606 OID 17232)
-- Name: attendancechangehistory fk_history_attendance; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancechangehistory
    ADD CONSTRAINT fk_history_attendance FOREIGN KEY (attendance_id) REFERENCES public.attendancerecords(attendance_id);


--
-- TOC entry 5042 (class 2606 OID 17227)
-- Name: attendancechangehistory fk_history_user; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.attendancechangehistory
    ADD CONSTRAINT fk_history_user FOREIGN KEY (user_id) REFERENCES public.users(user_id);


--
-- TOC entry 5033 (class 2606 OID 17117)
-- Name: lecturers fk_lecturers_department; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.lecturers
    ADD CONSTRAINT fk_lecturers_department FOREIGN KEY (department_id) REFERENCES public.departments(department_id);


--
-- TOC entry 5034 (class 2606 OID 17122)
-- Name: lecturers fk_lecturers_user; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.lecturers
    ADD CONSTRAINT fk_lecturers_user FOREIGN KEY (user_id) REFERENCES public.users(user_id);


--
-- TOC entry 5030 (class 2606 OID 17072)
-- Name: passwordresetrequests fk_passwordreset_user; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.passwordresetrequests
    ADD CONSTRAINT fk_passwordreset_user FOREIGN KEY (user_id) REFERENCES public.users(user_id);


--
-- TOC entry 5036 (class 2606 OID 17165)
-- Name: courseregistrations fk_registration_course; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courseregistrations
    ADD CONSTRAINT fk_registration_course FOREIGN KEY (course_id) REFERENCES public.courses(course_id);


--
-- TOC entry 5037 (class 2606 OID 17160)
-- Name: courseregistrations fk_registration_student; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courseregistrations
    ADD CONSTRAINT fk_registration_student FOREIGN KEY (student_id) REFERENCES public.students(student_id);


--
-- TOC entry 5038 (class 2606 OID 17183)
-- Name: classsessions fk_sessions_course; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.classsessions
    ADD CONSTRAINT fk_sessions_course FOREIGN KEY (course_id) REFERENCES public.courses(course_id);


--
-- TOC entry 5031 (class 2606 OID 17096)
-- Name: students fk_students_class; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.students
    ADD CONSTRAINT fk_students_class FOREIGN KEY (class_id) REFERENCES public.studentclasses(class_id);


--
-- TOC entry 5032 (class 2606 OID 17101)
-- Name: students fk_students_user; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.students
    ADD CONSTRAINT fk_students_user FOREIGN KEY (user_id) REFERENCES public.users(user_id);


-- Completed on 2026-10-31 13:57:35

--
-- PostgreSQL database dump complete
--

\unrestrict fCftaiyonsL6iYBy8e82l9p6KN7I3QXXf4WS7OKEYzDtwscmNweAeBOuoaq7ejj

