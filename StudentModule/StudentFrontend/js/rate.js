
    // Demo data based on the Figma layout.
    // Replace with backend results later.
    let courses = [];

    const courseFilter = document.getElementById("courseFilter");
    const rateTable = document.getElementById("rateTable");

    async function loadAttendanceRate() {
  try {
    const studentId = "ST001";

    const [coursesResponse, historyResponse] = await Promise.all([
      fetch("/api/my-courses?studentId=" + studentId),
      fetch("/api/attendance-history?studentId=" + studentId)
    ]);

    if (!coursesResponse.ok || !historyResponse.ok) {
      throw new Error("Cannot load attendance data");
    }

    const coursesData = await coursesResponse.json();
    const historyData = await historyResponse.json();

    const courseNames = {
      CS101: "CS101",
      PHI205: "Triết học Mác",
      DSA206: "Cấu trúc dữ liệu và giải thuật",
      CS201: "Cơ sở dữ liệu",
      SE202: "Công nghệ phần mềm",
      OOP203: "Lập trình hướng đối tượng",
      NET204: "Mạng máy tính"
    };

    courses = coursesData.courses.map(courseId => {
      const officialRecords = historyData.records.filter(record =>
        record.courseId === courseId &&
        ["PRESENT", "LATE", "ABSENT"].includes(record.status)
      );

      return {
        id: courseId,
        name: courseNames[courseId] || courseId,
        sessions: officialRecords.length,
        present: officialRecords.filter(
          record => record.status === "PRESENT"
        ).length,
        late: officialRecords.filter(
          record => record.status === "LATE"
        ).length,
        absent: officialRecords.filter(
          record => record.status === "ABSENT"
        ).length
      };
    });

    courseFilter.innerHTML =
      '<option value="all">All courses</option>';

    initializeFilter();
    updateRate();

  } catch (error) {
    console.error(error);
    alert("Cannot load attendance rate.");
  }
}

    function calculateRate(course) {
     if (course.sessions === 0) {
  return "N/A";
}

      return (
        (course.present + course.late) /
        course.sessions * 100
      ).toFixed(1);
    }

    function createCell(row, value) {
      const cell = document.createElement("td");
      cell.textContent = value;
      row.appendChild(cell);
      return cell;
    }

    function initializeFilter() {
      courses.forEach(course => {
        const option = document.createElement("option");
        option.value = course.id;
        option.textContent = course.name;
        courseFilter.appendChild(option);
      });
    }

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
        attendedSessions += course.present + course.late;

        const row = document.createElement("tr");

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

        createCell(row, course.sessions);
        createCell(row, course.present);
        createCell(row, course.late);
        createCell(row, course.absent);

        const rateCell = document.createElement("td");
        const badge = document.createElement("span");

        badge.className = "rate-badge";
        const rate = calculateRate(course);
badge.textContent = rate === "N/A" ? "N/A" : rate + "%";

        rateCell.appendChild(badge);
        row.appendChild(rateCell);

        rateTable.appendChild(row);
      });

      document.getElementById("registeredCount").textContent =
        filteredCourses.length;

      document.getElementById("completedCount").textContent =
        completedSessions;

      document.getElementById("attendedCount").textContent =
        attendedSessions;
    }

    courseFilter.addEventListener("change", updateRate);

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

   loadAttendanceRate();
