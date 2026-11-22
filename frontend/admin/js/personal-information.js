
/* =========================================
   ADMINISTRATOR PERSONAL INFORMATION
   Synchronized with data.sql
========================================= */

// Administrator account information
const administratorInfo = {
    accountId: "079080000001",
    fullName: "System Administrator",
    emailAddress: "admin@attendance.edu.vn",
    role: "Administrator"
};


/* =========================================
   DISPLAY ADMINISTRATOR INFORMATION
========================================= */

function renderAdministratorInfo() {

    const accountIdElements =
        document.querySelectorAll(".js-account-id");

    const fullNameElements =
        document.querySelectorAll(".js-full-name");

    const roleElements =
        document.querySelectorAll(".js-administrator-role");

    const emailElement =
        document.getElementById("administratorEmail");


    // Display account ID
    accountIdElements.forEach((element) => {
        element.textContent = administratorInfo.accountId;
    });


    // Display full name
    fullNameElements.forEach((element) => {
        element.textContent = administratorInfo.fullName;
    });


    // Display email address
    if (emailElement) {
        emailElement.textContent = administratorInfo.emailAddress;
    }


    // Display role
    roleElements.forEach((element) => {
        element.textContent = administratorInfo.role;
    });

}


/* =========================================
   INITIALIZE PAGE
========================================= */

document.addEventListener("DOMContentLoaded", () => {
    renderAdministratorInfo();
});
