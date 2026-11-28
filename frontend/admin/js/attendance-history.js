
/* =====================================
   ATTENDANCE HISTORY DATA
   Synchronized with data.sql
===================================== */

const courses = {
    C0001: {
        courseName: "Database Systems"
    },

    C0002: {
        courseName: "Software Engineering"
    }
};

const attendanceRecords = [
    // SE001 - Database Systems - 07 Sep 2026
    {
        attendanceId: "AT0000000001",
        studentId: "079205003456",
        studentName: "Le Minh Khang",
        courseId: "C0001",
        sessionId: "SE001",
        session: 1,
        sessionDate: "2026-09-07",
        status: "Present",
        checkinTime: "07:25:00"
    },
    {
        attendanceId: "AT0000000002",
        studentId: "079305004567",
        studentName: "Pham Ngoc Mai",
        courseId: "C0001",
        sessionId: "SE001",
        session: 1,
        sessionDate: "2026-09-07",
        status: "Late",
        checkinTime: "07:50:00"
    },
    {
        attendanceId: "AT0000000003",
        studentId: "079204005678",
        studentName: "Vo Quoc Huy",
        courseId: "C0001",
        sessionId: "SE001",
        session: 1,
        sessionDate: "2026-09-07",
        status: "Absent",
        checkinTime: null
    },

    // SE002 - Database Systems - 14 Sep 2026
    {
        attendanceId: "AT0000000004",
        studentId: "079205003456",
        studentName: "Le Minh Khang",
        courseId: "C0001",
        sessionId: "SE002",
        session: 2,
        sessionDate: "2026-09-14",
        status: "Present",
        checkinTime: "07:28:00"
    },
    {
        attendanceId: "AT0000000005",
        studentId: "079305004567",
        studentName: "Pham Ngoc Mai",
        courseId: "C0001",
        sessionId: "SE002",
        session: 2,
        sessionDate: "2026-09-14",
        status: "Present",
        checkinTime: "07:29:00"
    },
    {
        attendanceId: "AT0000000006",
        studentId: "079204005678",
        studentName: "Vo Quoc Huy",
        courseId: "C0001",
        sessionId: "SE002",
        session: 2,
        sessionDate: "2026-09-14",
        status: "Late",
        checkinTime: "07:40:00"
    },

    // SE003 - Software Engineering - 09 Sep 2026
    {
        attendanceId: "AT0000000007",
        studentId: "079205003456",
        studentName: "Le Minh Khang",
        courseId: "C0002",
        sessionId: "SE003",
        session: 3,
        sessionDate: "2026-09-09",
        status: "Present",
        checkinTime: "12:55:00"
    },
    {
        attendanceId: "AT0000000008",
        studentId: "079305004567",
        studentName: "Pham Ngoc Mai",
        courseId: "C0002",
        sessionId: "SE003",
        session: 3,
        sessionDate: "2026-09-09",
        status: "Absent",
        checkinTime: null
    }
];


/* =====================================
   DOM ELEMENTS
===================================== */

const searchStudent = document.getElementById("searchStudent");
const courseFilter = document.getElementById("courseFilter");
const dateFilter = document.getElementById("dateFilter");
const statusFilter = document.getElementById("statusFilter");

const attendanceTableBody = document.getElementById("attendanceTableBody");
const emptyRecords = document.getElementById("emptyRecords");


/* =====================================
   FORMAT ATTENDANCE STATUS
===================================== */

function getStatusClass(status) {
    switch (status) {
        case "Present":
            return "status-present";

        case "Late":
            return "status-late";

        case "Absent":
            return "status-absent";

        default:
            return "";
    }
}


/* =====================================
   FORMAT DATE
===================================== */

function formatDate(dateString) {
    if (!dateString) {
        return "—";
    }

    const date = new Date(`${dateString}T00:00:00`);

    return date.toLocaleDateString("en-GB", {
        day: "2-digit",
        month: "short",
        year: "numeric"
    });
}


/* =====================================
   FORMAT CHECK-IN TIME
===================================== */

function formatCheckinTime(timeString) {
    if (!timeString) {
        return "—";
    }

    return timeString.substring(0, 8);
}


/* =====================================
   RENDER ATTENDANCE RECORDS
===================================== */

function renderAttendanceRecords(records) {
    attendanceTableBody.innerHTML = "";

    if (records.length === 0) {
        emptyRecords.hidden = false;
        return;
    }

    emptyRecords.hidden = true;

    records.forEach((record) => {
        const tableRow = document.createElement("tr");

        const courseName =
            courses[record.courseId]?.courseName || record.courseId;

        const checkinContent = record.checkinTime
            ? `
                <div class="checkin-cell">
                    <span class="checkin-date">
                        ${formatDate(record.sessionDate)}
                    </span>

                    <span class="checkin-time">
                        ${formatCheckinTime(record.checkinTime)}
                    </span>
                </div>
            `
            : `
                <span class="no-checkin">—</span>
            `;

        tableRow.innerHTML = `
            <td>
                <div class="student-cell">
                    <span class="student-name">
                        ${record.studentName}
                    </span>

                    <span class="student-id">
                        ${record.studentId}
                    </span>
                </div>
            </td>

            <td>
                <div class="course-cell">
                    <span class="course-name">
                        ${courseName}
                    </span>

                    <span class="session-number">
                        Session ${record.session} · ${record.sessionId}
                    </span>
                </div>
            </td>

            <td>
                ${formatDate(record.sessionDate)}
            </td>

            <td>
                <span class="status-badge ${getStatusClass(record.status)}">
                    ${record.status}
                </span>
            </td>

            <td>
                ${checkinContent}
            </td>
        `;

        attendanceTableBody.appendChild(tableRow);
    });
}


/* =====================================
   FILTER ATTENDANCE RECORDS
===================================== */

function filterAttendanceRecords() {
    const keyword = searchStudent.value.trim().toLowerCase();
    const selectedCourse = courseFilter.value;
    const selectedDate = dateFilter.value;
    const selectedStatus = statusFilter.value;

    const filteredRecords = attendanceRecords.filter((record) => {

        const matchesSearch =
            record.studentName.toLowerCase().includes(keyword) ||
            record.studentId.toLowerCase().includes(keyword);

        const matchesCourse =
            selectedCourse === "all" ||
            record.courseId === selectedCourse;

        const matchesDate =
            selectedDate === "all" ||
            record.sessionDate === selectedDate;

        const matchesStatus =
            selectedStatus === "all" ||
            record.status === selectedStatus;

        return (
            matchesSearch &&
            matchesCourse &&
            matchesDate &&
            matchesStatus
        );

    });

    // Display newest session first
    filteredRecords.sort((a, b) => {
        const dateComparison =
            b.sessionDate.localeCompare(a.sessionDate);

        if (dateComparison !== 0) {
            return dateComparison;
        }

        return b.attendanceId.localeCompare(a.attendanceId);
    });

    renderAttendanceRecords(filteredRecords);
}


/* =====================================
   RESET FILTERS
===================================== */

function resetFilters() {
    searchStudent.value = "";
    courseFilter.value = "all";
    dateFilter.value = "all";
    statusFilter.value = "all";

    filterAttendanceRecords();
}


/* =====================================
   EVENT LISTENERS
===================================== */

searchStudent.addEventListener("input", filterAttendanceRecords);

courseFilter.addEventListener("change", filterAttendanceRecords);

dateFilter.addEventListener("change", filterAttendanceRecords);

statusFilter.addEventListener("change", filterAttendanceRecords);


/* =====================================
   INITIALIZE PAGE
===================================== */

filterAttendanceRecords();
