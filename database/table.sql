CREATE TABLE Student (
    student_id VARCHAR(20) PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    date_of_birth DATE,
    gender VARCHAR(10),
    address TEXT,
    phone_number VARCHAR(15),
    email_address VARCHAR(100),
    class_name VARCHAR(50)
);