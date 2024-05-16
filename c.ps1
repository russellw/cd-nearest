# Path to the file storing the list of visited directories
$visitedDirsFile = [System.IO.Path]::Combine($HOME, "Documents", "cd-nearest.csv")

# Ensure the visited directories file exists
if (-not (Test-Path $visitedDirsFile)) {
    New-Item -ItemType File -Path $visitedDirsFile -Force
}

# Read the list of previously visited directories
$visitedDirs = Get-Content -Path $visitedDirsFile

# Get the command line argument
param (
    [string]$TargetDir
)

if (-not $TargetDir) {
    Write-Host "No directory specified."
    exit
}

# Determine the target directory
if ([System.IO.Path]::IsPathRooted($TargetDir)) {
    # Argument is an absolute path
    $finalDir = $TargetDir
} elseif ($TargetDir -match "\\") {
    # Argument is a relative path
    $finalDir = [System.IO.Path]::Combine((Get-Location).Path, $TargetDir)
} else {
    # Argument is a prefix
    $prefix = $TargetDir.ToLower()
    $matchingDirs = $visitedDirs | Where-Object { $_.Split([System.IO.Path]::DirectorySeparatorChar)[-1].ToLower().StartsWith($prefix) }
    if ($matchingDirs.Count -eq 0) {
        Write-Host "No matching directory found."
        exit
    }
    $finalDir = $matchingDirs[0]
}

# Update the visited directories list
if ($visitedDirs -contains $finalDir) {
    $visitedDirs = $visitedDirs | Where-Object { $_ -ne $finalDir }
}
$visitedDirs = $finalDir + "`n" + ($visitedDirs -join "`n")
Set-Content -Path $visitedDirsFile -Value $visitedDirs

# Change to the chosen directory
Set-Location -Path $finalDir
Write-Host "Changed directory to $finalDir"
