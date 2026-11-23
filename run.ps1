# Configure PATH - prioritize MSYS2 UCRT64
$env:PATH = "C:\msys64\ucrt64\bin;C:\Program Files\PostgreSQL\18\bin;" + $env:PATH

# Load database password from User environment variable
$env:DB_PASSWORD = [Environment]::GetEnvironmentVariable("DB_PASSWORD", "User")

if ([string]::IsNullOrWhiteSpace($env:DB_PASSWORD)) {
    Write-Error "DB_PASSWORD is not configured."
    exit 1
}

Write-Host "Starting SAMS..."

.\build-msys2\AttendanceSystem.exe

Write-Host "Program exit code: $LASTEXITCODE"
Read-Host "Press Enter to close"
