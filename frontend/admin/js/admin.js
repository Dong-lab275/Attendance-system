
document.addEventListener("DOMContentLoaded", () => {

    const createAccountForm = document.getElementById("createAccountForm");
    const assignRoleForm = document.getElementById("assignRoleForm");

    // Update selected role appearance
    const roleOptions = document.querySelectorAll(".role-option");

    roleOptions.forEach((option) => {
        option.addEventListener("click", () => {
            roleOptions.forEach((item) => {
                item.classList.remove("selected");
            });

            option.classList.add("selected");
        });
    });

    // Create account form
    if (createAccountForm) {
        createAccountForm.addEventListener("submit", (event) => {
            event.preventDefault();

            const accountId = document.getElementById("accountId").value.trim();
            const fullName = document.getElementById("fullName").value.trim();
            const email = document.getElementById("email").value.trim();

            const selectedRole = document.querySelector(
                'input[name="role"]:checked'
            );

            if (!selectedRole) {
                alert("Please select an account role.");
                return;
            }

            const role = selectedRole.value;

            alert(
                `Account information:\n` +
                `ID: ${accountId}\n` +
                `Name: ${fullName}\n` +
                `Email: ${email}\n` +
                `Role: ${role}`
            );
        });
    }

    // Assign role form
    if (assignRoleForm) {
        assignRoleForm.addEventListener("submit", (event) => {
            event.preventDefault();

            const account = document.getElementById("selectedAccount").value;
            const role = document.getElementById("assignedRole").value;

            alert(
                `Role assignment:\n` +
                `Account: ${account}\n` +
                `New role: ${role}`
            );
        });
    }

});
