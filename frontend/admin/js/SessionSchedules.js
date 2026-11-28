
/* =========================================
   SESSION SCHEDULES
   Frontend demonstration data based on data.sql
   This version does not connect to PostgreSQL.
========================================= */


/* =========================================
   COURSE AND LECTURER DATA
========================================= */

const courses = [
    {
        courseId: "C0001",
        courseName: "Database Systems",
        lecturerId: "079082001234",
        lecturerName: "Nguyen Van An"
    },
    {
        courseId: "C0002",
        courseName: "Software Engineering",
        lecturerId: "079185002345",
        lecturerName: "Tran Thi Binh"
    }
];


/* =========================================
   SESSION DATA FROM SQL
========================================= */

let sessionSchedules = [
    {
        id: "SE001",
        courseId: "C0001",
        courseName: "Database Systems",
        lecturer: "Nguyen Van An",
        sessionNumber: 1,
        date: "2026-09-07",
        startTime: "07:30",
        endTime: "09:30",
        checkInOpens: "07:15",
        periodEnds: "07:45",
        status: "Completed"
    },
    {
        id: "SE002",
        courseId: "C0001",
        courseName: "Database Systems",
        lecturer: "Nguyen Van An",
        sessionNumber: 2,
        date: "2026-09-14",
        startTime: "07:30",
        endTime: "09:30",
        checkInOpens: "07:15",
        periodEnds: "07:45",
        status: "Completed"
    },
    {
        id: "SE003",
        courseId: "C0002",
        courseName: "Software Engineering",
        lecturer: "Tran Thi Binh",
        sessionNumber: 3,
        date: "2026-09-09",
        startTime: "13:00",
        endTime: "15:00",
        checkInOpens: "12:45",
        periodEnds: "13:15",
        status: "Completed"
    }
];


/* =========================================
   DOM ELEMENTS
========================================= */

const scheduleTableBody =
    document.getElementById("scheduleTableBody");

const emptyState =
    document.getElementById("emptyState");

const sessionForm =
    document.getElementById("sessionForm");

const editorTitle =
    document.getElementById("editorTitle");

const sessionIdInput =
    document.getElementById("sessionId");

const sessionCourseInput =
    document.getElementById("sessionCourse");

const sessionNumberInput =
    document.getElementById("sessionNumber");

const sessionDateInput =
    document.getElementById("sessionDate");

const sessionLecturerInput =
    document.getElementById("sessionLecturer");

const startTimeInput =
    document.getElementById("startTime");

const endTimeInput =
    document.getElementById("endTime");

const checkInOpensInput =
    document.getElementById("checkInOpens");

const periodEndsInput =
    document.getElementById("periodEnds");

const automaticClosureNote =
    document.getElementById("automaticClosureNote");

const createSessionBtn =
    document.getElementById("createSessionBtn");

const saveSessionBtn =
    document.getElementById("saveSessionBtn");

const cancelSessionBtn =
    document.getElementById("cancelSessionBtn");

const deleteSessionBtn =
    document.getElementById("deleteSessionBtn");

const courseFilter =
    document.getElementById("courseFilter");

const lecturerFilter =
    document.getElementById("lecturerFilter");

const filterStartDate =
    document.getElementById("filterStartDate");

const filterEndDate =
    document.getElementById("filterEndDate");


/* =========================================
   HELPER FUNCTIONS
========================================= */

function formatDate(dateString) {

    if (!dateString) return "";

    return new Date(dateString + "T00:00:00")
        .toLocaleDateString("en-GB", {
            day: "2-digit",
            month: "short",
            year: "numeric"
        });
}


function formatShortDate(dateString) {

    if (!dateString) return "";

    return new Date(dateString + "T00:00:00")
        .toLocaleDateString("en-GB", {
            day: "2-digit",
            month: "short"
        });
}


function getCourse(courseId) {

    return courses.find(
        course => course.courseId === courseId
    );
}


function getCourseName(courseId) {

    const course = getCourse(courseId);

    return course ? course.courseName : "Unknown course";
}


function getLecturerName(courseId) {

    const course = getCourse(courseId);

    return course ? course.lecturerName : "";
}


function getStatusClass(status) {

    const statusClasses = {
        Open: "status-open",
        Scheduled: "status-scheduled",
        Closed: "status-closed",
        Completed: "status-completed"
    };

    return statusClasses[status] || "status-scheduled";
}


