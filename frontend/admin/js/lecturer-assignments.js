
/* =====================================
   LECTURER ASSIGNMENTS
   Frontend demonstration data.
   Initial data is based on data.sql.
   This page does not connect to PostgreSQL.
===================================== */


/* =====================================
   SAMPLE DATA FROM SQL
===================================== */

const lecturers = [
    {
        lecturerId: "079082001234",
        lecturerName: "Nguyen Van An",
        departmentId: "D0001",
        department: "Information Technology"
    },
    {
        lecturerId: "079185002345",
        lecturerName: "Tran Thi Binh",
        departmentId: "D0002",
        department: "Computer Science"
    }
];

const courses = [
    {
        courseId: "C0001",
        courseName: "Database Systems",
        academicYear: "2026-2027",
        semester: "1",
        lecturerId: "079082001234"
    },
    {
        courseId: "C0002",
        courseName: "Software Engineering",
        academicYear: "2026-2027",
        semester: "1",
        lecturerId: "079185002345"
    }
];


/*
    Assignment IDs are frontend-only identifiers.
    The provided SQL does not contain an assignment table.
*/

let assignments = [
    {
        assignmentId: "AS001",
        lecturerId: "079082001234",
        courseId: "C0001",
        academicYear: "2026-2027",
        semester: "1"
    },
    {
        assignmentId: "AS002",
        lecturerId: "079185002345",
        courseId: "C0002",
        academicYear: "2026-2027",
        semester: "1"
    }
];


/* =====================================
   DOM ELEMENTS
===================================== */

// Table
const assignmentTableBody =
    document.getElementById("assignmentTableBody");

const assignmentCount =
    document.getElementById("assignmentCount");

const emptyState =
    document.getElementById("emptyState");


// Search and filters
const searchAssignmentInput =
    document.getElementById("searchAssignment");

const filterDepartment =
    document.getElementById("filterDepartment");

const filterAcademicYear =
    document.getElementById("filterAcademicYear");

const filterSemester =
    document.getElementById("filterSemester");


// Form
const assignmentForm =
    document.getElementById("assignmentForm");

const formTitle =
    document.getElementById("formTitle");

const lecturerIdInput =
    document.getElementById("lecturerId");

const courseIdInput =
    document.getElementById("courseId");

const academicYearInput =
    document.getElementById("academicYear");

const semesterInput =
    document.getElementById("semester");


// Buttons
const addAssignmentBtn =
    document.getElementById("addAssignmentBtn");

const cancelAssignmentBtn =
    document.getElementById("cancelAssignmentBtn");

const deleteAssignmentBtn =
    document.getElementById("deleteAssignmentBtn");


// Message
const formMessage =
    document.getElementById("formMessage");


/* =====================================
   STATE
===================================== */

let selectedAssignmentId = "AS001";
let isCreating = false;

let nextAssignmentNumber = 3;


/* =====================================
   INITIALIZATION
===================================== */

document.addEventListener("DOMContentLoaded", () => {
    populateLecturerOptions();
    populateCourseOptions();
    populateDepartmentOptions();
    populateAcademicYearOptions();

    renderAssignments();
    selectAssignment(selectedAssignmentId);

    attachEventListeners();
});


/* =====================================
   GET LECTURER
===================================== */

function getLecturer(lecturerId) {
    return lecturers.find(
        lecturer => lecturer.lecturerId === lecturerId
    );
}


/* =====================================
   GET COURSE
===================================== */

function getCourse(courseId) {
    return courses.find(
        course => course.courseId === courseId
    );
}


/* =====================================
   GET COURSE DEPARTMENT
===================================== */

/*
    The courses table has no department_id.
    Department is obtained through the lecturer
    assigned to the course.
*/

function getCourseDepartment(course) {
    if (!course) return "";

    const lecturer = getLecturer(course.lecturerId);

    return lecturer ? lecturer.department : "";
}


/* =====================================
   GENERATE ASSIGNMENT ID
===================================== */

function generateAssignmentId() {
    const assignmentId =
        `AS${String(nextAssignmentNumber).padStart(3, "0")}`;

    nextAssignmentNumber++;

    return assignmentId;
}


/* =====================================
   LECTURER OPTIONS
===================================== */

