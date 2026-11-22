
const studentId = "ST001";

const demoCourseId = "PHI205";
const demoSessionId = "DEMO001";

let activeSession = null;
let demoRegistered = false;
let gpsPosition = null;
let demoFinished = false;

function $(id) {
    return document.getElementById(id);
}

function setMessage(id, text, type = "") {
    const element = $(id);

    element.textContent = text;
    element.className = "message";

    if (type) {
        element.classList.add(type);
    }
}

async function getJson(url) {
    const response = await fetch(url);

    const data = await response.json();

    return {
        ok: response.ok,
        status: response.status,
        data: data
    };
}

async function postParams(url, parameters) {
    const body = new URLSearchParams(parameters);

    const response = await fetch(url, {
        method: "POST",
        headers: {
            "Content-Type":
                "application/x-www-form-urlencoded"
        },
        body: body.toString()
    });

    const data = await response.json();

    return {
        ok: response.ok,
        status: response.status,
        data: data
    };
}

function updateDemoButton() {
    $("demoCheckin").disabled =
        !demoRegistered ||
        gpsPosition === null ||
        demoFinished;
}

function requestGps() {
    return new Promise((resolve, reject) => {
        if (!navigator.geolocation) {
            reject(new Error(
                "Your browser does not support GPS."
            ));
            return;
        }

        navigator.geolocation.getCurrentPosition(
            position => {
                resolve({
                    latitude: position.coords.latitude,
                    longitude: position.coords.longitude
                });
            },
            error => {
                reject(new Error(
                    "Cannot access GPS: " + error.message
                ));
            },
            {
                enableHighAccuracy: true,
                timeout: 15000,
                maximumAge: 0
            }
        );
    });
}

async function loadSession() {
    const button = $("mainCheckin");

    button.disabled = true;

    setMessage(
        "mainMessage",
        "Checking attendance session..."
    );

    try {
        const result = await getJson(
            "/api/checkin-session"
        );

        if (!result.ok) {
            throw new Error(
                "Cannot load attendance session."
            );
        }

        const session = result.data;

        activeSession = session;

        const isOpen =
            session.status === "Open" ||
            session.status === "OPEN";

        const canCheckIn =
            isOpen &&
            session.canCheckIn === true;

        $("sessionStatus").textContent =
            session.status || "Unknown";

        $("sessionStatus").className =
            "status " + (isOpen ? "open" : "closed");

        button.disabled = !canCheckIn;

        setMessage(
            "mainMessage",
            session.message ||
            (canCheckIn
                ? "You can check in now."
                : "Check-in is not available.")
        );

    } catch (error) {
        activeSession = null;

        button.disabled = true;

        $("sessionStatus").textContent = "Offline";

        setMessage(
            "mainMessage",
            error.message,
            "error"
        );
    }
}

async function loadRegistration() {
    try {
        const result = await getJson(
            "/api/my-courses?studentId=" +
            encodeURIComponent(studentId)
        );

        if (!result.ok) {
            throw new Error(
                "Cannot load course registration."
            );
        }

        demoRegistered =
            Array.isArray(result.data.courses) &&
            result.data.courses.includes(demoCourseId);

        setMessage(
            "demoMessage",
            demoRegistered
                ? "Demo course already registered."
                : "Register the demo course to continue."
        );

    } catch (error) {
        demoRegistered = false;

        setMessage(
            "demoMessage",
            error.message,
            "error"
        );
    }

    updateDemoButton();
}

async function submitCheckin(
    courseId,
    sessionId,
    messageId
) {
    setMessage(
        messageId,
        "Requesting your current location..."
    );

    const position = await requestGps();

    setMessage(
        messageId,
        "Submitting attendance request..."
    );

    return await postParams(
        "/api/check-in",
        {
            studentId: studentId,
            courseId: courseId,
            sessionId: sessionId,
            studentLat: position.latitude,
            studentLon: position.longitude
        }
    );
}

$("mainCheckin").addEventListener(
    "click",
    async function () {
        if (!activeSession ||
            activeSession.canCheckIn !== true ||
            !["Open", "OPEN"].includes(
                activeSession.status
            )) {
            return;
        }

        this.disabled = true;

        try {
            const result = await submitCheckin(
                activeSession.courseId,
                activeSession.sessionId,
                "mainMessage"
            );

            setMessage(
                "mainMessage",
                result.data.message ||
                "Attendance request completed.",
                result.ok ? "success" : "error"
            );

            if (result.ok) {
                $("sessionStatus").textContent =
                    "Checked in";

                $("sessionStatus").className =
                    "status open";

                return;
            }

        } catch (error) {
            setMessage(
                "mainMessage",
                error.message,
                "error"
            );
        }

        await loadSession();
    }
);

$("registerDemo").addEventListener(
    "click",
    async function () {
        this.disabled = true;

        setMessage(
            "demoMessage",
            "Registering demo course..."
        );

        try {
            const result = await postParams(
                "/api/register-course",
                {
                    studentId: studentId,
                    courseId: demoCourseId
                }
            );

            if (result.ok || result.status === 409) {
                demoRegistered = true;

                setMessage(
                    "demoMessage",
                    result.data.message,
                    "success"
                );
            } else {
                setMessage(
                    "demoMessage",
                    result.data.message,
                    "error"
                );
            }

        } catch (error) {
            setMessage(
                "demoMessage",
                "Cannot connect to Student Server.",
                "error"
            );
        }

        this.disabled = false;

        updateDemoButton();
    }
);

$("checkGps").addEventListener(
    "click",
    async function () {
        this.disabled = true;

        gpsPosition = null;
        updateDemoButton();

        setMessage(
            "demoMessage",
            "Checking your GPS location..."
        );

        try {
            const position = await requestGps();

            const url =
                "/api/check-gps?studentLat=" +
                encodeURIComponent(position.latitude) +
                "&studentLon=" +
                encodeURIComponent(position.longitude);

            const result = await getJson(url);

            if (!result.ok) {
                throw new Error(
                    result.data.message ||
                    "GPS verification failed."
                );
            }

            if (result.data.withinRadius === true) {
                gpsPosition = position;

                setMessage(
                    "demoMessage",
                    "Inside demo classroom radius.",
                    "success"
                );
            } else {
                setMessage(
                    "demoMessage",
                    "Outside demo classroom radius.",
                    "error"
                );
            }

        } catch (error) {
            setMessage(
                "demoMessage",
                error.message,
                "error"
            );
        }

        this.disabled = false;

        updateDemoButton();
    }
);

$("demoCheckin").addEventListener(
    "click",
    async function () {
        if (!demoRegistered ||
            gpsPosition === null ||
            demoFinished) {
            return;
        }

        this.disabled = true;

        try {
            const result = await submitCheckin(
                demoCourseId,
                demoSessionId,
                "demoMessage"
            );

            if (result.ok) {
                demoFinished = true;
            }

            if (result.status === 409) {
                demoFinished = true;
            }

            setMessage(
                "demoMessage",
                result.data.message ||
                "Demo attendance request completed.",
                result.ok ? "success" : "error"
            );

        } catch (error) {
            setMessage(
                "demoMessage",
                error.message,
                "error"
            );
        }

        updateDemoButton();
    }
);

async function initialize() {
    await Promise.all([
        loadSession(),
        loadRegistration()
    ]);
}

initialize();
