
/* =====================================
   DEPARTMENTS
===================================== */

/*
    Frontend demonstration data.
    Initial data is based on data.sql.
    This version does not connect to PostgreSQL.
*/


/* =====================================
   SAMPLE DATA FROM SQL
===================================== */

let departments = [
    {
        departmentId: "D0001",
        departmentName: "Information Technology"
    },
    {
        departmentId: "D0002",
        departmentName: "Computer Science"
    },
    {
        departmentId: "D0003",
        departmentName: "Business Administration"
    }
];


/* =====================================
   DOM ELEMENTS
===================================== */

// Table
const departmentTableBody =
    document.getElementById("departmentTableBody");

const emptyState =
    document.getElementById("emptyState");


// Search
const searchDepartmentInput =
    document.getElementById("searchDepartment");


// Form
const departmentForm =
    document.getElementById("departmentForm");

const formTitle =
    document.getElementById("formTitle");

const departmentIdInput =
    document.getElementById("departmentId");

const departmentNameInput =
    document.getElementById("departmentName");


// Buttons
const createDepartmentBtn =
    document.getElementById("createDepartmentBtn");

const saveDepartmentBtn =
    document.getElementById("saveDepartmentBtn");

const cancelDepartmentBtn =
    document.getElementById("cancelDepartmentBtn");

const deleteDepartmentBtn =
    document.getElementById("deleteDepartmentBtn");


// Message
const formMessage =
    document.getElementById("formMessage");


/* =====================================
   STATE
===================================== */

let selectedDepartmentId = "D0001";

let isCreating = false;


/* =====================================
   INITIALIZATION
===================================== */

document.addEventListener("DOMContentLoaded", () => {

    if (
        !departmentTableBody ||
        !departmentForm ||
        !searchDepartmentInput
    ) {
        console.error(
            "Department page elements could not be found. Check the HTML element IDs."
        );
        return;
    }

    renderDepartments();

    selectDepartment(selectedDepartmentId);

    attachEventListeners();

});


/* =====================================
   EVENT LISTENERS
===================================== */

function attachEventListeners() {

    searchDepartmentInput.addEventListener(
        "input",
        renderDepartments
    );

    createDepartmentBtn.addEventListener(
        "click",
        startCreatingDepartment
    );

    departmentForm.addEventListener(
        "submit",
        saveDepartment
    );

    cancelDepartmentBtn.addEventListener(
        "click",
        cancelEditing
    );

    deleteDepartmentBtn.addEventListener(
        "click",
        deleteDepartment
    );

}


/* =====================================
   GET FILTERED DEPARTMENTS
===================================== */

function getFilteredDepartments() {

    const keyword = searchDepartmentInput.value
        .trim()
        .toLowerCase();

    return departments.filter(department => {

        const matchesSearch =
            department.departmentId
                .toLowerCase()
                .includes(keyword) ||

            department.departmentName
                .toLowerCase()
                .includes(keyword);

        return matchesSearch;

    });

}


/* =====================================
   RENDER DEPARTMENT TABLE
===================================== */

function renderDepartments() {

    const filteredDepartments = getFilteredDepartments();

    departmentTableBody.innerHTML = "";

    if (emptyState) {
        emptyState.hidden = filteredDepartments.length !== 0;
    }

    filteredDepartments.forEach(department => {

        const row = document.createElement("tr");

        if (
            department.departmentId === selectedDepartmentId &&
            !isCreating
        ) {
            row.classList.add("selected");
        }


        // Department name
        const nameCell = document.createElement("td");

        const departmentName =
            document.createElement("span");

        departmentName.className = "department-name";

        departmentName.textContent =
            department.departmentName;

        nameCell.appendChild(departmentName);


        // Department ID
        const idCell = document.createElement("td");

        const departmentId =
            document.createElement("span");

        departmentId.className = "department-id";

        departmentId.textContent =
            department.departmentId;

        idCell.appendChild(departmentId);


        // Actions
        const actionCell = document.createElement("td");

        const actionButtons =
            document.createElement("div");

        actionButtons.className = "action-buttons";


        const editButton = document.createElement("button");

        editButton.type = "button";

        editButton.className = "action-link";

        editButton.textContent = "View / Edit";

        editButton.addEventListener("click", () => {

            selectDepartment(department.departmentId);

        });


        actionButtons.appendChild(editButton);

        actionCell.appendChild(actionButtons);


        row.appendChild(nameCell);

        row.appendChild(idCell);

        row.appendChild(actionCell);


        row.addEventListener("click", event => {

            if (!event.target.closest("button")) {

                selectDepartment(department.departmentId);

            }

        });


        departmentTableBody.appendChild(row);

    });

}


/* =====================================
   SELECT DEPARTMENT
===================================== */

function selectDepartment(departmentId) {

    const department = departments.find(
        item => item.departmentId === departmentId
    );

    if (!department) {
        return;
    }

    selectedDepartmentId = departmentId;

    isCreating = false;

    formTitle.textContent = "Edit department";

    departmentIdInput.value = department.departmentId;

    departmentNameInput.value = department.departmentName;

    // Department ID cannot be changed when editing.
    departmentIdInput.disabled = true;

    deleteDepartmentBtn.hidden = false;

    saveDepartmentBtn.textContent = "Save changes";

    clearMessage();

    renderDepartments();

}