function populateLecturerOptions() {
    lecturerIdInput.innerHTML =
        '<option value="">Select lecturer</option>';

    lecturers.forEach(lecturer => {
        const option = document.createElement("option");

        option.value = lecturer.lecturerId;
        option.textContent =
            `${lecturer.lecturerName} (${lecturer.lecturerId})`;

        lecturerIdInput.appendChild(option);
    });
}


/* =====================================
   COURSE OPTIONS
===================================== */

function populateCourseOptions() {
    courseIdInput.innerHTML =
        '<option value="">Select course</option>';

    courses.forEach(course => {
        const option = document.createElement("option");

        option.value = course.courseId;
        option.textContent =
            `${course.courseName} (${course.courseId})`;

        courseIdInput.appendChild(option);
    });
}


/* =====================================
   DEPARTMENT OPTIONS
===================================== */

function populateDepartmentOptions() {
    const departments = [
        ...new Set(
            lecturers.map(lecturer => lecturer.department)
        )
    ];

    const currentValue = filterDepartment.value;

    filterDepartment.innerHTML =
        '<option value="all">All departments</option>';

    departments.forEach(department => {
        const option = document.createElement("option");

        option.value = department;
        option.textContent = department;

        filterDepartment.appendChild(option);
    });

    filterDepartment.value =
        departments.includes(currentValue)
            ? currentValue
            : "all";
}


/* =====================================
   ACADEMIC YEAR OPTIONS
===================================== */

function populateAcademicYearOptions() {
    const years = [
        ...new Set(
            assignments.map(assignment => assignment.academicYear)
        )
    ].sort();

    const currentValue = filterAcademicYear.value;

    filterAcademicYear.innerHTML =
        '<option value="all">All academic years</option>';

    years.forEach(year => {
        const option = document.createElement("option");

        option.value = year;
        option.textContent = year;

        filterAcademicYear.appendChild(option);
    });

    filterAcademicYear.value =
        years.includes(currentValue)
            ? currentValue
            : "all";
}


/* =====================================
   EVENT LISTENERS
===================================== */

function attachEventListeners() {
    searchAssignmentInput.addEventListener(
        "input",
        renderAssignments
    );

    filterDepartment.addEventListener(
        "change",
        renderAssignments
    );

    filterAcademicYear.addEventListener(
        "change",
        renderAssignments
    );

    filterSemester.addEventListener(
        "change",
        renderAssignments
    );

    addAssignmentBtn.addEventListener(
        "click",
        startCreatingAssignment
    );

    cancelAssignmentBtn.addEventListener(
        "click",
        cancelEditing
    );

    deleteAssignmentBtn.addEventListener(
        "click",
        deleteAssignment
    );

    assignmentForm.addEventListener(
        "submit",
        saveAssignment
    );
}


/* =====================================
   GET FILTERED ASSIGNMENTS
===================================== */

function getFilteredAssignments() {
    const keyword = searchAssignmentInput.value
        .trim()
        .toLowerCase();

    const selectedDepartment = filterDepartment.value;
    const selectedYear = filterAcademicYear.value;
    const selectedSemester = filterSemester.value;

    return assignments.filter(assignment => {
        const lecturer = getLecturer(assignment.lecturerId);
        const course = getCourse(assignment.courseId);

        if (!lecturer || !course) {
            return false;
        }

        const matchesSearch =
            assignment.assignmentId.toLowerCase().includes(keyword) ||
            lecturer.lecturerId.toLowerCase().includes(keyword) ||
            lecturer.lecturerName.toLowerCase().includes(keyword) ||
            course.courseId.toLowerCase().includes(keyword) ||
            course.courseName.toLowerCase().includes(keyword);

        const matchesDepartment =
            selectedDepartment === "all" ||
            lecturer.department === selectedDepartment;

        const matchesAcademicYear =
            selectedYear === "all" ||
            assignment.academicYear === selectedYear;

        const matchesSemester =
            selectedSemester === "all" ||
            assignment.semester === selectedSemester;

        return (
            matchesSearch &&
            matchesDepartment &&
            matchesAcademicYear &&
            matchesSemester
        );
    });
}


/* =====================================
   RENDER ASSIGNMENT TABLE
===================================== */

