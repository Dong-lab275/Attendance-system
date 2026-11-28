
document.addEventListener("DOMContentLoaded", () => {

    const availableCoursesBody =
        document.getElementById("availableCoursesBody");

    const registeredCoursesBody =
        document.getElementById("registeredCoursesBody");

    const availableEmptyMessage =
        document.getElementById("availableEmptyMessage");

    const registeredEmptyMessage =
        document.getElementById("registeredEmptyMessage");

    // Check whether a course is already registered
    function isCourseRegistered(courseId) {

        return Array.from(
            registeredCoursesBody.querySelectorAll("tr")
        ).some(row => row.dataset.courseId === courseId);
    }

    // Create the Registered status badge
    function createRegisteredStatus() {

        const status = document.createElement("span");

        status.className = "registration-status";
        status.textContent = "Registered";

        return status;
    }

    // Register a course
    function registerCourse(button) {

        const courseId = button.dataset.register;

        const availableRow = button.closest("tr");

        if (!availableRow || !courseId) {
            return;
        }

        // Prevent duplicate registration
        if (isCourseRegistered(courseId)) {
            button.disabled = true;
            button.textContent = "Registered";
            return;
        }

        // Get course information
        const cells = availableRow.querySelectorAll("td");

        if (cells.length !== 4) {
            return;
        }

        const courseName = cells[0].textContent.trim();
        const lecturerName = cells[2].textContent.trim();

        // Create a new registered course row
        const registeredRow = document.createElement("tr");

        registeredRow.dataset.courseId = courseId;

        // Course name
        const courseNameCell = document.createElement("td");
        courseNameCell.className = "course-name";
        courseNameCell.textContent = courseName;

        // Course ID
        const courseIdCell = document.createElement("td");
        courseIdCell.textContent = courseId;

        // Lecturer
        const lecturerCell = document.createElement("td");
        lecturerCell.textContent = lecturerName;

        // Registration status
        const statusCell = document.createElement("td");
        statusCell.appendChild(createRegisteredStatus());

        registeredRow.append(
            courseNameCell,
            courseIdCell,
            lecturerCell,
            statusCell
        );

        // Add to registered courses
        registeredCoursesBody.appendChild(registeredRow);

        // Remove from available courses
        availableRow.remove();

        updateEmptyMessages();
    }

    // Update empty table messages
    function updateEmptyMessages() {

        const availableCount =
            availableCoursesBody.querySelectorAll("tr").length;

        const registeredCount =
            registeredCoursesBody.querySelectorAll("tr").length;

        availableEmptyMessage.hidden = availableCount !== 0;

        registeredEmptyMessage.hidden = registeredCount !== 0;
    }

    // Handle Register button clicks
    availableCoursesBody.addEventListener("click", event => {

        const button = event.target.closest(".register-button");

        if (!button) {
            return;
        }

        registerCourse(button);
    });

    // Initialize page
    updateEmptyMessages();

});