/* =====================================
   CREATE DEPARTMENT
===================================== */

function startCreatingDepartment() {

    isCreating = true;

    selectedDepartmentId = null;

    formTitle.textContent = "Create department";

    departmentForm.reset();

    departmentIdInput.disabled = false;

    deleteDepartmentBtn.hidden = true;

    saveDepartmentBtn.textContent = "Create";

    clearMessage();

    renderDepartments();

    departmentIdInput.focus();

}


/* =====================================
   VALIDATE DEPARTMENT
===================================== */

function validateDepartment() {

    const departmentId =
        departmentIdInput.value.trim();

    const departmentName =
        departmentNameInput.value.trim();


    // Required fields
    if (!departmentId || !departmentName) {

        showMessage(
            "Please complete all required fields.",
            true
        );

        return false;

    }


    // Department ID format: D0001
    const validId = /^D\d{4}$/.test(departmentId);

    if (!validId) {

        showMessage(
            "Department ID must follow the format D0001.",
            true
        );

        return false;

    }


    // Department name
    if (departmentName.length < 2) {

        showMessage(
            "Department name must contain at least 2 characters.",
            true
        );

        return false;

    }


    // Check duplicate Department ID
    if (isCreating) {

        const duplicateId = departments.some(
            department =>
                department.departmentId.toLowerCase() ===
                departmentId.toLowerCase()
        );

        if (duplicateId) {

            showMessage(
                "This Department ID already exists.",
                true
            );

            return false;

        }

    }


    // Check duplicate Department name
    const duplicateName = departments.some(department => {

        return (
            department.departmentName.toLowerCase() ===
                departmentName.toLowerCase() &&
            department.departmentId !== selectedDepartmentId
        );

    });

    if (duplicateName) {

        showMessage(
            "This department name already exists.",
            true
        );

        return false;

    }

    return true;

}


/* =====================================
   SAVE DEPARTMENT
===================================== */

function saveDepartment(event) {

    event.preventDefault();

    if (!validateDepartment()) {
        return;
    }

    const departmentData = {

        departmentId: departmentIdInput.value.trim(),

        departmentName: departmentNameInput.value.trim()

    };


    if (isCreating) {

        createDepartment(departmentData);

    } else {

        updateDepartment(departmentData);

    }

}


/* =====================================
   CREATE DEPARTMENT DATA
===================================== */

function createDepartment(departmentData) {

    const duplicateId = departments.some(
        department =>
            department.departmentId.toLowerCase() ===
            departmentData.departmentId.toLowerCase()
    );

    if (duplicateId) {

        showMessage(
            "This Department ID already exists.",
            true
        );

        return;

    }

    departments.push(departmentData);

    selectedDepartmentId = departmentData.departmentId;

    isCreating = false;

    selectDepartment(departmentData.departmentId);

    renderDepartments();

    showMessage("Department created successfully.");

}


/* =====================================
   UPDATE DEPARTMENT DATA
===================================== */

function updateDepartment(departmentData) {

    const departmentIndex = departments.findIndex(
        department =>
            department.departmentId === selectedDepartmentId
    );

    if (departmentIndex === -1) {

        showMessage(
            "Department not found.",
            true
        );

        return;

    }


    // Keep the original Department ID.
    departments[departmentIndex] = {

        departmentId: selectedDepartmentId,

        departmentName: departmentData.departmentName

    };


    selectDepartment(selectedDepartmentId);

    renderDepartments();

    showMessage("Department information updated successfully.");

}


/* =====================================
   DELETE DEPARTMENT
===================================== */

function deleteDepartment() {

    if (isCreating || !selectedDepartmentId) {
        return;
    }

    const department = departments.find(
        item => item.departmentId === selectedDepartmentId
    );

    if (!department) {
        return;
    }


    const confirmed = confirm(
        `Are you sure you want to delete department "${department.departmentName}"?`
    );

    if (!confirmed) {
        return;
    }


    departments = departments.filter(
        item => item.departmentId !== selectedDepartmentId
    );


    if (departments.length > 0) {

        selectDepartment(departments[0].departmentId);

    } else {

        resetEditor();

    }

    renderDepartments();

}


/* =====================================
   CANCEL EDITING
===================================== */

function cancelEditing() {

    clearMessage();

    if (isCreating) {

        if (departments.length > 0) {

            selectDepartment(departments[0].departmentId);

        } else {

            resetEditor();

        }

        return;

    }

    if (selectedDepartmentId) {

        selectDepartment(selectedDepartmentId);

    }

}


/* =====================================
   RESET EDITOR
===================================== */

function resetEditor() {

    isCreating = true;

    selectedDepartmentId = null;

    formTitle.textContent = "Create department";

    departmentForm.reset();

    departmentIdInput.disabled = false;

    deleteDepartmentBtn.hidden = true;

    saveDepartmentBtn.textContent = "Create";

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
