
document.addEventListener("DOMContentLoaded", () => {

    const checkInButton = document.getElementById("checkInButton");
    const sessionStatus = document.getElementById("sessionStatus");
    const remainingTime = document.getElementById("remainingTime");
    const guidanceMessage = document.getElementById("guidanceMessage");

    // Check-in button: frontend demonstration only
    if (checkInButton) {
        checkInButton.addEventListener("click", () => {

            const confirmCheckIn = confirm(
                "Are you sure you want to check in to this class?"
            );

            if (!confirmCheckIn) {
                return;
            }

            // Update button
            checkInButton.textContent = "Checked in";
            checkInButton.disabled = true;

            // Update session information
            remainingTime.textContent = "Your attendance has been recorded.";

            // Update guidance message
            guidanceMessage.textContent =
                "Check-in successful. Your attendance has been recorded.";

            // Update status badge
            sessionStatus.textContent = "Checked in";
            sessionStatus.classList.remove("open");
            sessionStatus.classList.add("checked-in");

        });
    }

});