function escapeHTML(value) {

    return String(value ?? "").replace(/[&<>"']/g, character => {

        const entities = {
            "&": "&amp;",
            "<": "&lt;",
            ">": "&gt;",
            '"': "&quot;",
            "'": "&#39;"
        };

        return entities[character];
    });
}


/* =========================================
   POPULATE SELECT OPTIONS
========================================= */

function populateCourseOptions() {

    const courseOptions = courses.map(course => `
        <option value="${course.courseId}">
            ${course.courseName} · ${course.courseId}
        </option>
    `).join("");

    sessionCourseInput.innerHTML =
        `<option value="">Select course</option>` +
        courseOptions;

    courseFilter.innerHTML =
        `<option value="">All courses</option>` +
        courseOptions;
}


function populateLecturerOptions() {

    const lecturerOptions = [
        ...new Map(
            courses.map(course => [
                course.lecturerName,
                course.lecturerName
            ])
        ).values()
    ];

    const options = lecturerOptions.map(name => `
        <option value="${escapeHTML(name)}">
            ${escapeHTML(name)}
        </option>
    `).join("");

    sessionLecturerInput.innerHTML =
        `<option value="">Select lecturer</option>` +
        options;

    lecturerFilter.innerHTML =
        `<option value="">All lecturers</option>` +
        options;
}


/* Automatically select the lecturer assigned to the course. */

function updateLecturerByCourse() {

    const courseId = sessionCourseInput.value;

    const lecturerName = getLecturerName(courseId);

    sessionLecturerInput.value = lecturerName;
}


/* =========================================
   AUTOMATIC CLOSURE
========================================= */

function calculateAutomaticClosure(periodEnds) {

    if (!periodEnds) return "--:--";

    const [hours, minutes] = periodEnds
        .split(":")
        .map(Number);

    const totalMinutes = hours * 60 + minutes + 15;

    const closureHours =
        Math.floor(totalMinutes / 60) % 24;

    const closureMinutes = totalMinutes % 60;

    return (
        String(closureHours).padStart(2, "0") +
        ":" +
        String(closureMinutes).padStart(2, "0")
    );
}


function updateAutomaticClosureNote() {

    const closureTime =
        calculateAutomaticClosure(periodEndsInput.value);

    automaticClosureNote.textContent =
        `Automatic closure: ${closureTime}. ` +
        "Unrecorded students will be marked Absent.";
}


/* =========================================
   RENDER SCHEDULE TABLE
========================================= */

function renderSchedules() {

    scheduleTableBody.innerHTML = "";

    const selectedCourse = courseFilter.value;
    const selectedLecturer = lecturerFilter.value;

    const startDate = filterStartDate.value;
    const endDate = filterEndDate.value;

    const filteredSchedules = sessionSchedules
        .filter(schedule => {

            const matchesCourse =
                !selectedCourse ||
                schedule.courseId === selectedCourse;

            const matchesLecturer =
                !selectedLecturer ||
                schedule.lecturer === selectedLecturer;

            const matchesStartDate =
                !startDate ||
                schedule.date >= startDate;

            const matchesEndDate =
                !endDate ||
                schedule.date <= endDate;

            return (
                matchesCourse &&
                matchesLecturer &&
                matchesStartDate &&
                matchesEndDate
            );
        })
        .sort((a, b) => {

            const dateComparison =
                a.date.localeCompare(b.date);

            if (dateComparison !== 0) {
                return dateComparison;
            }

            return a.startTime.localeCompare(b.startTime);
        });


    emptyState.classList.toggle(
        "show",
        filteredSchedules.length === 0
    );


    filteredSchedules.forEach(schedule => {

        const row = document.createElement("tr");

        if (schedule.id === sessionIdInput.value) {
            row.classList.add("selected-row");
        }

        row.innerHTML = `
            <td>
                <div class="course-information">
                    <strong>
                        ${escapeHTML(schedule.courseName)}
                    </strong>
                    <span>
                        ${escapeHTML(schedule.lecturer)}
                    </span>
                </div>
            </td>

            <td>
                <div class="class-information">
                    <span>${formatShortDate(schedule.date)}</span>
                    <span>
                        ${escapeHTML(schedule.startTime)}
                        –
                        ${escapeHTML(schedule.endTime)}
                    </span>
                </div>
            </td>

            <td>
                <span class="status-badge ${getStatusClass(schedule.status)}">
                    ${escapeHTML(schedule.status)}
                </span>
            </td>

            <td>
                <div class="table-actions">

                    <button type="button"
                            class="table-action"
                            data-action="view"
                            data-id="${escapeHTML(schedule.id)}">
                        View
                    </button>

                    <button type="button"
                            class="table-action"
                            data-action="edit"
                            data-id="${escapeHTML(schedule.id)}">
                        Edit
                    </button>

                    <button type="button"
                            class="table-action delete"
                            data-action="delete"
                            data-id="${escapeHTML(schedule.id)}">
                        Delete
                    </button>

                </div>
            </td>
        `;

        scheduleTableBody.appendChild(row);
    });
}


