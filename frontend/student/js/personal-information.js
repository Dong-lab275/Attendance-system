
/* =========================================
   STUDENT PERSONAL INFORMATION
========================================= */

// Student account information
const studentInfo = {
    studentId: "079205003456",
    fullName: "Le Minh Khang",
    dateOfBirth: "14 Feb 2005",
    gender: "Male",
    emailAddress: "khang.le@student.edu.vn",
    phoneNumber: "0910000001",
    address: "Thu Duc, Ho Chi Minh City"
};

let isEditing = false;


/* =========================================
   ELEMENT REFERENCES
========================================= */

const editableFields = [
    {
        id: "studentDateOfBirth",
        key: "dateOfBirth",
        type: "date"
    },
    {
        id: "studentGender",
        key: "gender",
        type: "select",
        options: ["Male", "Female", "Other"]
    },
    {
        id: "studentEmail",
        key: "emailAddress",
        type: "email"
    },
    {
        id: "studentPhone",
        key: "phoneNumber",
        type: "tel"
    },
    {
        id: "studentAddress",
        key: "address",
        type: "text"
    }
];


/* =========================================
   DATE CONVERSION
========================================= */

// Convert "14 Feb 2005" to "2005-02-14"
function convertDateToInput(dateString) {

    const [day, month, year] = dateString.split(" ");

    const monthMap = {
        Jan: "01",
        Feb: "02",
        Mar: "03",
        Apr: "04",
        May: "05",
        Jun: "06",
        Jul: "07",
        Aug: "08",
        Sep: "09",
        Oct: "10",
        Nov: "11",
        Dec: "12"
    };

    if (!day || !monthMap[month] || !year) {
        return "";
    }

    return `${year}-${monthMap[month]}-${day.padStart(2, "0")}`;
}


// Convert "2005-02-14" to "14 Feb 2005"
function formatDate(dateString) {

    if (!dateString) {
        return "";
    }

    const [year, month, day] = dateString.split("-");

    const date = new Date(
        Number(year),
        Number(month) - 1,
        Number(day)
    );

    return date.toLocaleDateString("en-GB", {
        day: "2-digit",
        month: "short",
        year: "numeric"
    });
}


/* =========================================
   DISPLAY STUDENT INFORMATION
========================================= */

function renderStudentInfo() {

    // Student ID
    document.querySelectorAll(".js-student-id")
        .forEach(element => {
            element.textContent = studentInfo.studentId;
        });

    // Full name
    document.querySelectorAll(".js-full-name")
        .forEach(element => {
            element.textContent = studentInfo.fullName;
        });

    // Date of birth
    const dateOfBirthElement =
        document.getElementById("studentDateOfBirth");

    if (dateOfBirthElement) {
        dateOfBirthElement.textContent =
            studentInfo.dateOfBirth;
    }

    // Gender
    const genderElement =
        document.getElementById("studentGender");

    if (genderElement) {
        genderElement.textContent =
            studentInfo.gender;
    }

    // Email address
    const emailElement =
        document.getElementById("studentEmail");

    if (emailElement) {
        emailElement.textContent =
            studentInfo.emailAddress;
    }

    // Phone number
    const phoneElement =
        document.getElementById("studentPhone");

    if (phoneElement) {
        phoneElement.textContent =
            studentInfo.phoneNumber;
    }

    // Address
    const addressElement =
        document.getElementById("studentAddress");

    if (addressElement) {
        addressElement.textContent =
            studentInfo.address;
    }
}


/* =========================================
   CREATE EDIT INPUT
========================================= */

function createEditInput(field) {

    const element = document.getElementById(field.id);

    if (!element) {
        return;
    }

    let input;

    // Gender selection
    if (field.type === "select") {

        input = document.createElement("select");

        field.options.forEach(option => {

            const optionElement =
                document.createElement("option");

            optionElement.value = option;
            optionElement.textContent = option;

            if (option === studentInfo[field.key]) {
                optionElement.selected = true;
            }

            input.appendChild(optionElement);

        });

    } else {

        input = document.createElement("input");

        input.type = field.type;

        if (field.type === "date") {
            input.value = convertDateToInput(
                studentInfo[field.key]
            );
        } else {
            input.value = studentInfo[field.key];
        }

    }

    input.className = "personal-edit-input";
    input.dataset.field = field.key;

    if (field.key !== "address") {
        input.required = true;
    }

    if (field.type === "email") {
        input.autocomplete = "email";
    }

    if (field.type === "tel") {
        input.autocomplete = "tel";
    }

    element.textContent = "";
    element.appendChild(input);
}


/* =========================================
   ENTER EDIT MODE
========================================= */

