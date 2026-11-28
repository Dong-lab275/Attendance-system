
/* =====================================
   COURSE MANAGEMENT
===================================== */

/*
    Frontend demonstration data.
    Data is matched with data.sql.
    This version does not connect to PostgreSQL.
*/

let courses = [
    {
        courseId: "C0001",
        courseName: "Database Systems",
        department: "Information Technology",
        academicYear: "2026-2027",
        semester: 1,
        lecturerId: "079082001234",
        lecturerName: "Nguyen Van An"
    },
    {
        courseId: "C0002",
        courseName: "Software Engineering",
        department: "Computer Science",
        academicYear: "2026-2027",
        semester: 1,
        lecturerId: "079185002345",
        lecturerName: "Tran Thi Binh"
    }
];


/* =====================================
   DOM ELEMENTS
===================================== */

const courseTableBody = document.getElementById("courseTableBody");
const courseCount = document.getElementById("courseCount");
const emptyState = document.getElementById("emptyState");

const searchCourseInput = document.getElementById("searchCourse");
const filterDepartment = document.getElementById("filterDepartment");

const courseForm = document.getElementById("courseForm");
const formTitle = document.getElementById("formTitle");

const courseIdInput = document.getElementById("courseId");
const courseNameInput = document.getElementById("courseName");
const departmentInput = document.getElementById("department");

const addCourseBtn = document.getElementById("addCourseBtn");
const cancelCourseBtn = document.getElementById("cancelCourseBtn");
const deleteCourseBtn = document.getElementById("deleteCourseBtn");

const formMessage = document.getElementById("formMessage");


/* =====================================
   STATE
===================================== */

let selectedCourseId = "C0001";
let isCreating = false;


/* =====================================
   INITIALIZATION
===================================== */

document.addEventListener("DOMContentLoaded", () => {
    populateDepartmentOptions();
    renderCourses();
    selectCourse(selectedCourseId);
    attachEventListeners();
});


/* =====================================
   DEPARTMENT OPTIONS
===================================== */

function populateDepartmentOptions() {
    const departments = [
        ...new Set(courses.map(course => course.department))
    ];

    filterDepartment.innerHTML =
        '<option value="all">All departments</option>';

    departmentInput.innerHTML =
        '<option value="">Select department</option>';

    departments.forEach(department => {
        const filterOption = document.createElement("option");

        filterOption.value = department;
        filterOption.textContent = department;

        filterDepartment.appendChild(filterOption);

        const formOption = document.createElement("option");

        formOption.value = department;
        formOption.textContent = department;

        departmentInput.appendChild(formOption);
    });
}


/* =====================================
   EVENT LISTENERS
===================================== */

function attachEventListeners() {
    searchCourseInput.addEventListener("input", renderCourses);

    filterDepartment.addEventListener("change", renderCourses);

    addCourseBtn.addEventListener("click", startCreatingCourse);

    cancelCourseBtn.addEventListener("click", cancelEditing);

    deleteCourseBtn.addEventListener("click", deleteCourse);

    courseForm.addEventListener("submit", saveCourse);
}


/* =====================================
   GET FILTERED COURSES
===================================== */

function getFilteredCourses() {
    const keyword = searchCourseInput.value
        .trim()
        .toLowerCase();

    const selectedDepartment = filterDepartment.value;

    return courses.filter(course => {
        const matchesSearch =
            course.courseId.toLowerCase().includes(keyword) ||
            course.courseName.toLowerCase().includes(keyword) ||
            course.lecturerName.toLowerCase().includes(keyword);

        const matchesDepartment =
            selectedDepartment === "all" ||
            course.department === selectedDepartment;

        return matchesSearch && matchesDepartment;
    });
}


/* =====================================
   RENDER COURSE TABLE
===================================== */

function renderCourses() {
    const filteredCourses = getFilteredCourses();

    courseTableBody.innerHTML = "";

    courseCount.textContent =
        `${filteredCourses.length} ${
            filteredCourses.length === 1 ? "course" : "courses"
        }`;

    emptyState.hidden = filteredCourses.length !== 0;

    filteredCourses.forEach(course => {
        const row = document.createElement("tr");

        if (course.courseId === selectedCourseId && !isCreating) {
            row.classList.add("selected");
        }

        // COURSE NAME AND ID
        const courseCell = document.createElement("td");

        const courseNameContainer = document.createElement("div");
        courseNameContainer.className = "course-name";

        const courseName = document.createElement("strong");
        courseName.textContent = course.courseName;

        const courseId = document.createElement("span");
        courseId.textContent = course.courseId;

        courseNameContainer.appendChild(courseName);
        courseNameContainer.appendChild(courseId);
        courseCell.appendChild(courseNameContainer);

        // DEPARTMENT
        const departmentCell = document.createElement("td");
        departmentCell.className = "course-department";
        departmentCell.textContent = course.department;

        // ACTION
        const actionCell = document.createElement("td");

        const actionButton = document.createElement("button");
        actionButton.type = "button";
        actionButton.className = "action-link";
        actionButton.textContent = "View / Edit";

        actionButton.addEventListener("click", () => {
            selectCourse(course.courseId);
        });

        actionCell.appendChild(actionButton);

        row.appendChild(courseCell);
        row.appendChild(departmentCell);
        row.appendChild(actionCell);

        row.addEventListener("click", event => {
            if (!event.target.closest(".action-link")) {
                selectCourse(course.courseId);
            }
        });

        courseTableBody.appendChild(row);
    });
}