/* =========================================
   RESET FORM
========================================= */

function resetSessionForm() {

    sessionForm.reset();

    sessionIdInput.value = "";

    editorTitle.textContent = "Create session schedule";

    saveSessionBtn.textContent = "Create session";

    deleteSessionBtn.disabled = true;

    automaticClosureNote.textContent =
        "Automatic closure: --:--. " +
        "Unrecorded students will be marked Absent.";
}


/* =========================================
   FILL FORM
========================================= */

function fillSessionForm(schedule) {

    sessionIdInput.value = schedule.id;

    sessionCourseInput.value = schedule.courseId;

    sessionNumberInput.value = schedule.sessionNumber;

    sessionDateInput.value = schedule.date;

    sessionLecturerInput.value = schedule.lecturer;

    startTimeInput.value = schedule.startTime;

    endTimeInput.value = schedule.endTime;

    checkInOpensInput.value = schedule.checkInOpens;

    periodEndsInput.value = schedule.periodEnds;

    editorTitle.textContent = "Edit session schedule";

    saveSessionBtn.textContent = "Save changes";

    deleteSessionBtn.disabled = false;

    updateAutomaticClosureNote();
}


/* =========================================
   SELECT SESSION
========================================= */

function selectSession(sessionId) {

    const schedule = sessionSchedules.find(
        item => item.id === sessionId
    );

    if (!schedule) return;

    fillSessionForm(schedule);

    renderSchedules();
}


/* =========================================
   CREATE SESSION
========================================= */

function createSession() {

    resetSessionForm();

    editorTitle.textContent = "Create session schedule";

    saveSessionBtn.textContent = "Create session";

    sessionCourseInput.focus();

    renderSchedules();
}


/* =========================================
   VALIDATE FORM
========================================= */

function validateSessionForm() {

    const courseId = sessionCourseInput.value;

    const sessionNumber =
        Number(sessionNumberInput.value);

    const sessionDate = sessionDateInput.value;

    const lecturer = sessionLecturerInput.value;

    const startTime = startTimeInput.value;

    const endTime = endTimeInput.value;

    const checkInOpens = checkInOpensInput.value;

    const periodEnds = periodEndsInput.value;


    if (
        !courseId ||
        !sessionDate ||
        !lecturer ||
        !startTime ||
        !endTime ||
        !checkInOpens ||
        !periodEnds
    ) {
        alert("Please complete all required fields.");
        return false;
    }


    if (!Number.isInteger(sessionNumber) || sessionNumber < 1) {

        alert("Session number must be a positive integer.");

        sessionNumberInput.focus();

        return false;
    }


    if (lecturer !== getLecturerName(courseId)) {

        alert("The selected lecturer does not match the course assignment.");

        sessionLecturerInput.focus();

        return false;
    }


    if (startTime >= endTime) {

        alert("End time must be later than start time.");

        endTimeInput.focus();

        return false;
    }


    if (
        checkInOpens < startTime ||
        checkInOpens >= endTime
    ) {

        alert(
            "Check-in opening time must be within the class schedule."
        );

        checkInOpensInput.focus();

        return false;
    }


    if (
        periodEnds <= checkInOpens ||
        periodEnds > endTime
    ) {

        alert(
            "Attendance period end must be later than check-in opening time and cannot exceed class end time."
        );

        periodEndsInput.focus();

        return false;
    }


    return true;
}


/* =========================================
   SAVE SESSION
========================================= */

