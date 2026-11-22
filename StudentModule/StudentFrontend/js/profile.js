
const studentId = "ST001";

let currentProfile = null;

function getElement(id) {
    return document.getElementById(id);
}

function showValue(value) {
    if (value === null || value === undefined || value === "") {
        return "Not provided";
    }

    return value;
}

function setText(id, value) {
    const element = getElement(id);

    if (element) {
        element.textContent = value;
    }
}

// Display student information
function displayProfile(student) {
    currentProfile = student;

    setText("studentId", student.studentId);
    setText("studentName", showValue(student.fullName));
    setText("headerStudentName", showValue(student.fullName));
    setText("fullName", showValue(student.fullName));
    setText("dateOfBirth", showValue(student.dateOfBirth));
    setText("gender", showValue(student.gender));
    setText("emailAddress", showValue(student.emailAddress));
    setText("phoneNumber", showValue(student.phoneNumber));
    setText("address", showValue(student.address));

    setText(
        "studentSummary",
        student.studentId + " · Student"
    );
}

// Load student profile from C++ backend
async function loadProfile() {
    try {
        const response = await fetch(
            "/api/student-profile?studentId=" +
            encodeURIComponent(studentId),
            { cache: "no-store" }
        );

        if (!response.ok) {
            throw new Error("Cannot load student profile.");
        }

        const student = await response.json();

        displayProfile(student);

        return true;

    } catch (error) {
        console.error("Load profile error:", error);

        setText("headerStudentName", "Unable to load");

        return false;
    }
}

// Open edit form
function openEditForm() {
    if (!currentProfile) {
        alert("Student information is not loaded yet.");
        return;
    }

    const editFormCard = getElement("editFormCard");

    if (!editFormCard) {
        alert("Edit form not found.");
        return;
    }

    getElement("editFullName").value =
        currentProfile.fullName || "";

    getElement("editDateOfBirth").value =
        currentProfile.dateOfBirth || "";

    getElement("editGender").value =
        currentProfile.gender || "";

    getElement("editEmail").value =
        currentProfile.emailAddress || "";

    getElement("editPhone").value =
        currentProfile.phoneNumber || "";

    getElement("editAddress").value =
        currentProfile.address || "";

    editFormCard.hidden = false;

    editFormCard.scrollIntoView({
        behavior: "smooth",
        block: "start"
    });
}

// Close edit form
function closeEditForm() {
    const editFormCard = getElement("editFormCard");

    if (editFormCard) {
        editFormCard.hidden = true;
    }
}

// Save student profile
async function saveProfile(event) {
    event.preventDefault();

    const profileForm = getElement("profileForm");

    if (!profileForm) {
        return;
    }

    const saveButton = profileForm.querySelector(
        'button[type="submit"]'
    );

    const profile = {
        studentId: studentId,

        fullName:
            getElement("editFullName").value.trim(),

        dateOfBirth:
            getElement("editDateOfBirth").value,

        gender:
            getElement("editGender").value,

        emailAddress:
            getElement("editEmail").value.trim(),

        phoneNumber:
            getElement("editPhone").value.trim(),

        address:
            getElement("editAddress").value.trim()
    };

    if (!profile.fullName) {
        alert("Please enter your full name.");
        return;
    }

    const formData = new URLSearchParams();

    Object.entries(profile).forEach(([key, value]) => {
        formData.append(key, value);
    });

    if (saveButton) {
        saveButton.disabled = true;
        saveButton.textContent = "Saving...";
    }

    try {
        const response = await fetch(
            "/api/update-profile",
            {
                method: "POST",

                headers: {
                    "Content-Type":
                        "application/x-www-form-urlencoded"
                },

                body: formData.toString()
            }
        );

        const result = await response.json();

        if (!response.ok || !result.success) {
            throw new Error(
                result.message || "Cannot save profile."
            );
        }

        const loaded = await loadProfile();

        if (!loaded) {
            throw new Error(
                "Profile saved, but reloading failed."
            );
        }

        closeEditForm();

        alert("Profile updated successfully.");

    } catch (error) {
        console.error("Save profile error:", error);

        alert(error.message);

    } finally {
        if (saveButton) {
            saveButton.disabled = false;
            saveButton.textContent = "Save changes";
        }
    }
}

// Initialize profile page
document.addEventListener("DOMContentLoaded", () => {

    const editButton = getElement("editButton");

    const cancelButton =
        getElement("cancelEditButton");

    const profileForm =
        getElement("profileForm");

    const editFormCard =
        getElement("editFormCard");

    // Hide edit form when page loads
    if (editFormCard) {
        editFormCard.hidden = true;
    }

    // Edit button
    if (editButton) {
        editButton.addEventListener(
            "click",
            openEditForm
        );
    }

    // Cancel button
    if (cancelButton) {
        cancelButton.addEventListener(
            "click",
            closeEditForm
        );
    }

    // Save button
    if (profileForm) {
        profileForm.addEventListener(
            "submit",
            saveProfile
        );
    }

    // Load student information
    loadProfile();
});
