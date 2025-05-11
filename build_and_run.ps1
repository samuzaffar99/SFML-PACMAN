# build-and-run.ps1

# Stop immediately on errors
$ErrorActionPreference = "Stop"

# Create build directory if it doesn't exist
if (-Not (Test-Path -Path "build")) {
    Write-Host "Creating build directory..."
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# Navigate to build directory
Set-Location -Path "build"

try {
    # Generate build system
    Write-Host "Running CMake generation..."
    cmake .. 

    # Build the project
    Write-Host "Building the project..."
    cmake --build .

    Set-Location -Path "Debug"

    # Run the executable
    $exe = "SFML-PACMAN.exe"
    if (Test-Path -Path $exe) {
        Write-Host "Launching $exe..."
        Start-Process -FilePath ".\$exe" -NoNewWindow
    }
    else {
        Write-Error "Error: Executable '$exe' not found in build directory."
    }
}
catch {
    Write-Error "❌ Script failed: $_"
}
