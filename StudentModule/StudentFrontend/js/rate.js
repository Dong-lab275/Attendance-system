
// Student Attendance Rate

let courses = [];

const studentId = "ST001";

const courseFilter = document.getElementById("courseFilter");
const rateTable = document.getElementById("rateTable");

// Course names
const courseNames = {
    CS201: "Cơ sở dữ liệu",
    CS202: "Lập trình hướng đối tượng",
    CS203: "Mạng máy tính",
    CS204: "Cấu trúc dữ liệu và giải thuật",
    CS302: "Công nghệ phần mềm",
    CS305: "Trí tuệ nhân tạo",
    PHI205: "Triết học Mác"
};

// Load attendance information
async function loadAttendanceRate() {
    try {
        const [coursesResponse, historyResponse] =
            await Promise.all([
                fetch("/api/my-courses?studentId=" + studentId),
                fetch("/api/attendance-history?studentId=" + studentId)
            ]);

        if (!coursesResponse.ok || !historyResponse.ok) {
            throw new Error("Cannot load attendance data.");
        }

        const coursesData = await coursesResponse.json();
        const historyData = await historyResponse.json();

        if (!Array.isArray(coursesData.courses) ||
            !Array.isArray(historyData.records)) {
            throw new Error("Invalid attendance data.");
        }

        // Create attendance information for each course
        courses = coursesData.courses.map(courseId => {

            const officialRecords = historyData.records.filter(record =>
                record.courseId === courseId &&
                ["PRESENT", "LATE", "ABSENT"].includes(
                    String(record.status).toUpperCase()
                )
            );

            return {
                id: courseId,
                name: courseNames[courseId] || courseId,

                sessions: officialRecords.length,

                present: officialRecords.filter(record =>
                    String(record.status).toUpperCase() === "PRESENT"
                ).length,

                late: officialRecords.filter(record =>
                    String(record.status).toUpperCase() === "LATE"
                ).length,

                absent: officialRecords.filter(record =>
                    String(record.status).toUpperCase() === "ABSENT"
                ).length
            };
        });

        // Initialize course filter
        courseFilter.innerHTML =
            '<option value="all">All courses</option>';

        initializeFilter();

        updateRate();

    } catch (error) {
        console.error("Attendance rate error:", error);

        rateTable.innerHTML =
            '<tr><td colspan="6">Cannot load attendance data.</td></tr>';

        alert("Cannot load attendance rate.");
    }
}

// Calculate attendance rate
function calculateRate(course) {

    if (course.sessions === 0) {
        return "N/A";
    }

    return (
        (course.present + course.late) /
        course.sessions * 100
    ).toFixed(1);
}

// Create table cell
function createCell(row, value) {

    const cell = document.createElement("td");

    cell.textContent = value;

    row.appendChild(cell);

    return cell;
}

// Initialize course filter
function initializeFilter() {

    courses.forEach(course => {

        const option = document.createElement("option");

        option.value = course.id;
        option.textContent = course.name;

        courseFilter.appendChild(option);
    });
}

// Update attendance rate
function updateRate() {

    const selectedCourse = courseFilter.value;

    const filteredCourses = courses.filter(course =>
        selectedCourse === "all" ||
        course.id === selectedCourse
    );

    rateTable.innerHTML = "";

    let completedSessions = 0;
    let attendedSessions = 0;

    filteredCourses.forEach(course => {

        completedSessions += course.sessions;

        attendedSessions +=
            course.present + course.late;

        const row = document.createElement("tr");

        // Course name and ID
        const courseCell = document.createElement("td");

        const name = document.createElement("span");

        name.className = "course-name";
        name.textContent = course.name;

        const id = document.createElement("span");

        id.className = "course-id";
        id.textContent = course.id;

        courseCell.appendChild(name);
        courseCell.appendChild(id);

        row.appendChild(courseCell);

        // Attendance statistics
        createCell(row, course.sessions);
        createCell(row, course.present);
        createCell(row, course.late);
        createCell(row, course.absent);

        // Attendance percentage
        const rateCell = document.createElement("td");

        const badge = document.createElement("span");

        badge.className = "rate-badge";

        const rate = calculateRate(course);

        badge.textContent =
            rate === "N/A" ? "N/A" : rate + "%";

        rateCell.appendChild(badge);

        row.appendChild(rateCell);

        rateTable.appendChild(row);
    });

    // Show empty message
    if (filteredCourses.length === 0) {

        const row = document.createElement("tr");

        const cell = document.createElement("td");

        cell.colSpan = 6;

        cell.textContent = "No registered courses found.";

        row.appendChild(cell);

        rateTable.appendChild(row);
    }

    // Update summary cards
    document.getElementById("registeredCount").textContent =
        filteredCourses.length;

    document.getElementById("completedCount").textContent =
        completedSessions;

    document.getElementById("attendedCount").textContent =
        attendedSessions;
}

// Course filter event
courseFilter.addEventListener("change", updateRate);

// Password event
document.getElementById("password-link")
    .addEventListener("click", function(event) {

        event.preventDefault();

        alert(
            "Password changes require the authentication backend."
        );
    });

// Sign out event
document.getElementById("signout-link")
    .addEventListener("click", function(event) {

        event.preventDefault();

        alert(
            "Sign out requires a real login session."
        );
    });

// Start application
loadAttendanceRate();
