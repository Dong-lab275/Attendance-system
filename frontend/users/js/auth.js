
document.addEventListener("DOMContentLoaded", () => {
    const loginForm = document.getElementById("loginForm");
    const changePasswordForm = document.getElementById("changePasswordForm");
    const resetPasswordForm = document.getElementById("resetPasswordForm");

    const forgotPassword = document.getElementById("forgotPassword");
    const backToLogin = document.getElementById("backToLogin");
    const backFromReset = document.getElementById("backFromReset");

    const allForms = [
        loginForm,
        changePasswordForm,
        resetPasswordForm
    ];

    const params = new URLSearchParams(window.location.search);

    function showForm(formToShow) {
        allForms.forEach(form => {
            if (form) {
                form.hidden = form !== formToShow;
            }
        });
    }

    function clearMessages() {
        document.querySelectorAll(".error").forEach(error => {
            error.textContent = "";
        });

        document.querySelectorAll(".message").forEach(message => {
            message.textContent = "";
        });
    }

    // Automatically open Change Password form
    if (params.get("mode") === "change-password") {
        showForm(changePasswordForm);
    }

    // Forgot Password -> Reset Password
    if (forgotPassword) {
        forgotPassword.addEventListener("click", event => {
            event.preventDefault();

            resetPasswordForm.reset();
            clearMessages();
            showForm(resetPasswordForm);
        });
    }

    // Change Password -> Personal Information or Login
    if (backToLogin) {
        backToLogin.addEventListener("click", () => {
            changePasswordForm.reset();
            clearMessages();

            const returnPage = params.get("return");

            if (returnPage === "student") {
                window.location.href =
                    "../student/personal-information.html";
                return;
            }

            if (returnPage === "admin") {
                window.location.href =
                    "../admin/personal-information.html";
                return;
            }

            showForm(loginForm);
        });
    }

    // Reset Password -> Login
    if (backFromReset) {
        backFromReset.addEventListener("click", () => {
            resetPasswordForm.reset();
            clearMessages();
            showForm(loginForm);
        });
    }

    // Password visibility
    document.querySelectorAll(".toggle-password").forEach(button => {
        button.addEventListener("click", () => {
            const targetId = button.dataset.target || "password";
            const input = document.getElementById(targetId);

            if (!input) return;

            const isPassword = input.type === "password";
            input.type = isPassword ? "text" : "password";

            button.innerHTML = isPassword
                ? '<i data-lucide="eye-off"></i>'
                : '<i data-lucide="eye"></i>';

            if (window.lucide) {
                lucide.createIcons();
            }

            button.setAttribute(
                "aria-label",
                isPassword ? "Hide password" : "Show password"
            );
        });
    });

    // Login validation and backend integration
    loginForm.addEventListener("submit", async event => {
        event.preventDefault();
        clearMessages();

        const username = document.getElementById("username").value.trim();
        const password = document.getElementById("password").value;
        const loginMessage = document.getElementById("loginMessage");
        const rememberMe = document.getElementById("rememberMe");

        if (!username) {
            document.getElementById("usernameError").textContent =
                "Please enter your username.";
            return;
        }

        if (!password) {
            document.getElementById("passwordError").textContent =
                "Please enter your password.";
            return;
        }

        try {
            loginMessage.textContent = "Signing in...";

            const result = await apiRequest(
                "/auth/login",
                "POST",
                {
                    username: username,
                    password: password
                }
            );

            if (result.status !== "success" || !result.data) {
                throw new Error(result.message || "Login failed.");
            }

            const authData = result.data;

            localStorage.removeItem("samsAuth");
            sessionStorage.removeItem("samsAuth");

            const storage = rememberMe.checked
                ? localStorage
                : sessionStorage;

            storage.setItem("samsAuth", JSON.stringify({
                token: authData.token,
                userId: authData.userId,
                username: authData.username,
                role: authData.role,
                expiresIn: authData.expiresIn
            }));

            console.log("Login successful:", authData);

            if (authData.role === "Administrator") {
                window.location.href = "../admin/admin.html";
                return;
            }

            if (authData.role === "Student") {
                window.location.href =
                    "../student/attendance-check-in.html";
                return;
            }

            if (authData.role === "Lecturer") {
    window.location.href = "/ModuleLecturer/FrontEnd/html/Class-Sessions.html";
    return;
}


            throw new Error("Unknown user role.");

        } catch (error) {
            loginMessage.textContent = error.message;
            console.error("Login error:", error);
        }
    });

    // Change Password validation
    changePasswordForm.addEventListener("submit", event => {
        event.preventDefault();
        clearMessages();

        const currentPassword =
            document.getElementById("currentPassword").value;

        const newPassword =
            document.getElementById("newPassword").value;

        const confirmPassword =
            document.getElementById("confirmPassword").value;

        if (!currentPassword) {
            document.getElementById("currentPasswordError").textContent =
                "Please enter your current password.";
            return;
        }

        if (!newPassword) {
            document.getElementById("newPasswordError").textContent =
                "Please enter a new password.";
            return;
        }

        if (newPassword.length < 8) {
            document.getElementById("newPasswordError").textContent =
                "Password must contain at least 8 characters.";
            return;
        }

        if (newPassword === currentPassword) {
            document.getElementById("newPasswordError").textContent =
                "New password must be different from current password.";
            return;
        }

        if (!confirmPassword) {
            document.getElementById("confirmPasswordError").textContent =
                "Please confirm your new password.";
            return;
        }

        if (newPassword !== confirmPassword) {
            document.getElementById("confirmPasswordError").textContent =
                "Passwords do not match.";
            return;
        }

        document.getElementById("changePasswordMessage").textContent =
            "Password validation successful. Backend integration is pending.";
    });

    // Reset Password validation
    resetPasswordForm.addEventListener("submit", event => {
        event.preventDefault();
        clearMessages();

        const email = document.getElementById("resetEmail").value.trim();
        const emailError = document.getElementById("resetEmailError");
        const message = document.getElementById("resetPasswordMessage");

        if (!email) {
            emailError.textContent = "Please enter your email address.";
            return;
        }

        const emailPattern = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

        if (!emailPattern.test(email)) {
            emailError.textContent = "Please enter a valid email address.";
            return;
        }

        message.textContent =
            "Email submitted. Password reset functionality will be added later.";
    });
});
