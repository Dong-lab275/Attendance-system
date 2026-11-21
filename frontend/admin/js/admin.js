
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
createAccountForm.addEventListener("submit", (event) => {
    event.preventDefault();

    const accountId = document.getElementById("accountId").value.trim();
    const fullName = document.getElementById("fullName").value.trim();
    const email = document.getElementById("email").value.trim();
    const role = document.querySelector('input[name="role"]:checked').value;

    alert(
        `Account information:\n` +
        `ID: ${accountId}\n` +
        `Name: ${fullName}\n` +
        `Email: ${email}\n` +
        `Role: ${role}`
    );
});

// Assign role form
assignRoleForm.addEventListener("submit", (event) => {
    event.preventDefault();

    const account = document.getElementById("selectedAccount").value;
    const role = document.getElementById("assignedRole").value;

    alert(`Role assignment:\nAccount: ${account}\nNew role: ${role}`);
});


const navItems = document.querySelectorAll(".sidebar-nav .nav-item");

navItems.forEach((item) => {
    item.addEventListener("click", (event) => {
        event.preventDefault();

        navItems.forEach((nav) => {
            nav.classList.remove("active");
        });

        item.classList.add("active");
    });
});
