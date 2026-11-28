
async function apiRequest(endpoint, method = "GET", body = null, token = null) {
    const headers = {
        "Content-Type": "application/json"
    };

    if (token) {
        headers.Authorization = `Bearer ${token}`;
    }

    const options = {
        method,
        headers
    };

    if (body !== null) {
        options.body = JSON.stringify(body);
    }

    const response = await fetch(`${API_BASE_URL}${endpoint}`, options);

    const result = await response.json();

    if (!response.ok) {
        throw new Error(result.message || "Request failed.");
    }

    return result;
}
