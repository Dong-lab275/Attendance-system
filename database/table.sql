

CREATE TABLE IF NOT EXISTS users (
    user_id       CHAR(12) PRIMARY KEY,
    username      VARCHAR(100) NOT NULL UNIQUE,
    password      VARCHAR(255) NOT NULL,
    role          VARCHAR(20) NOT NULL,
    updated_at    TIMESTAMP,
    is_active     BOOLEAN NOT NULL DEFAULT TRUE,
    created_at    TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT chk_users_role
        CHECK (role IN ('Student', 'Lecturer', 'Administrator'))
);


CREATE TABLE IF NOT EXISTS departments (
    department_id   CHAR(5) PRIMARY KEY,
    department_name VARCHAR(100) NOT NULL UNIQUE
);


CREATE TABLE IF NOT EXISTS students (
    student_id    CHAR(12) PRIMARY KEY,
    address       VARCHAR(100),
    full_name     VARCHAR(100) NOT NULL,
    date_of_birth DATE,
    gender        VARCHAR(10),
    phone         VARCHAR(15),
    email         VARCHAR(100),
    user_id       CHAR(12) NOT NULL UNIQUE,

    CONSTRAINT fk_students_user
        FOREIGN KEY (user_id) REFERENCES users(user_id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS lecturers (
    lecturer_id   CHAR(12) PRIMARY KEY,
    phone         VARCHAR(15),
    full_name     VARCHAR(100) NOT NULL,
    email         VARCHAR(100),
    date_of_birth DATE,
    gender        VARCHAR(10),
    department_id CHAR(5) NOT NULL,
    user_id       CHAR(12) NOT NULL UNIQUE,

    CONSTRAINT fk_lecturers_department
        FOREIGN KEY (department_id) REFERENCES departments(department_id)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT fk_lecturers_user
        FOREIGN KEY (user_id) REFERENCES users(user_id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS administrators (
    admin_id    CHAR(12) PRIMARY KEY,
    phone       VARCHAR(15),
    full_name   VARCHAR(100) NOT NULL,
    email       VARCHAR(100),
    user_id     CHAR(12) NOT NULL UNIQUE,

    CONSTRAINT fk_administrators_user
        FOREIGN KEY (user_id) REFERENCES users(user_id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS password_changes (
    password_change_id CHAR(12) PRIMARY KEY,
    changed_at         TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    old_password_hash  VARCHAR(255) NOT NULL,
    new_password_hash  VARCHAR(255) NOT NULL,
    user_id            CHAR(12) NOT NULL,

    CONSTRAINT fk_password_changes_user
        FOREIGN KEY (user_id) REFERENCES users(user_id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS courses (
    course_id     CHAR(5) PRIMARY KEY,
    course_name   VARCHAR(100) NOT NULL,
    description   TEXT,
    academic_year VARCHAR(20),
    semester      INTEGER,
    lecturer_id   CHAR(12),

    CONSTRAINT chk_courses_semester
        CHECK (semester IS NULL OR semester > 0),
    CONSTRAINT fk_courses_lecturer
        FOREIGN KEY (lecturer_id) REFERENCES lecturers(lecturer_id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS course_registrations (
    registration_id     CHAR(12) PRIMARY KEY,
    registration_date   TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    registration_status VARCHAR(20),
    student_id          CHAR(12) NOT NULL,
    course_id           CHAR(5) NOT NULL,

    CONSTRAINT fk_course_registrations_student
        FOREIGN KEY (student_id) REFERENCES students(student_id)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT fk_course_registrations_course
        FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT uq_course_registrations_student_course
        UNIQUE (student_id, course_id)
);


CREATE TABLE IF NOT EXISTS class_sessions (
    session_id             CHAR(5) PRIMARY KEY,
    class_date             DATE NOT NULL,
    start_time             TIME NOT NULL,
    end_time               TIME NOT NULL,
    classroom              VARCHAR(50),
    attendance_start_time  TIMESTAMP,
    attendance_end_time    TIMESTAMP,
    latitude               NUMERIC(10, 7),
    longitude              NUMERIC(10, 7),
    allowed_radius         NUMERIC(10, 2),
    status                 VARCHAR(20),
    course_id              CHAR(5) NOT NULL,

    CONSTRAINT chk_class_sessions_time
        CHECK (end_time > start_time),
    CONSTRAINT chk_class_sessions_radius
        CHECK (allowed_radius IS NULL OR allowed_radius >= 0),
    CONSTRAINT fk_class_sessions_course
        FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS attendance_records (
    attendance_id       CHAR(12) PRIMARY KEY,
    attendance_timestamp TIMESTAMP,
    status              VARCHAR(20) NOT NULL,
    notes               TEXT,
    created_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at          TIMESTAMP,
    check_in_latitude   NUMERIC(10, 7),
    check_in_longitude  NUMERIC(10, 7),
    student_id          CHAR(12) NOT NULL,
    session_id          CHAR(5) NOT NULL,

    CONSTRAINT chk_attendance_records_status
        CHECK (status IN ('Present', 'Absent', 'Late')),
    CONSTRAINT fk_attendance_records_student
        FOREIGN KEY (student_id) REFERENCES students(student_id)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT fk_attendance_records_session
        FOREIGN KEY (session_id) REFERENCES class_sessions(session_id)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT uq_attendance_student_session
        UNIQUE (student_id, session_id)
);

<<<<<<< HEAD
CREATE SEQUENCE IF NOT EXISTS registration_id_seq
START WITH 1
INCREMENT BY 1;

CREATE INDEX IF NOT EXISTS idx_students_user_id
    ON students(user_id);

CREATE INDEX IF NOT EXISTS idx_lecturers_department_id
    ON lecturers(department_id);

CREATE INDEX IF NOT EXISTS idx_lecturers_user_id
    ON lecturers(user_id);

CREATE INDEX IF NOT EXISTS idx_courses_lecturer_id
    ON courses(lecturer_id);

CREATE INDEX IF NOT EXISTS idx_registrations_course_id
    ON course_registrations(course_id);

CREATE INDEX IF NOT EXISTS idx_class_sessions_course_id
    ON class_sessions(course_id);

CREATE INDEX IF NOT EXISTS idx_class_sessions_class_date
    ON class_sessions(class_date);

CREATE INDEX IF NOT EXISTS idx_attendance_student_id
    ON attendance_records(student_id);

CREATE INDEX IF NOT EXISTS idx_attendance_session_id
    ON attendance_records(session_id);

CREATE INDEX IF NOT EXISTS idx_password_changes_user_id
    ON password_changes(user_id);

=======
CREATE SEQUENCE IF NOT EXISTS registration_id_seq
START WITH 1
INCREMENT BY 1;

CREATE INDEX IF NOT EXISTS idx_students_user_id
    ON students(user_id);

CREATE INDEX IF NOT EXISTS idx_lecturers_department_id
    ON lecturers(department_id);

CREATE INDEX IF NOT EXISTS idx_lecturers_user_id
    ON lecturers(user_id);

CREATE INDEX IF NOT EXISTS idx_courses_lecturer_id
    ON courses(lecturer_id);

CREATE INDEX IF NOT EXISTS idx_registrations_course_id
    ON course_registrations(course_id);

>>>>>>> Đông
CREATE INDEX IF NOT EXISTS idx_class_sessions_course_id
    ON class_sessions(course_id);

CREATE INDEX IF NOT EXISTS idx_class_sessions_class_date
    ON class_sessions(class_date);

CREATE INDEX IF NOT EXISTS idx_attendance_student_id
    ON attendance_records(student_id);

CREATE INDEX IF NOT EXISTS idx_attendance_session_id
    ON attendance_records(session_id);

CREATE INDEX IF NOT EXISTS idx_password_changes_user_id
    ON password_changes(user_id);
