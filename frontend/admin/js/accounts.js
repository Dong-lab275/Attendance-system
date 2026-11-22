
const searchInput = document.getElementById("searchAccount");
const roleFilter = document.getElementById("filterRole");
const statusFilter = document.getElementById("filterStatus");

const accountTableBody = document.getElementById("accountTableBody");
const accountCount = document.getElementById("accountCount");
const emptyState = document.getElementById("emptyState");

let selectedAccountId = "079205003456";

// ACCOUNT DATA - MATCHED WITH data.sql
const accountData = {
    "079205003456": {
        username: "student01",
        name: "Le Minh Khang",
        email: "khang.le@student.edu.vn",
        role: "student",
        status: "active"
    },

    "079305004567": {
        username: "student02",
        name: "Pham Ngoc Mai",
        email: "mai.pham@student.edu.vn",
        role: "student",
        status: "active"
    },

    "079204005678": {
        username: "student03",
        name: "Vo Quoc Huy",
        email: "huy.vo@student.edu.vn",
        role: "student",
        status: "active"
    },

    "079082001234": {
        username: "lecturer01",
        name: "Nguyen Van An",
        email: "an.nguyen@attendance.edu.vn",
        role: "lecturer",
        status: "active"
    },

    "079185002345": {
        username: "lecturer02",
        name: "Tran Thi Binh",
        email: "binh.tran@attendance.edu.vn",
        role: "lecturer",
        status: "active"
    }
};

// GET CURRENT ACCOUNT ROWS
function getAccountRows() {
    return Array.from(
        accountTableBody.querySelectorAll("tr")
    );
}

// SEARCH AND FILTER
function filterAccounts() {
    const keyword = searchInput.value.trim().toLowerCase();
    const selectedRole = roleFilter.value;
    const selectedStatus = statusFilter.value;

    let visibleCount = 0;

    getAccountRows().forEach((row) => {
        const name = (row.dataset.name || "").toLowerCase();
        const id = (row.dataset.id || "").toLowerCase();
        const username = (row.dataset.username || "").toLowerCase();
        const role = row.dataset.role;
        const status = row.dataset.status;

        const matchesKeyword =
            name.includes(keyword) ||
            id.includes(keyword) ||
            username.includes(keyword);

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
    emptyState.style.display =
        visibleCount === 0 ? "block" : "none";
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

    // Update selected row
    getAccountRows().forEach((row) => {
        row.classList.toggle(
            "selected",
            row.dataset.id === accountId
        );
    });
}

// VIEW / EDIT BUTTONS
accountTableBody.addEventListener("click", (event) => {
    const button = event.target.closest(".view-edit-btn");

    if (!button) return;

    selectAccount(button.dataset.accountId);
});

// EDIT BUTTON
document.getElementById("editAccountBtn").addEventListener("click", () => {
    document.getElementById("fullName").focus();
});

// SAVE CHANGES
document.getElementById("editAccountForm").addEventListener("submit", (event) => {
    event.preventDefault();

    if (!selectedAccountId || !accountData[selectedAccountId]) {
        alert("Please select an account first.");
        return;
    }

    const newName = document.getElementById("fullName").value.trim();
    const newEmail = document.getElementById("emailAddress").value.trim();

    if (!newName || !newEmail) {
        alert("Please enter all required information.");
        return;
    }

    // Update local account data
    accountData[selectedAccountId].name = newName;
    accountData[selectedAccountId].email = newEmail;

    const selectedRow = getAccountRows().find(
        row => row.dataset.id === selectedAccountId
    );

    if (selectedRow) {
        selectedRow.dataset.name = newName;

        const nameElement = selectedRow.querySelector(".user-cell strong");

        if (nameElement) {
            nameElement.textContent = newName;
        }
    }

    document.getElementById("selectedName").textContent = newName;

    // Update avatar
    const initials = newName
        .split(" ")
        .map(part => part[0])
        .slice(-2)
        .join("")
        .toUpperCase();

    document.getElementById("selectedAvatar").textContent = initials;

    alert("Account information updated successfully.");

    filterAccounts();
});

// CANCEL EDIT
document.getElementById("cancelEditBtn").addEventListener("click", () => {
    if (selectedAccountId && accountData[selectedAccountId]) {
        selectAccount(selectedAccountId);
    }
});

// LOCK ACCOUNT
document.getElementById("lockAccountBtn").addEventListener("click", () => {
    if (!selectedAccountId || !accountData[selectedAccountId]) {
        alert("Please select an account first.");
        return;
    }

    const row = getAccountRows().find(
        item => item.dataset.id === selectedAccountId
    );

    if (!row) return;

    if (row.dataset.status === "locked") {
        alert("This account is already locked.");
        return;
    }

    const confirmed = confirm(
        "Are you sure you want to lock this account?"
    );

    if (!confirmed) return;

    row.dataset.status = "locked";
    accountData[selectedAccountId].status = "locked";

    const statusBadge = row.querySelector(".status-badge");

    if (statusBadge) {
        statusBadge.textContent = "Locked";
        statusBadge.className = "status-badge locked-status";
    }

    alert("Account locked successfully.");

    filterAccounts();
});

// RESET PASSWORD
document.getElementById("resetPasswordBtn").addEventListener("click", () => {
    if (!selectedAccountId || !accountData[selectedAccountId]) {
        alert("Please select an account first.");
        return;
    }

    const confirmed = confirm(
        "Are you sure you want to reset this account's password?"
    );

    if (confirmed) {
        alert("Password reset successfully. (Demo only)");
    }
});

// DELETE ACCOUNT
document.getElementById("deleteAccountBtn").addEventListener("click", () => {
    if (!selectedAccountId || !accountData[selectedAccountId]) {
        alert("Please select an account first.");
        return;
    }

    const confirmed = confirm(
        "Are you sure you want to delete this account?"
    );

    if (!confirmed) return;

    const row = getAccountRows().find(
        item => item.dataset.id === selectedAccountId
    );

    if (row) {
        row.remove();
    }

    delete accountData[selectedAccountId];

    selectedAccountId = "";

    document.getElementById("selectedName").textContent =
        "No account selected";

    document.getElementById("selectedId").textContent = "";
    document.getElementById("accountId").value = "";
    document.getElementById("fullName").value = "";
    document.getElementById("emailAddress").value = "";
    document.getElementById("accountRole").value = "";

    document.getElementById("selectedAvatar").textContent = "";

    filterAccounts();
});

// CREATE ACCOUNT
document.getElementById("createAccountBtn").addEventListener("click", () => {
    alert("The Create Account function will be implemented in the next step.");
});

// INITIALIZE
selectAccount(selectedAccountId);
filterAccounts();
