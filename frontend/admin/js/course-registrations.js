
const students = [
    {
        id: "079205003456",
        name: "Le Minh Khang"
    },
    {
        id: "079305004567",
        name: "Pham Ngoc Mai"
    },
    {
        id: "079204005678",
        name: "Vo Quoc Huy"
    }
];

const courseNames = {
    C0001: "Database Systems",
    C0002: "Software Engineering"
};

let enrollments = [
    {
        id: "R00000000001",
        studentId: "079205003456",
        courseId: "C0001"
    },
    {
        id: "R00000000002",
        studentId: "079305004567",
        courseId: "C0001"
    },
    {
        id: "R00000000003",
        studentId: "079204005678",
        courseId: "C0001"
    },
    {
        id: "R00000000004",
        studentId: "079205003456",
        courseId: "C0002"
    },
    {
        id: "R00000000005",
        studentId: "079305004567",
        courseId: "C0002"
    }
];

let selectedEnrollmentId = "R00000000001";
let nextRegistrationNumber = 6;

const courseFilter = document.getElementById("courseFilter");
const studentSearch = document.getElementById("studentSearch");
const formCourse = document.getElementById("formCourse");
const formStudent = document.getElementById("formStudent");
const enrollmentForm = document.getElementById("enrollmentForm");

const enrollmentRows = document.getElementById("enrollmentRows");
const emptyState = document.getElementById("emptyState");
const showingCount = document.getElementById("showingCount");
const listTitle = document.getElementById("listTitle");
const totalBadge = document.getElementById("totalBadge");

const editorTitle = document.getElementById("editorTitle");
const saveEnrollmentBtn = document.getElementById("saveEnrollmentBtn");

function getStudent(studentId) {
    return students.find(student => student.id === studentId);
}

function getVisibleEnrollments() {
    const keyword = studentSearch.value.trim().toLowerCase();

    return enrollments.filter(enrollment => {
        const student = getStudent(enrollment.studentId);

        const matchesCourse =
            courseFilter.value === "all" ||
            enrollment.courseId === courseFilter.value;

        const searchText = `
            ${student?.name || ""}
            ${enrollment.studentId}
            ${courseNames[enrollment.courseId] || ""}
            ${enrollment.courseId}
        `.toLowerCase();

        return matchesCourse && searchText.includes(keyword);
    });
}

function renderEnrollments() {
    const visibleEnrollments = getVisibleEnrollments();

    listTitle.textContent =
        `Enrolled students · ${
            courseFilter.value === "all"
                ? "All courses"
                : courseFilter.value
        }`;

    totalBadge.textContent =
        `${enrollments.length} registrations`;

    enrollmentRows.innerHTML = visibleEnrollments.map(enrollment => {
        const student = getStudent(enrollment.studentId);

        return `
            <div class="enrollment-row">
                <div class="student-cell">
                    <span>
                        ${student ? student.name : "Unknown student"}
                    </span>
                    <span class="student-id">
                        ${enrollment.studentId}
                    </span>
                </div>

                <div class="course-cell">
                    ${enrollment.courseId} -
                    ${courseNames[enrollment.courseId] || "Unknown course"}
                </div>

                <div class="row-actions">
                    <button type="button" data-edit="${enrollment.id}">
                        Edit
                    </button>
                    /
                    <button type="button" data-remove="${enrollment.id}">
                        Remove
                    </button>
                </div>
            </div>
        `;
    }).join("");

    emptyState.hidden = visibleEnrollments.length > 0;

    showingCount.textContent =
        `Showing ${visibleEnrollments.length} of ${enrollments.length} enrolled students`;

    enrollmentRows.querySelectorAll("[data-edit]").forEach(button => {
        button.addEventListener("click", () => {
            selectEnrollment(button.dataset.edit);
        });
    });

    enrollmentRows.querySelectorAll("[data-remove]").forEach(button => {
        button.addEventListener("click", () => {
            removeEnrollment(button.dataset.remove);
        });
    });
}

function selectEnrollment(id) {
    const enrollment = enrollments.find(item => item.id === id);

    if (!enrollment) return;

    selectedEnrollmentId = id;

    editorTitle.textContent = "Edit registration";
    saveEnrollmentBtn.textContent = "Save changes";

    formCourse.value = enrollment.courseId;
    formStudent.value = enrollment.studentId;
}

function startNewEnrollment() {
    selectedEnrollmentId = null;

    editorTitle.textContent = "Add enrollment";
    saveEnrollmentBtn.textContent = "Add enrollment";

    formCourse.value =
        courseFilter.value === "all"
            ? "C0001"
            : courseFilter.value;

    formStudent.selectedIndex = 0;
}

function generateRegistrationId() {
    const registrationId =
        `R${String(nextRegistrationNumber).padStart(11, "0")}`;

    nextRegistrationNumber++;

    return registrationId;
}

function isDuplicateEnrollment(courseId, studentId, currentId = null) {
    return enrollments.some(item =>
        item.courseId === courseId &&
        item.studentId === studentId &&
        item.id !== currentId
    );
}

function removeEnrollment(id) {
    if (!confirm("Are you sure you want to remove this registration?")) {
        return;
    }

    enrollments = enrollments.filter(item => item.id !== id);

    if (selectedEnrollmentId === id) {
        selectedEnrollmentId = null;

        if (enrollments.length > 0) {
            selectEnrollment(enrollments[0].id);
        } else {
            startNewEnrollment();
        }
    }

    renderEnrollments();
}

enrollmentForm.addEventListener("submit", event => {
    event.preventDefault();

    const courseId = formCourse.value;
    const studentId = formStudent.value;

    if (isDuplicateEnrollment(
        courseId,
        studentId,
        selectedEnrollmentId
    )) {
        alert("This student is already registered for this course.");
        return;
    }

    if (selectedEnrollmentId) {
        const enrollment = enrollments.find(
            item => item.id === selectedEnrollmentId
        );

        if (!enrollment) return;

        enrollment.courseId = courseId;
        enrollment.studentId = studentId;

        alert("Registration updated successfully.");
    } else {
        const newEnrollment = {
            id: generateRegistrationId(),
            courseId,
            studentId
        };

        enrollments.push(newEnrollment);
        selectedEnrollmentId = newEnrollment.id;

        alert("Registration added successfully.");
    }

    courseFilter.value = courseId;

    renderEnrollments();
    selectEnrollment(selectedEnrollmentId);
});

document.getElementById("addEnrollmentBtn")
    .addEventListener("click", startNewEnrollment);

document.getElementById("cancelEnrollmentBtn")
    .addEventListener("click", () => {
        if (
            selectedEnrollmentId &&
            enrollments.some(item => item.id === selectedEnrollmentId)
        ) {
            selectEnrollment(selectedEnrollmentId);
        } else if (enrollments.length > 0) {
            selectEnrollment(enrollments[0].id);
        } else {
            startNewEnrollment();
        }
    });

courseFilter.addEventListener("change", renderEnrollments);
studentSearch.addEventListener("input", renderEnrollments);

renderEnrollments();
selectEnrollment(selectedEnrollmentId);