function renderAssignments() {
    const filteredAssignments = getFilteredAssignments();

    assignmentTableBody.innerHTML = "";

    assignmentCount.textContent =
        `${filteredAssignments.length} ${
            filteredAssignments.length === 1
                ? "assignment"
                : "assignments"
        }`;

    emptyState.hidden = filteredAssignments.length !== 0;

    filteredAssignments.forEach(assignment => {
        const lecturer = getLecturer(assignment.lecturerId);
        const course = getCourse(assignment.courseId);

        if (!lecturer || !course) return;

        const row = document.createElement("tr");

        if (
            assignment.assignmentId === selectedAssignmentId &&
            !isCreating
        ) {
            row.classList.add("selected");
        }

        // Lecturer
        const lecturerCell = document.createElement("td");
        const lecturerInformation = document.createElement("div");

        lecturerInformation.className = "lecturer-information";

        const lecturerName = document.createElement("strong");
        lecturerName.textContent = lecturer.lecturerName;

        const lecturerId = document.createElement("span");
        lecturerId.textContent = lecturer.lecturerId;

        lecturerInformation.appendChild(lecturerName);
        lecturerInformation.appendChild(lecturerId);
        lecturerCell.appendChild(lecturerInformation);

        // Course
        const courseCell = document.createElement("td");
        const courseInformation = document.createElement("div");

        courseInformation.className = "course-information";

        const courseName = document.createElement("strong");
        courseName.textContent = course.courseName;

        const courseId = document.createElement("span");
        courseId.textContent = course.courseId;

        courseInformation.appendChild(courseName);
        courseInformation.appendChild(courseId);
        courseCell.appendChild(courseInformation);

        // Academic year
        const academicYearCell = document.createElement("td");
        const academicYearLabel = document.createElement("span");

        academicYearLabel.className = "academic-year-label";
        academicYearLabel.textContent = assignment.academicYear;

        academicYearCell.appendChild(academicYearLabel);

        // Semester
        const semesterCell = document.createElement("td");
        const semesterLabel = document.createElement("span");

        semesterLabel.className = "semester-label";
        semesterLabel.textContent =
            `Semester ${assignment.semester}`;

        semesterCell.appendChild(semesterLabel);

        // Actions
        const actionCell = document.createElement("td");
        const actionButtons = document.createElement("div");

        actionButtons.className = "action-buttons";

        const editButton = document.createElement("button");

        editButton.type = "button";
        editButton.className = "action-link";
        editButton.textContent = "View / Edit";

        editButton.addEventListener("click", () => {
            selectAssignment(assignment.assignmentId);
        });

        const deleteButton = document.createElement("button");

        deleteButton.type = "button";
        deleteButton.className = "action-link delete-link";
        deleteButton.textContent = "Delete";

        deleteButton.addEventListener("click", () => {
            deleteAssignmentById(assignment.assignmentId);
        });

        actionButtons.appendChild(editButton);
        actionButtons.appendChild(deleteButton);
        actionCell.appendChild(actionButtons);

        row.appendChild(lecturerCell);
        row.appendChild(courseCell);
        row.appendChild(academicYearCell);
        row.appendChild(semesterCell);
        row.appendChild(actionCell);

        row.addEventListener("click", event => {
            if (!event.target.closest("button")) {
                selectAssignment(assignment.assignmentId);
            }
        });

        assignmentTableBody.appendChild(row);
    });
}


/* =====================================
   SELECT ASSIGNMENT
===================================== */

function selectAssignment(assignmentId) {
    const assignment = assignments.find(
        item => item.assignmentId === assignmentId
    );

    if (!assignment) return;

    selectedAssignmentId = assignmentId;
    isCreating = false;

    formTitle.textContent = "Edit assignment";

    lecturerIdInput.value = assignment.lecturerId;
    courseIdInput.value = assignment.courseId;
    academicYearInput.value = assignment.academicYear;
    semesterInput.value = assignment.semester;

    deleteAssignmentBtn.hidden = false;

    clearMessage();
    renderAssignments();
}


/* =====================================
   CREATE ASSIGNMENT
===================================== */

function startCreatingAssignment() {
    isCreating = true;
    selectedAssignmentId = null;

    formTitle.textContent = "Add assignment";

    assignmentForm.reset();

    deleteAssignmentBtn.hidden = true;

    clearMessage();
    renderAssignments();

    lecturerIdInput.focus();
}


/* =====================================
   VALIDATE ASSIGNMENT
===================================== */

