
    // Demo student data.
   let student = {};

async function loadStudentProfile() {
  try {
    const response = await fetch(
      "/api/student-profile?studentId=ST001"
    );

    if (!response.ok) {
      throw new Error("Cannot load student profile");
    }

    student = await response.json();

if (typeof student === "string") {
  student = JSON.parse(student);
}
    displayStudentInformation();

  } catch (error) {
    console.error(error);
    alert("Cannot load student profile.");
  }
}

    function displayStudentInformation() {
      document.getElementById("studentName").textContent =
        student.fullName;

      document.getElementById("studentSummary").textContent =
        student.studentId + " · Student";

      document.getElementById("studentId").textContent =
        student.studentId;

      document.getElementById("fullName").textContent =
        student.fullName;

      document.getElementById("dateOfBirth").textContent =
        student.dateOfBirth;

      document.getElementById("gender").textContent =
        student.gender;

      document.getElementById("emailAddress").textContent =
        student.emailAddress;

      document.getElementById("phoneNumber").textContent =
        student.phoneNumber;

      document.getElementById("address").textContent =
        student.address;
    }

    document.getElementById("password-link")
      .addEventListener("click", function(event) {
        event.preventDefault();
        alert("Password changes require the authentication backend.");
      });

    document.getElementById("signout-link")
      .addEventListener("click", function(event) {
        event.preventDefault();
        alert("Sign out requires a real login session.");
      });

   loadStudentProfile();
