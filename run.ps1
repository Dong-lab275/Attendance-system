
# Configure PATH - prioritize MSYS2 UCRT64
$env:PATH = "C:\msys64\ucrt64\bin;C:\Program Files\PostgreSQL\18\bin;" + $env:PATH

# PostgreSQL configuration
$env:DB_PASSWORD = "1"

Write-Host "Starting SAMS..."

.\build\AttendanceSystem.exe

Write-Host "Program exit code: $LASTEXITCODE"
Read-Host "Press Enter to close"
