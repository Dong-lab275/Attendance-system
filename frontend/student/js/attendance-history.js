
document.addEventListener("DOMContentLoaded", () => {

    const courseFilter = document.getElementById("courseFilter");
    const statusFilter = document.getElementById("statusFilter");
    const dateFilter = document.getElementById("dateFilter");

    const tableBody = document.getElementById("attendanceTableBody");
    const recordsTitle = document.getElementById("recordsTitle");
    const recordsCount = document.getElementById("recordsCount");
    const emptyMessage = document.getElementById("emptyMessage");

    const attendanceRows = Array.from(tableBody.querySelectorAll("tr"));

    const totalSessions = 12;

    const courseNames = {
        all: "All courses",
        database: "Cơ sở dữ liệu",
        programming: "Object-Oriented Programming",
        web: "Web Development"
    };

    function filterAttendanceRecords() {

        const selectedCourse = courseFilter.value;
        const selectedStatus = statusFilter.value;
        const selectedMonth = dateFilter.value;

        let visibleCount = 0;

        attendanceRows.forEach(row => {

            const rowCourse = row.dataset.course;
            const rowStatus = row.dataset.status;
            const rowMonth = row.dataset.month;

            const matchesCourse =
                selectedCourse === "all" ||
                rowCourse === selectedCourse;

            const matchesStatus =
                selectedStatus === "all" ||
                rowStatus === selectedStatus;

            const matchesMonth =
                selectedMonth === "all" ||
                rowMonth === selectedMonth;

            const isVisible =
                matchesCourse &&
                matchesStatus &&
                matchesMonth;

            row.hidden = !isVisible;

            if (isVisible) {
                visibleCount++;
            }
        });

        updateRecordsTitle(selectedCourse);
        updateRecordsCount(visibleCount);

        emptyMessage.hidden = visibleCount !== 0;
    }

    function updateRecordsTitle(course) {

        if (course === "all") {
            recordsTitle.textContent = "All courses · Attendance records";
        } else {
            recordsTitle.textContent =
                `${courseNames[course]} · Attendance records`;
        }
    }

    function updateRecordsCount(visibleCount) {

        recordsCount.textContent =
            `Showing ${visibleCount} of ${totalSessions} sessions`;
    }

    courseFilter.addEventListener("change", filterAttendanceRecords);
    statusFilter.addEventListener("change", filterAttendanceRecords);
    dateFilter.addEventListener("change", filterAttendanceRecords);

    // Initialize the page
    filterAttendanceRecords();

});
