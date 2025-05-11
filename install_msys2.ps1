# PowerShell script to check and install MSYS2

$msys2Dir = "C:\msys64"
$msys2InstallerUrl = "https://github.com/msys2/msys2-installer/releases/download/2024-01-13/msys2-x86_64-20240113.exe" # Check for the latest version on the MSYS2 website
$installerName = "msys2-installer.exe"
$downloadPath = Join-Path $env:TEMP $installerName

# Check if MSYS2 directory exists
if (Test-Path $msys2Dir) {
    Write-Host "MSYS2 appears to be installed already at $msys2Dir"
}
else {
    Write-Host "MSYS2 not found. Attempting to download and install."

    # Download the installer
    try {
        Write-Host "Downloading MSYS2 installer from $msys2InstallerUrl..."
        Invoke-WebRequest -Uri $msys2InstallerUrl -OutFile $downloadPath -ErrorAction Stop
        Write-Host "Download complete: $downloadPath"
    }
    catch {
        Write-Error "Failed to download MSYS2 installer. Error: $($_.Exception.Message)"
        Write-Error "Please download it manually from https://www.msys2.org/ and install it to $msys2Dir"
        exit 1
    }

    # Run the installer
    try {
        Write-Host "Starting MSYS2 installer... Please follow the on-screen instructions."
        Write-Host "It is recommended to install to the default location ($msys2Dir) if prompted."
        Start-Process -FilePath $downloadPath -Wait
        Write-Host "MSYS2 installation process finished."

        # Basic check if installation might have succeeded
        if (Test-Path $msys2Dir) {
            Write-Host "MSYS2 installation directory found at $msys2Dir."
            # Attempt to run pacman commands
            $bashPath = Join-Path $msys2Dir "usr\\bin\\bash.exe"
            if (Test-Path $bashPath) {
                Write-Host "Attempting to update MSYS2 and install MinGW toolchain..."
                Write-Host "This may take several minutes and might require you to close and re-open the MSYS2 window if it pops up."
                try {
                    # First command: pacman -Syu
                    Write-Host "Running: $bashPath -l -c ""pacman -Syu --noconfirm"""
                    Start-Process -FilePath $bashPath -ArgumentList "-l -c ""pacman -Syu --noconfirm""" -Wait -ErrorAction Stop
                    Write-Host "Initial MSYS2 update (pacman -Syu) finished."
                    
                    # Second command: pacman -S mingw-w64-x86_64-toolchain
                    Write-Host "Running: $bashPath -l -c ""pacman -S --noconfirm mingw-w64-x86_64-toolchain"""
                    Start-Process -FilePath $bashPath -ArgumentList "-l -c ""pacman -S --noconfirm mingw-w64-x86_64-toolchain""" -Wait -ErrorAction Stop
                    Write-Host "MinGW toolchain installation (pacman -S mingw-w64-x86_64-toolchain) finished."
                    Write-Host "MinGW g++ should now be available in your MSYS2 MinGW64 environment."
                    Write-Host "You might need to add C:\\msys64\\mingw64\\bin to your system PATH to use g++ directly from PowerShell or CMD."
                } catch {
                    Write-Error "An error occurred while running pacman commands. Error: $($_.Exception.Message)"
                    Write-Warning "Please open an MSYS2 terminal (e.g., by running C:\\msys64\\msys2.exe) and manually run the following commands:"
                    Write-Warning "1. pacman -Syu"
                    Write-Warning "2. pacman -S mingw-w64-x86_64-toolchain"
                }
            } else {
                Write-Warning "Could not find bash.exe at $bashPath."
                Write-Warning "Please open an MSYS2 terminal (e.g., by running C:\\msys64\\msys2.exe) and manually run:"
                Write-Warning "1. pacman -Syu"
                Write-Warning "2. pacman -S mingw-w64-x86_64-toolchain"
            }
        }
        else {
            Write-Warning "MSYS2 installation directory not found after running installer. Please check the installation."
        }
    }
    catch {
        Write-Error "Failed to start MSYS2 installer. Error: $($_.Exception.Message)"
        Write-Error "Please run the downloaded installer manually from $downloadPath"
        exit 1
    }
    finally {
        # Clean up the installer
        if (Test-Path $downloadPath) {
            Remove-Item $downloadPath
        }
    }
}

Write-Host "Script finished."