function saveSession(event) {

    event.preventDefault();

    if (!validateSessionForm()) return;

    const id = sessionIdInput.value;

    const courseId = sessionCourseInput.value;

    const scheduleData = {

        courseId,

        courseName: getCourseName(courseId),

        sessionNumber: Number(sessionNumberInput.value),

        date: sessionDateInput.value,

        lecturer: getLecturerName(courseId),

        startTime: startTimeInput.value,

        endTime: endTimeInput.value,

        checkInOpens: checkInOpensInput.value,

        periodEnds: periodEndsInput.value
    };


    /* Update existing session */

    if (id) {

        const index = sessionSchedules.findIndex(
            schedule => schedule.id === id
        );

        if (index === -1) {
            alert("Session schedule not found.");
            return;
        }

        sessionSchedules[index] = {
            ...sessionSchedules[index],
            ...scheduleData
        };

        alert("Session schedule updated successfully.");
    }

    /* Create new session */

    else {

        const newSession = {
            id: generateSessionId(),
            ...scheduleData,
            status: "Scheduled"
        };

        sessionSchedules.push(newSession);

        alert("New session schedule created successfully.");
    }


    resetSessionForm();

    renderSchedules();
}


/* Generate a demo session ID in SE001 format. */

function generateSessionId() {

    let number = 1;

    while (
        sessionSchedules.some(
            schedule => schedule.id ===
                `SE${String(number).padStart(3, "0")}`
        )
    ) {
        number++;
    }

    return `SE${String(number).padStart(3, "0")}`;
}


/* =========================================
   DELETE SESSION
========================================= */

function deleteSession(sessionId = null) {

    const id = sessionId || sessionIdInput.value;

    if (!id) {
        alert("Please select a session to delete.");
        return;
    }

    const schedule = sessionSchedules.find(
        item => item.id === id
    );

    if (!schedule) return;

    const confirmed = confirm(
        `Are you sure you want to delete the session of "${schedule.courseName}" on ${formatDate(schedule.date)}?`
    );

    if (!confirmed) return;

    sessionSchedules = sessionSchedules.filter(
        item => item.id !== id
    );

    resetSessionForm();

    renderSchedules();

    alert("Session schedule deleted successfully.");
}


/* =========================================
   TABLE ACTION HANDLER
========================================= */

scheduleTableBody.addEventListener("click", event => {

    const button = event.target.closest("button[data-action]");

    if (!button) return;

    const action = button.dataset.action;

    const id = button.dataset.id;

    if (action === "view" || action === "edit") {

        selectSession(id);

    } else if (action === "delete") {

        deleteSession(id);
    }
});


/* =========================================
   FILTER EVENTS
========================================= */

courseFilter.addEventListener("change", renderSchedules);

lecturerFilter.addEventListener("change", renderSchedules);


filterStartDate.addEventListener("change", () => {

    if (
        filterEndDate.value &&
        filterStartDate.value > filterEndDate.value
    ) {
        alert("Start date cannot be later than end date.");

        filterStartDate.value = "";

        return;
    }

    renderSchedules();
});


filterEndDate.addEventListener("change", () => {

    if (
        filterStartDate.value &&
        filterEndDate.value < filterStartDate.value
    ) {
        alert("End date cannot be earlier than start date.");

        filterEndDate.value = "";

        return;
    }

    renderSchedules();
});


/* =========================================
   BUTTON EVENTS
========================================= */

createSessionBtn.addEventListener("click", createSession);

sessionForm.addEventListener("submit", saveSession);

cancelSessionBtn.addEventListener("click", () => {

    if (sessionIdInput.value) {

        selectSession(sessionIdInput.value);

    } else {

        resetSessionForm();

        renderSchedules();
    }
});


deleteSessionBtn.addEventListener("click", () => {
    deleteSession();
});


/* =========================================
   COURSE CHANGE EVENT
========================================= */

sessionCourseInput.addEventListener(
    "change",
    updateLecturerByCourse
);


/* =========================================
   AUTOMATIC CLOSURE NOTE EVENTS
========================================= */

periodEndsInput.addEventListener(
    "change",
    updateAutomaticClosureNote
);


/* =========================================
   INITIALIZE PAGE
========================================= */

function initializePage() {

    populateCourseOptions();

    populateLecturerOptions();

    // Do not set the old November filter range.
    // Show all SQL sample sessions by default.
    filterStartDate.value = "";

    filterEndDate.value = "";

    resetSessionForm();

    renderSchedules();

    // Select the first existing session.
    if (sessionSchedules.length > 0) {
        selectSession(sessionSchedules[0].id);
    }
}


initializePage();