/* =====================================
   SELECT COURSE
===================================== */

function selectCourse(courseId) {
    const course = courses.find(
        item => item.courseId === courseId
    );

    if (!course) {
        return;
    }

    selectedCourseId = courseId;
    isCreating = false;

    formTitle.textContent = "Edit course";

    courseIdInput.value = course.courseId;
    courseNameInput.value = course.courseName;
    departmentInput.value = course.department;

    courseIdInput.disabled = true;

    deleteCourseBtn.hidden = false;

    clearMessage();
    renderCourses();
}


/* =====================================
   CREATE COURSE
===================================== */

function startCreatingCourse() {
    isCreating = true;
    selectedCourseId = null;

    formTitle.textContent = "Create course";

    courseForm.reset();

    courseIdInput.disabled = false;

    deleteCourseBtn.hidden = true;

    clearMessage();
    renderCourses();

    courseIdInput.focus();
}


/* =====================================
   SAVE COURSE
===================================== */

function saveCourse(event) {
    event.preventDefault();

    const courseId = courseIdInput.value
        .trim()
        .toUpperCase();

    const courseName = courseNameInput.value.trim();
    const department = departmentInput.value;

    if (!courseId || !courseName || !department) {
        showMessage(
            "Please complete all required fields.",
            true
        );
        return;
    }

    if (!/^[A-Z0-9]{5}$/.test(courseId)) {
        showMessage(
            "Course ID must contain exactly 5 letters or numbers.",
            true
        );
        return;
    }

    if (isCreating) {
        createCourse(courseId, courseName, department);
    } else {
        updateCourse(courseId, courseName, department);
    }
}


/* =====================================
   CREATE COURSE DATA
===================================== */

function createCourse(courseId, courseName, department) {
    const existingCourse = courses.some(
        course => course.courseId === courseId
    );

    if (existingCourse) {
        showMessage(
            "This course ID already exists.",
            true
        );
        return;
    }

    const newCourse = {
        courseId: courseId,
        courseName: courseName,
        department: department,
        academicYear: "2026-2027",
        semester: 1,
        lecturerId: "",
        lecturerName: ""
    };

    courses.push(newCourse);

    populateDepartmentOptions();

    selectedCourseId = courseId;
    isCreating = false;

    selectCourse(courseId);

    showMessage("Course created successfully.");
}


/* =====================================
   UPDATE COURSE DATA
===================================== */

function updateCourse(courseId, courseName, department) {
    const courseIndex = courses.findIndex(
        course => course.courseId === selectedCourseId
    );

    if (courseIndex === -1) {
        showMessage("Course not found.", true);
        return;
    }

    courses[courseIndex].courseName = courseName;
    courses[courseIndex].department = department;

    populateDepartmentOptions();

    selectCourse(courseId);

    showMessage("Course information updated successfully.");
}


/* =====================================
   DELETE COURSE
===================================== */

function deleteCourse() {
    if (isCreating || !selectedCourseId) {
        return;
    }

    const course = courses.find(
        item => item.courseId === selectedCourseId
    );

    if (!course) {
        return;
    }

    const confirmed = confirm(
        `Are you sure you want to delete "${course.courseName}" (${course.courseId})?`
    );

    if (!confirmed) {
        return;
    }

    courses = courses.filter(
        item => item.courseId !== selectedCourseId
    );

    selectedCourseId = null;

    populateDepartmentOptions();

    if (courses.length > 0) {
        selectCourse(courses[0].courseId);
    } else {
        resetEditor();
    }

    renderCourses();
}


/* =====================================
   CANCEL EDITING
===================================== */

function cancelEditing() {
    clearMessage();

    if (isCreating) {
        if (courses.length > 0) {
            selectCourse(courses[0].courseId);
        } else {
            resetEditor();
        }

        return;
    }

    if (selectedCourseId) {
        selectCourse(selectedCourseId);
    }
}


/* =====================================
   RESET EDITOR
===================================== */

function resetEditor() {
    isCreating = false;
    selectedCourseId = null;

    formTitle.textContent = "Edit course";

    courseForm.reset();

    courseIdInput.disabled = false;

    deleteCourseBtn.hidden = true;

    clearMessage();
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
