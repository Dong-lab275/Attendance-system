
    // Demo registration period from the Figma example.
    const registrationStart = "2026-11-02";
    const registrationEnd = "2026-11-20";

    const courses = [
      {
        id: "CS201",
        name: "Cơ sở dữ liệu",
        lecturer: "Trần Đức Huy",
        registered: true
      },
      {
        id: "SE202",
        name: "Công nghệ phần mềm",
        lecturer: "Nguyễn Văn Minh",
        registered: true
      },
      {
        id: "OOP203",
        name: "Lập trình hướng đối tượng",
        lecturer: "Lê Thu Hà",
        registered: true
      },
      {
        id: "NET204",
        name: "Mạng máy tính",
        lecturer: "Phạm Quốc Bảo",
        registered: true
      },
      {
        id: "PHI205",
        name: "Triết học Mác",
        lecturer: "Nguyễn Thị Lan",
        registered: false
      },
      {
        id: "DSA206",
        name: "Cấu trúc dữ liệu và giải thuật",
        lecturer: "Trần Minh Đức",
        registered: false
      }
    ];

    const today = new Date();
    const localDate = [
      today.getFullYear(),
      String(today.getMonth() + 1).padStart(2, "0"),
      String(today.getDate()).padStart(2, "0")
    ].join("-");

    // Demo mode: allow registration for testing
const registrationOpen = true;

    const availableTable =
      document.getElementById("availableCourses");

    const registeredTable =
      document.getElementById("registeredCourses");

    function createCell(row, value) {
      const cell = document.createElement("td");
      cell.textContent = value;
      row.appendChild(cell);
      return cell;
    }
async function loadRegisteredCourses() {
  try {
    const response = await fetch(
      "/api/my-courses?studentId=ST001"
    );

    if (!response.ok) {
      throw new Error("Cannot load registered courses");
    }

    const data = await response.json();

    courses.forEach(course => {
      course.registered = data.courses.includes(course.id);
    });

    renderCourses();
  } catch (error) {
    console.error(error);
    alert("Cannot load registered courses!");
  }
}
    function renderCourses() {
      availableTable.innerHTML = "";
      registeredTable.innerHTML = "";

      courses.forEach(course => {
        if (!course.registered) {
          const row = document.createElement("tr");

          createCell(row, course.name).className = "course-name";
          createCell(row, course.id);
          createCell(row, course.lecturer);

          const actionCell = document.createElement("td");
          const button = document.createElement("button");

          button.className = "register-btn";
          button.textContent = "Register";
          button.disabled = !registrationOpen;

          button.addEventListener("click", async function() {
            if (!registrationOpen || course.registered) {
              return;
            }

            // Frontend demo only; no database write.
            const studentId = "ST001";

button.disabled = true;
button.textContent = "Registering...";

try {
  const formData = new URLSearchParams({
    studentId: studentId,
    courseId: course.id
  });

  const response = await fetch("/api/register-course", {
    method: "POST",
    body: formData
  });

  const result = await response.json();

  if (response.ok || response.status === 409) {
    course.registered = true;
    renderCourses();

    alert(response.ok
      ? "Course registered successfully!"
      : "Course already registered!");
  } else {
    alert(result.message || "Registration failed!");
    button.disabled = false;
    button.textContent = "Register";
  }
} catch (error) {
  alert("Cannot connect to Student Server!");
  button.disabled = false;
  button.textContent = "Register";
}
          });

          actionCell.appendChild(button);
          row.appendChild(actionCell);
          availableTable.appendChild(row);
        } else {
          const row = document.createElement("tr");

          createCell(row, course.name).className = "course-name";
          createCell(row, course.id);
          createCell(row, course.lecturer);

          const statusCell = document.createElement("td");
          const badge = document.createElement("span");

          badge.className = "badge";
          badge.textContent = "Registered";

          statusCell.appendChild(badge);
          row.appendChild(statusCell);
          registeredTable.appendChild(row);
        }
      });

      if (!availableTable.children.length) {
        const row = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 4;
        cell.textContent = "No available courses.";
        row.appendChild(cell);
        availableTable.appendChild(row);
      }
    }

    const notice = document.getElementById("registrationNotice");

    if (registrationOpen) {
      notice.textContent =
        "Registration is open · 02–20 Nov 2026. " +
        "You can register for the available courses below.";
    } else {
      notice.textContent =
        "Registration is closed. Demo registration period: " +
        "02–20 Nov 2026. New registrations are unavailable.";
    }

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

   loadRegisteredCourses();
