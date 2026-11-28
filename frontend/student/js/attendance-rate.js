
document.addEventListener("DOMContentLoaded", () => {

    // =====================================
    // COURSE ATTENDANCE DATA
    // =====================================

    const courses = [
        {
            courseId: "CS201",
            courseName: "Cơ sở dữ liệu",
            sessions: 12,
            present: 10,
            late: 1,
            absent: 1
        },
        {
            courseId: "CS202",
            courseName: "Lập trình hướng đối tượng",
            sessions: 12,
            present: 9,
            late: 1,
            absent: 2
        },
        {
            courseId: "CS203",
            courseName: "Mạng máy tính",
            sessions: 10,
            present: 8,
            late: 1,
            absent: 1
        },
        {
            courseId: "CS204",
            courseName: "Cấu trúc dữ liệu và giải thuật",
            sessions: 10,
            present: 7,
            late: 1,
            absent: 2
        }
    ];

    // =====================================
    // DOM ELEMENTS
    // =====================================

    const courseFilter = document.getElementById("courseFilter");

    const registeredCourses =
        document.getElementById("registeredCourses");

    const registeredDescription =
        document.getElementById("registeredDescription");

    const completedSessions =
        document.getElementById("completedSessions");

    const sessionsAttended =
        document.getElementById("sessionsAttended");

    const attendanceTableBody =
        document.getElementById("attendanceTableBody");

    const emptyState =
        document.getElementById("emptyState");


    // =====================================
    // CALCULATE ATTENDANCE RATE
    // =====================================

    function calculateAttendanceRate(course) {

        if (course.sessions === 0) {
            return 0;
        }

        const attended = course.present + course.late;

        return (attended / course.sessions) * 100;
    }


    // =====================================
    // GET FILTERED COURSES
    // =====================================

    function getFilteredCourses() {

        const selectedCourse = courseFilter.value;

        if (selectedCourse === "all") {
            return courses;
        }

        return courses.filter(course =>
            course.courseId === selectedCourse
        );
    }


    // =====================================
    // UPDATE ATTENDANCE SUMMARY
    // =====================================

    function updateSummary(filteredCourses) {

        const totalCourses = filteredCourses.length;

        const totalSessions = filteredCourses.reduce(
            (total, course) => total + course.sessions,
            0
        );

        const totalPresent = filteredCourses.reduce(
            (total, course) => total + course.present,
            0
        );

        const totalLate = filteredCourses.reduce(
            (total, course) => total + course.late,
            0
        );

        const totalAttended = totalPresent + totalLate;

        registeredCourses.textContent = totalCourses;

        registeredDescription.textContent =
            totalCourses === 1
                ? "Course with attendance records"
                : "Courses with attendance records";

        completedSessions.textContent = totalSessions;

        sessionsAttended.textContent = totalAttended;

    }


    // =====================================
    // RENDER ATTENDANCE TABLE
    // =====================================

    function renderAttendanceTable(filteredCourses) {

        attendanceTableBody.innerHTML = "";

        if (filteredCourses.length === 0) {

            emptyState.hidden = false;

            return;
        }

        emptyState.hidden = true;

        filteredCourses.forEach(course => {

            const rate = calculateAttendanceRate(course);

            const row = document.createElement("tr");

            // Course name and ID
            const courseCell = document.createElement("td");

            courseCell.className = "course-name";

            courseCell.textContent =
                `${course.courseName} · ${course.courseId}`;

            // Sessions
            const sessionsCell = document.createElement("td");

            sessionsCell.textContent = course.sessions;

            // Present
            const presentCell = document.createElement("td");

            presentCell.textContent = course.present;

            // Late
            const lateCell = document.createElement("td");

            lateCell.textContent = course.late;

            // Absent
            const absentCell = document.createElement("td");

            absentCell.textContent = course.absent;

            // Attendance rate
            const rateCell = document.createElement("td");

            rateCell.className = "rate-value";

            rateCell.textContent = `${rate.toFixed(1)}%`;

            // Append cells
            row.appendChild(courseCell);
            row.appendChild(sessionsCell);
            row.appendChild(presentCell);
            row.appendChild(lateCell);
            row.appendChild(absentCell);
            row.appendChild(rateCell);

            attendanceTableBody.appendChild(row);

        });

    }


    // =====================================
    // UPDATE PAGE
    // =====================================

    function updateAttendancePage() {

        const filteredCourses = getFilteredCourses();

        updateSummary(filteredCourses);

        renderAttendanceTable(filteredCourses);

    }


    // =====================================
    // COURSE FILTER EVENT
    // =====================================

    courseFilter.addEventListener("change", () => {

        updateAttendancePage();

    });


    // =====================================
    // INITIALIZE PAGE
    // =====================================

    updateAttendancePage();

});
