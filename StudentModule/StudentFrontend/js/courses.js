
// Registration period
const registrationStart = "2026-11-02";
const registrationEnd = "2026-11-20";

// Student information
const studentId = "ST001";

// Course catalog
const courses = [
    {
        id: "CS305",
        name: "Trí tuệ nhân tạo",
        lecturer: "Nguyễn Thu Trang",
        registered: false
    },
    {
        id: "CS302",
        name: "Công nghệ phần mềm",
        lecturer: "Phạm Hoàng Nam",
        registered: false
    },
    {
        id: "CS201",
        name: "Cơ sở dữ liệu",
        lecturer: "Trần Quốc Huy",
        registered: true
    },
    {
        id: "CS202",
        name: "Lập trình hướng đối tượng",
        lecturer: "Nguyễn Thu Trang",
        registered: true
    },
    {
        id: "CS203",
        name: "Mạng máy tính",
        lecturer: "Phạm Hoàng Nam",
        registered: true
    },
    {
        id: "CS204",
        name: "Cấu trúc dữ liệu và giải thuật",
        lecturer: "Trần Quốc Huy",
        registered: true
    }
];

// Demo mode

const today = new Date();

const localDate = [
    today.getFullYear(),
    String(today.getMonth() + 1).padStart(2, "0"),
    String(today.getDate()).padStart(2, "0")
].join("-");

const registrationOpen =
    localDate >= registrationStart &&
    localDate <= registrationEnd;


// HTML elements
const availableTable =
    document.getElementById("availableCourses");

const registeredTable =
    document.getElementById("registeredCourses");

const notice =
    document.getElementById("registrationNotice");

// Create table cell
function createCell(row, value) {
    const cell = document.createElement("td");
    cell.textContent = value;
    row.appendChild(cell);
    return cell;
}

// Load registered courses from C++ backend
async function loadRegisteredCourses() {
    try {
        const response = await fetch(
            "/api/my-courses?studentId=" + studentId
        );

        if (!response.ok) {
            throw new Error("Cannot load registered courses.");
        }

        const data = await response.json();

        courses.forEach(course => {
            course.registered =
                data.courses.includes(course.id);
        });

        renderCourses();

    } catch (error) {
        console.error(error);
        alert("Cannot load registered courses!");
    }
}

// Register course
async function registerCourse(course, button) {
    if (!registrationOpen || course.registered) {
        return;
    }

    button.disabled = true;
    button.textContent = "Registering...";

    try {
        const formData = new URLSearchParams({
            studentId: studentId,
            courseId: course.id
        });

        const response = await fetch(
            "/api/register-course",
            {
                method: "POST",
                body: formData
            }
        );

        const result = await response.json();

        if (!response.ok || !result.success) {
            throw new Error(
                result.message || "Registration failed."
            );
        }

        alert("Course registered successfully!");

        await loadRegisteredCourses();

    } catch (error) {
        console.error(error);
        alert(error.message);

        button.disabled = false;
        button.textContent = "Register";
    }
}

// Unregister course
async function unregisterCourse(course, button) {
    if (!course.registered) {
        return;
    }

    const confirmed = confirm(
        "Are you sure you want to unregister from " +
        course.name + "?"
    );

    if (!confirmed) {
        return;
    }

    button.disabled = true;
    button.textContent = "Removing...";

    try {
        const formData = new URLSearchParams({
            studentId: studentId,
            courseId: course.id
        });

        const response = await fetch(
            "/api/unregister-course",
            {
                method: "POST",
                body: formData
            }
        );

        const result = await response.json();

        if (!response.ok || !result.success) {
            throw new Error(
                result.message || "Unregistration failed."
            );
        }

        alert("Course unregistered successfully!");

        await loadRegisteredCourses();

    } catch (error) {
        console.error(error);
        alert(error.message);

        button.disabled = false;
        button.textContent = "Unregister";
    }
}

// Display courses
function renderCourses() {
    availableTable.innerHTML = "";
    registeredTable.innerHTML = "";

    courses.forEach(course => {

        if (!course.registered) {

            // Available courses
            const row = document.createElement("tr");

            createCell(row, course.name).className =
                "course-name";

            createCell(row, course.id);
            createCell(row, course.lecturer);

            const actionCell =
                document.createElement("td");

            const button =
                document.createElement("button");

            button.className = "register-btn";
            button.textContent = "Register";
            button.disabled = !registrationOpen;

            button.addEventListener("click", function() {
                registerCourse(course, button);
            });

            actionCell.appendChild(button);
            row.appendChild(actionCell);

            availableTable.appendChild(row);

        } else {

            // Registered courses
            const row = document.createElement("tr");

            createCell(row, course.name).className =
                "course-name";

            createCell(row, course.id);
            createCell(row, course.lecturer);

            const statusCell =
                document.createElement("td");

            const button =
                document.createElement("button");

            button.className = "unregister-btn";
            button.textContent = "Unregister";

            button.addEventListener("click", function() {
                unregisterCourse(course, button);
            });

            statusCell.appendChild(button);
            row.appendChild(statusCell);

            registeredTable.appendChild(row);
        }
    });

    // Empty available courses
    if (availableTable.children.length === 0) {
        const row = document.createElement("tr");
        const cell = document.createElement("td");

        cell.colSpan = 4;
        cell.textContent = "No available courses.";

        row.appendChild(cell);
        availableTable.appendChild(row);
    }

    // Empty registered courses
    if (registeredTable.children.length === 0) {
        const row = document.createElement("tr");
        const cell = document.createElement("td");

        cell.colSpan = 4;
        cell.textContent = "You have not registered for any courses.";

        row.appendChild(cell);
        registeredTable.appendChild(row);
    }
}

// Registration notice
if (registrationOpen) {
    notice.textContent =
        "Registration is open · 02–20 Nov 2026. " +
        "You can register for the available courses below.";
} else {
    notice.textContent =
        "Registration is closed. New registrations are unavailable.";
}

// Password
document.getElementById("password-link")
    .addEventListener("click", function(event) {
        event.preventDefault();

        alert(
            "Password changes require the authentication backend."
        );
    });

// Sign out
document.getElementById("signout-link")
    .addEventListener("click", function(event) {
        event.preventDefault();

        alert(
            "Sign out requires a real login session."
        );
    });

// Start application
loadRegisteredCourses();