function validateAssignment() {
    const lecturerId = lecturerIdInput.value;
    const courseId = courseIdInput.value;
    const academicYear = academicYearInput.value;
    const semester = semesterInput.value;

    if (!lecturerId || !courseId || !academicYear || !semester) {
        showMessage(
            "Please complete all required fields.",
            true
        );

        return false;
    }

    if (!getLecturer(lecturerId)) {
        showMessage("Please select a valid lecturer.", true);
        return false;
    }

    if (!getCourse(courseId)) {
        showMessage("Please select a valid course.", true);
        return false;
    }

    const duplicateAssignment = assignments.some(assignment => {
        return (
            assignment.courseId === courseId &&
            assignment.academicYear === academicYear &&
            assignment.semester === semester &&
            assignment.assignmentId !== selectedAssignmentId
        );
    });

    if (duplicateAssignment) {
        showMessage(
            "This course already has an assignment for the selected academic year and semester.",
            true
        );

        return false;
    }

    return true;
}


/* =====================================
   SAVE ASSIGNMENT
===================================== */

function saveAssignment(event) {
    event.preventDefault();

    if (!validateAssignment()) return;

    const assignmentData = {
        assignmentId: isCreating
            ? generateAssignmentId()
            : selectedAssignmentId,

        lecturerId: lecturerIdInput.value,
        courseId: courseIdInput.value,
        academicYear: academicYearInput.value,
        semester: semesterInput.value
    };

    if (isCreating) {
        createAssignment(assignmentData);
    } else {
        updateAssignment(assignmentData);
    }
}


/* =====================================
   CREATE ASSIGNMENT DATA
===================================== */

function createAssignment(assignmentData) {
    const duplicateId = assignments.some(
        assignment =>
            assignment.assignmentId === assignmentData.assignmentId
    );

    if (duplicateId) {
        showMessage(
            "Unable to generate a unique Assignment ID.",
            true
        );

        return;
    }

    assignments.push(assignmentData);

    selectedAssignmentId = assignmentData.assignmentId;
    isCreating = false;

    populateAcademicYearOptions();
    selectAssignment(assignmentData.assignmentId);

    showMessage("Assignment created successfully.");
}


/* =====================================
   UPDATE ASSIGNMENT DATA
===================================== */

function updateAssignment(assignmentData) {
    const assignmentIndex = assignments.findIndex(
        assignment =>
            assignment.assignmentId === selectedAssignmentId
    );

    if (assignmentIndex === -1) {
        showMessage("Assignment not found.", true);
        return;
    }

    assignments[assignmentIndex] = assignmentData;

    populateAcademicYearOptions();
    selectAssignment(assignmentData.assignmentId);

    showMessage("Assignment information updated successfully.");
}


/* =====================================
   DELETE ASSIGNMENT
===================================== */

function deleteAssignment() {
    if (isCreating || !selectedAssignmentId) return;

    deleteAssignmentById(selectedAssignmentId);
}


/* =====================================
   DELETE ASSIGNMENT BY ID
===================================== */

function deleteAssignmentById(assignmentId) {
    const assignment = assignments.find(
        item => item.assignmentId === assignmentId
    );

    if (!assignment) return;

    const confirmed = confirm(
        `Are you sure you want to delete assignment "${assignment.assignmentId}"?`
    );

    if (!confirmed) return;

    assignments = assignments.filter(
        item => item.assignmentId !== assignmentId
    );

    if (selectedAssignmentId === assignmentId) {
        if (assignments.length > 0) {
            selectAssignment(assignments[0].assignmentId);
        } else {
            resetEditor();
        }
    }

    populateAcademicYearOptions();
    renderAssignments();
}


/* =====================================
   CANCEL EDITING
===================================== */

function cancelEditing() {
    clearMessage();

    if (isCreating) {
        if (assignments.length > 0) {
            selectAssignment(assignments[0].assignmentId);
        } else {
            resetEditor();
        }

        return;
    }

    if (selectedAssignmentId) {
        selectAssignment(selectedAssignmentId);
    }
}


/* =====================================
   RESET EDITOR
===================================== */

function resetEditor() {
    isCreating = false;
    selectedAssignmentId = null;

    formTitle.textContent = "Add assignment";

    assignmentForm.reset();

    deleteAssignmentBtn.hidden = true;

    clearMessage();
    renderAssignments();
}


/* =====================================
   FORM MESSAGES
===================================== */

function showMessage(message, isError = false) {
    formMessage.textContent = message;

    formMessage.classList.toggle("error", isError);
}


function clearMessage() {
    formMessage.textContent = "";
    formMessage.classList.remove("error");
}
