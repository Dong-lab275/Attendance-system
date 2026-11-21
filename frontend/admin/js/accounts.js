
const searchInput = document.getElementById("searchAccount");
const roleFilter = document.getElementById("filterRole");
const statusFilter = document.getElementById("filterStatus");

const accountRows = document.querySelectorAll("#accountTableBody tr");
const accountCount = document.getElementById("accountCount");
const emptyState = document.getElementById("emptyState");

let selectedAccountId = "SV2024001";

const accountData = {
    SV2024001: {
        name: "Nguyễn Minh Anh",
        email: "minhanh@example.com",
        role: "student"
    },
    SV2024002: {
        name: "Trần Đức Anh",
        email: "ducanh@example.com",
        role: "student"
    },
    GV001: {
        name: "Trần Quốc Huy",
        email: "quochuy@example.com",
        role: "lecturer"
    },
    GV002: {
        name: "Lê Thanh Hà",
        email: "thanhha@example.com",
        role: "lecturer"
    }
};

// SEARCH AND FILTER
function filterAccounts() {
    const keyword = searchInput.value.trim().toLowerCase();
    const selectedRole = roleFilter.value;
    const selectedStatus = statusFilter.value;

    let visibleCount = 0;

    accountRows.forEach((row) => {
        const name = row.dataset.name.toLowerCase();
        const id = row.dataset.id.toLowerCase();
        const role = row.dataset.role;
        const status = row.dataset.status;

        const matchesKeyword =
            name.includes(keyword) || id.includes(keyword);

        const matchesRole =
            selectedRole === "all" || role === selectedRole;

        const matchesStatus =
            selectedStatus === "all" || status === selectedStatus;

        const isVisible =
            matchesKeyword && matchesRole && matchesStatus;

        row.style.display = isVisible ? "" : "none";

        if (isVisible) {
            visibleCount++;
        }
    });

    accountCount.textContent = `${visibleCount} accounts`;
    emptyState.style.display = visibleCount === 0 ? "block" : "none";
}

searchInput.addEventListener("input", filterAccounts);
roleFilter.addEventListener("change", filterAccounts);
statusFilter.addEventListener("change", filterAccounts);

// SELECT ACCOUNT
function selectAccount(accountId) {
    const data = accountData[accountId];

    if (!data) return;

    selectedAccountId = accountId;

    document.getElementById("selectedName").textContent = data.name;
    document.getElementById("selectedId").textContent =
        `${accountId} · ${data.role === "student" ? "Student" : "Lecturer"}`;

    document.getElementById("accountId").value = accountId;
    document.getElementById("fullName").value = data.name;
    document.getElementById("emailAddress").value = data.email;
    document.getElementById("accountRole").value = data.role;

    const initials = data.name
        .split(" ")
        .map(part => part[0])
        .slice(-2)
        .join("")
        .toUpperCase();

    document.getElementById("selectedAvatar").textContent = initials;
}

// VIEW / EDIT BUTTONS
document.querySelectorAll(".view-edit-btn").forEach((button) => {
    button.addEventListener("click", () => {
        selectAccount(button.dataset.accountId);
    });
});

// EDIT BUTTON
document.getElementById("editAccountBtn").addEventListener("click", () => {
    document.getElementById("fullName").focus();
});

// SAVE CHANGES
document.getElementById("editAccountForm").addEventListener("submit", (event) => {
    event.preventDefault();

    const newName = document.getElementById("fullName").value.trim();
    const newEmail = document.getElementById("emailAddress").value.trim();

    if (!newName || !newEmail) {
        alert("Please enter all required information.");
        return;
    }

    accountData[selectedAccountId].name = newName;
    accountData[selectedAccountId].email = newEmail;

    const selectedRow = [...accountRows].find(
        row => row.dataset.id === selectedAccountId
    );

    if (selectedRow) {
        selectedRow.dataset.name = newName;
        selectedRow.querySelector(".user-cell strong").textContent = newName;
    }

    document.getElementById("selectedName").textContent = newName;

    alert("Account information updated successfully.");
    filterAccounts();
});

// CANCEL EDIT
document.getElementById("cancelEditBtn").addEventListener("click", () => {
    selectAccount(selectedAccountId);
});

// LOCK ACCOUNT
document.getElementById("lockAccountBtn").addEventListener("click", () => {
    const row = [...accountRows].find(
        item => item.dataset.id === selectedAccountId
    );

    if (!row) return;

    if (row.dataset.status === "locked") {
        alert("This account is already locked.");
        return;
    }

    const confirmed = confirm("Are you sure you want to lock this account?");

    if (confirmed) {
        row.dataset.status = "locked";
        row.querySelector(".status-badge").textContent = "Locked";
        row.querySelector(".status-badge").className =
            "status-badge locked-status";

        alert("Account locked successfully.");
        filterAccounts();
    }
});

// RESET PASSWORD
document.getElementById("resetPasswordBtn").addEventListener("click", () => {
    const confirmed = confirm(
        "Are you sure you want to reset this account's password?"
    );

    if (confirmed) {
        alert("Password reset successfully. (Demo only)");
    }
});

// DELETE ACCOUNT
document.getElementById("deleteAccountBtn").addEventListener("click", () => {
    const confirmed = confirm(
        "Are you sure you want to delete this account?"
    );

    if (!confirmed) return;

    const row = [...accountRows].find(
        item => item.dataset.id === selectedAccountId
    );

    if (row) {
        row.remove();
    }

    delete accountData[selectedAccountId];

    selectedAccountId = "";

    document.getElementById("selectedName").textContent = "No account selected";
    document.getElementById("selectedId").textContent = "";
    document.getElementById("accountId").value = "";
    document.getElementById("fullName").value = "";
    document.getElementById("emailAddress").value = "";

    filterAccounts();
});

// CREATE ACCOUNT
document.getElementById("createAccountBtn").addEventListener("click", () => {
    alert("The Create Account function will be implemented in the next step.");
});

// INITIALIZE
selectAccount("SV2024001");
filterAccounts();
