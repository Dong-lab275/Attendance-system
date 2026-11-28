
/* =========================================
   ATTENDANCE REPORTS
========================================= */

document.addEventListener("DOMContentLoaded", () => {

    // =====================================
    // 1. DATA FROM SQL
    // =====================================

    // Course information from courses table
    const courses = {
        "C0001": {
            courseName: "Database Systems",
            lecturerName: "Nguyen Van An"
        },
        "C0002": {
            courseName: "Software Engineering",
            lecturerName: "Tran Thi Binh"
        }
    };

    // Attendance records from attendance_records
    // Grouped by class session for report display.
    const attendanceReports = [
        {
            sessionId: "SE001",
            date: "2026-09-07",
            courseId: "C0001",
            students: 3,
            present: 1,
            late: 1,
            absent: 1
        },
        {
            sessionId: "SE002",
            date: "2026-09-14",
            courseId: "C0001",
            students: 3,
            present: 2,
            late: 1,
            absent: 0
        },
        {
            sessionId: "SE003",
            date: "2026-09-09",
            courseId: "C0002",
            students: 2,
            present: 1,
            late: 0,
            absent: 1
        }
    ];


    // =====================================
    // 2. GET HTML ELEMENTS
    // =====================================

    const courseFilter = document.getElementById("courseFilter");
    const dateFilter = document.getElementById("dateFilter");
    const statusFilter = document.getElementById("statusFilter");

    const reportTableBody = document.getElementById("reportTableBody");
    const emptyState = document.getElementById("emptyState");

    const attendanceRate = document.getElementById("attendanceRate");
    const attendanceDescription =
        document.getElementById("attendanceDescription");

    const presentCount = document.getElementById("presentCount");
    const lateCount = document.getElementById("lateCount");
    const absentCount = document.getElementById("absentCount");

    const reportTitle = document.getElementById("reportTitle");
    const sessionCount = document.getElementById("sessionCount");
    const attendanceGuidance =
        document.getElementById("attendanceGuidance");


    // =====================================
    // 3. FORMAT DATE
    // =====================================

    function formatDate(dateString) {

        const date = new Date(dateString + "T00:00:00");

        return date.toLocaleDateString("en-GB", {
            day: "2-digit",
            month: "short",
            year: "numeric"
        });
    }


    // =====================================
    // 4. FILTER BY DATE
    // =====================================

    function filterByDate(report) {

        const selectedDate = dateFilter.value;

        if (selectedDate === "all") {
            return true;
        }

        if (selectedDate === "september") {
            return report.date >= "2026-09-01" &&
                   report.date <= "2026-09-30";
        }

        return true;
    }


    // =====================================
    // 5. FILTER BY ATTENDANCE STATUS
    // =====================================

    function filterByStatus(report) {

        const selectedStatus = statusFilter.value;

        if (selectedStatus === "all") {
            return true;
        }

        if (selectedStatus === "present") {
            return report.present > 0;
        }

        if (selectedStatus === "late") {
            return report.late > 0;
        }

        if (selectedStatus === "absent") {
            return report.absent > 0;
        }

        return true;
    }


    // =====================================
    // 6. GET FILTERED REPORTS
    // =====================================

    function getFilteredReports() {

        const selectedCourse = courseFilter.value;

        return attendanceReports
            .filter(report => {

                const matchCourse =
                    selectedCourse === "all" ||
                    report.courseId === selectedCourse;

                const matchDate = filterByDate(report);

                const matchStatus = filterByStatus(report);

                return matchCourse && matchDate && matchStatus;

            })
            .sort((a, b) => b.date.localeCompare(a.date));
    }


    // =====================================
    // 7. CALCULATE SUMMARY
    // =====================================

    function updateSummary(reports) {

        let totalRecords = 0;
        let totalPresent = 0;
        let totalLate = 0;
        let totalAbsent = 0;

        reports.forEach(report => {

            totalRecords += report.students;
            totalPresent += report.present;
            totalLate += report.late;
            totalAbsent += report.absent;

        });

        const totalAttended = totalPresent + totalLate;

        const rate = totalRecords > 0
            ? (totalAttended / totalRecords) * 100
            : 0;

        attendanceRate.textContent = rate.toFixed(1) + "%";

        attendanceDescription.textContent =
            `${totalAttended} attended / ${totalRecords} records`;

        presentCount.textContent = totalPresent;
        lateCount.textContent = totalLate;
        absentCount.textContent = totalAbsent;

        return {
            totalRecords,
            totalPresent,
            totalLate,
            totalAbsent,
            totalAttended,
            rate
        };
    }


    // =====================================
    // 8. RENDER REPORT TABLE
    // =====================================

    function renderReportTable(reports) {

        reportTableBody.innerHTML = "";

        if (reports.length === 0) {

            emptyState.hidden = false;
            return;
        }

        emptyState.hidden = true;

        reports.forEach(report => {

            const rate = report.students > 0
                ? ((report.present + report.late) /
                    report.students) * 100
                : 0;

            const row = document.createElement("tr");

            row.innerHTML = `
                <td>
                    ${report.sessionId} · ${formatDate(report.date)}
                </td>

                <td>${report.students}</td>

                <td class="highlight-present">
                    ${report.present}
                </td>

                <td class="highlight-late">
                    ${report.late}
                </td>

                <td class="highlight-absent">
                    ${report.absent}
                </td>

                <td class="rate-cell">
                    ${rate.toFixed(1)}%
                </td>
            `;

            reportTableBody.appendChild(row);

        });
    }


    // =====================================
    // 9. UPDATE REPORT TITLE
    // =====================================

    function updateReportTitle() {

        const selectedCourse = courseFilter.value;

        if (selectedCourse === "all") {

            reportTitle.textContent =
                "All courses · Session report";

            return;
        }

        const course = courses[selectedCourse];

        const courseName = course
            ? course.courseName
            : "Unknown course";

        reportTitle.textContent =
            `${courseName} · Session report`;
    }


    // =====================================
    // 10. UPDATE SESSION COUNT
    // =====================================

    function updateSessionCount(reports) {

        const count = reports.length;

        sessionCount.textContent =
            `${count} completed ${count === 1 ? "session" : "sessions"}`;
    }


    // =====================================
    // 11. UPDATE ATTENDANCE GUIDANCE
    // =====================================

    function updateGuidance(reports, summary) {

        const totalSessions = reports.length;

        if (totalSessions === 0) {

            attendanceGuidance.textContent =
                "No completed sessions found for the selected filters.";

            return;
        }

        attendanceGuidance.textContent =
            `${totalSessions} completed sessions · ` +
            `${summary.totalRecords} attendance records. ` +
            `Attendance rate is calculated from Present and Late records.`;
    }


    // =====================================
    // 12. UPDATE ALL REPORT DATA
    // =====================================

    function updateReports() {

        const filteredReports = getFilteredReports();

        const summary = updateSummary(filteredReports);

        renderReportTable(filteredReports);

        updateReportTitle();

        updateSessionCount(filteredReports);

        updateGuidance(filteredReports, summary);
    }


    // =====================================
    // 13. FILTER EVENT LISTENERS
    // =====================================

    courseFilter.addEventListener("change", updateReports);

    dateFilter.addEventListener("change", updateReports);

    statusFilter.addEventListener("change", updateReports);


    // =====================================
    // 14. INITIALIZE PAGE
    // =====================================

    updateReports();

});
