
    // Demo data. Replace with backend results later.
    let attendanceRecords = [];
const PAGE_SIZE = 5;
let currentPage = 1;
    const courseFilter = document.getElementById("courseFilter");
    const statusFilter = document.getElementById("statusFilter");
    const dateFilter = document.getElementById("dateFilter");
    const historyTable = document.getElementById("historyTable");

async function loadAttendanceHistory() {
  try {
    const response = await fetch(
      "/api/attendance-history?studentId=ST001"
    );

    if (!response.ok) {
      throw new Error("Cannot load attendance history");
    }

    const data = await response.json();

    const courseNames = {
      PHI205: "Triết học Mác",
      CS204: "Cấu trúc dữ liệu và giải thuật"
    };

    attendanceRecords = data.records.map(record => ({
      courseId: record.courseId,
      course: courseNames[record.courseId] || record.courseId,
      session: record.sessionId,
      date: record.checkinTime ? record.checkinTime.substring(0, 10) : "",
      time: record.checkinTime ? record.checkinTime.substring(11, 19) : "Not recorded",
      checkin: "Demo",
      status: record.status === "DEMO_PRESENT"
        ? "Demo Present"
        : record.status
    }));

    // Add available course IDs to the filter.
    data.records.forEach(record => {
      const exists = Array.from(courseFilter.options)
        .some(option => option.value === record.courseId);

      if (!exists) {
        const option = document.createElement("option");
        option.value = record.courseId;
        option.textContent =
          courseNames[record.courseId] || record.courseId;

        courseFilter.appendChild(option);
      }
    });

    // Add the demo status to the filter.
    if (!Array.from(statusFilter.options)
        .some(option => option.value === "Demo Present")) {
      const option = document.createElement("option");
      option.value = "Demo Present";
      option.textContent = "Demo Present";
      statusFilter.appendChild(option);
    }

    updateHistory();
  } catch (error) {
    console.error(error);
    alert("Cannot load attendance history.");
  }
}

    function updateHistory() {
      const selectedCourse = courseFilter.value;
      const selectedStatus = statusFilter.value;
      const selectedDate = dateFilter.value;

      const historyTitle = document.getElementById("historyTitle");
const selectedCourseName =
  courseFilter.options[courseFilter.selectedIndex]?.textContent || "";

historyTitle.textContent =
  selectedCourse === "all"
    ? "All courses · Attendance records"
    : selectedCourseName + " · Attendance records";
const filteredRecords = attendanceRecords.filter(record => {
        const matchCourse =
          selectedCourse === "all" ||
          record.courseId === selectedCourse;

        const matchStatus =
          selectedStatus === "all" ||
          record.status === selectedStatus;

        const matchDate =
          !selectedDate ||
          record.date === selectedDate;

        return matchCourse && matchStatus && matchDate;
      });

      historyTable.innerHTML = "";

      const totalPages = Math.max(1, Math.ceil(filteredRecords.length / PAGE_SIZE));
currentPage = Math.min(currentPage, totalPages);

const startIndex = (currentPage - 1) * PAGE_SIZE;
const visibleRecords = filteredRecords.slice(startIndex, startIndex + PAGE_SIZE);

visibleRecords.forEach(record => {
        const row = document.createElement("tr");

        const values = [
          "Session " + record.session,
          record.date + " · " + record.time,
          record.checkin
        ];

        values.forEach(value => {
          const cell = document.createElement("td");
          cell.textContent = value;
          row.appendChild(cell);
        });

        const statusCell = document.createElement("td");
        const badge = document.createElement("span");
        badge.className = "badge " + record.status.toLowerCase();
        badge.textContent = record.status;
        statusCell.appendChild(badge);
        row.appendChild(statusCell);

        historyTable.appendChild(row);
      });

      if (filteredRecords.length === 0) {
        const row = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 4;
        cell.textContent = "No attendance records found.";
        row.appendChild(cell);
        historyTable.appendChild(row);
      }

      const historyCount = document.getElementById("historyCount");
const pageButtons = document.getElementById("historyPageButtons");

historyCount.textContent =
  "Showing " + visibleRecords.length +
  " of " + filteredRecords.length + " sessions";

pageButtons.innerHTML = "";

function addPageButton(label, page, disabled, active = false) {
  const button = document.createElement("button");
  button.textContent = label;
  button.disabled = disabled;
  if (active) button.style.background = "#E7F4F1";

  button.addEventListener("click", () => {
    currentPage = page;
    updateHistory();
  });

  pageButtons.appendChild(button);
}

addPageButton("Previous", currentPage - 1, currentPage === 1);

for (let page = 1; page <= totalPages; page++) {
  addPageButton(String(page), page, false, page === currentPage);
}

addPageButton("Next", currentPage + 1, currentPage === totalPages);

const present = filteredRecords.filter(
        record => record.status === "Present" || record.status === "Demo Present"
      ).length;

      const late = filteredRecords.filter(
        record => record.status === "Late"
      ).length;

      const absent = filteredRecords.filter(
        record => record.status === "Absent"
      ).length;

      const total = present + late + absent;

      const rate = total > 0
        ? ((present + late) / total * 100).toFixed(1)
        : "0.0";

      document.getElementById("rateValue").textContent = rate + "%";
      document.getElementById("presentValue").textContent = present;
      document.getElementById("lateValue").textContent = late;
      document.getElementById("absentValue").textContent = absent;
    }

    courseFilter.addEventListener("change", updateHistory);
    statusFilter.addEventListener("change", updateHistory);
    dateFilter.addEventListener("change", updateHistory);

    document.getElementById("password-link")
      .addEventListener("click", function(event) {
        event.preventDefault();
        alert("Password changes require the authentication backend.");
      });

    document.getElementById("signout-link")
      .addEventListener("click", function(event) {
        event.preventDefault();
        alert("Sign out requires a real login session.");
      });

   loadAttendanceHistory();
