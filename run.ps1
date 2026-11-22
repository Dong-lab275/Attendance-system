
# Configure PATH
$env:PATH = "C:\msys64\ucrt64\bin;C:\Program Files\PostgreSQL\18\bin;" + $env:PATH

# Load database password
$env:DB_PASSWORD = [Environment]::GetEnvironmentVariable("DB_PASSWORD", "User")

# Start SAMS
Write-Host "Starting SAMS..."

& .\build\AttendanceSystem.exe

Write-Host "Program exit code: $LASTEXITCODE"
Read-Host "Press Enter to close"