function enterEditMode() {

    if (isEditing) {
        return;
    }

    isEditing = true;

    // Change editable fields into inputs
    editableFields.forEach(field => {
        createEditInput(field);
    });

    // Change button label
    const editButton =
        document.getElementById("editInformationButton");

    if (editButton) {
        editButton.textContent = "Save";
        editButton.classList.add("save-information-button");
    }

    // Create Cancel button
    createCancelButton();

}


/* =========================================
   CREATE CANCEL BUTTON
========================================= */

function createCancelButton() {

    const editButton =
        document.getElementById("editInformationButton");

    if (!editButton ||
        document.getElementById("cancelInformationButton")) {
        return;
    }

    const cancelButton =
        document.createElement("button");

    cancelButton.type = "button";
    cancelButton.id = "cancelInformationButton";
    cancelButton.className = "cancel-information-button";
    cancelButton.textContent = "Cancel";

    editButton.insertAdjacentElement("afterend", cancelButton);

    cancelButton.addEventListener("click", cancelEdit);

}


/* =========================================
   VALIDATE STUDENT INFORMATION
========================================= */

function validateStudentInfo() {

    const email = document
        .getElementById("studentEmail")
        ?.querySelector("input");

    const phone = document
        .getElementById("studentPhone")
        ?.querySelector("input");

    const address = document
        .getElementById("studentAddress")
        ?.querySelector("input");

    const dateOfBirth = document
        .getElementById("studentDateOfBirth")
        ?.querySelector("input");

    if (dateOfBirth && !dateOfBirth.value) {
        alert("Please enter your date of birth.");
        dateOfBirth.focus();
        return false;
    }

    if (dateOfBirth && dateOfBirth.value) {

        const selectedDate = new Date(
            dateOfBirth.value + "T00:00:00"
        );

        const today = new Date();
        today.setHours(0, 0, 0, 0);

        if (selectedDate > today) {
            alert("Date of birth cannot be in the future.");
            dateOfBirth.focus();
            return false;
        }
    }

    // Validate email
    if (email) {

        const emailPattern =
            /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

        if (!emailPattern.test(email.value.trim())) {
            alert("Please enter a valid email address.");
            email.focus();
            return false;
        }

    }

    // Validate phone number
    if (phone) {

        const phoneDigits =
            phone.value.replace(/\D/g, "");

        if (phoneDigits.length < 8 ||
            phoneDigits.length > 15) {

            alert("Phone number must contain between 8 and 15 digits.");
            phone.focus();
            return false;
        }

    }

    // Validate address
    if (address && !address.value.trim()) {
        alert("Address cannot be empty.");
        address.focus();
        return false;
    }

    return true;
}


/* =========================================
   SAVE STUDENT INFORMATION
========================================= */

function saveStudentInfo() {

    if (!validateStudentInfo()) {
        return;
    }

    // Get updated editable fields
    const updatedInfo = {};

    editableFields.forEach(field => {

        const element =
            document.getElementById(field.id);

        if (!element) {
            return;
        }

        const input = element.querySelector("input, select");

        if (!input) {
            return;
        }

        if (field.key === "dateOfBirth") {
            updatedInfo[field.key] =
                formatDate(input.value);
        } else {
            updatedInfo[field.key] =
                input.value.trim();
        }

    });

    // Update local student information
    Object.assign(studentInfo, updatedInfo);

    // Exit edit mode
    isEditing = false;

    // Render updated information
    renderStudentInfo();

    // Restore buttons
    resetEditButtons();

    alert("Student information updated successfully.");

}


/* =========================================
   CANCEL EDIT
========================================= */

function cancelEdit() {

    if (!isEditing) {
        return;
    }

    const confirmCancel = confirm(
        "Are you sure you want to cancel your changes?"
    );

    if (!confirmCancel) {
        return;
    }

    isEditing = false;

    // Restore original information
    renderStudentInfo();

    // Restore buttons
    resetEditButtons();

}


/* =========================================
   RESET EDIT BUTTONS
========================================= */

function resetEditButtons() {

    const editButton =
        document.getElementById("editInformationButton");

    const cancelButton =
        document.getElementById("cancelInformationButton");

    if (editButton) {
        editButton.textContent = "Edit information";
        editButton.classList.remove("save-information-button");
    }

    if (cancelButton) {
        cancelButton.remove();
    }

}


/* =========================================
   HANDLE EDIT BUTTON
========================================= */

function handleEditButton() {

    if (isEditing) {
        saveStudentInfo();
    } else {
        enterEditMode();
    }

}


/* =========================================
   INITIALIZE PAGE
========================================= */

document.addEventListener("DOMContentLoaded", () => {

    // Display initial student information
    renderStudentInfo();

    // Edit / Save button
    const editButton =
        document.getElementById("editInformationButton");

    if (editButton) {
        editButton.addEventListener(
            "click",
            handleEditButton
        );
    }

});
