$JobBuild = "test-crwproc-build"

$SolutionPath = (Get-Item .).FullName
$SolutionName = "crwproc.sln"

$ProjectUtPath = "$SolutionPath\tests\ut"
$ProjectUtName = "ut.vcxproj"

$BinaryUtPath = "$SolutionPath\x64\Release"
$BinaryUtName = "ut.exe"

$InstallerMiniconda = "Miniconda3-4.7.12.1-Windows-x86_64.exe"
$UrlInstallerMiniconda = "https://repo.continuum.io/miniconda/" + $InstallerMiniconda
$MinicondaPath = "C:\Specific-Miniconda"


function Announce-Step($Message) {
    Write-Host "[New Step] $Message" -ForegroundColor green
}


function Download-Miniconda() {
    Announce-Step "Download Miniconda."
    
    $WebClient = New-Object System.Net.WebClient

    $Attempts = 3
    for($i = 0; $i -lt $Attempts; $i++){
        try {
            $WebClient.DownloadFile($UrlInstallerMiniconda, $InstallerMiniconda)
            break
        }
        Catch [Exception]{
            Write-Host "[Info] Miniconda downloading failed (attempts: '$i')." -ForegroundColor Green
            Start-Sleep 1
        }
    }
    
    if (Test-Path $InstallerMiniconda) {
        Write-Host "[Info] Miniconda installer file has been download to '$InstallerMiniconda'." -ForegroundColor Green
    }
    else {
        Write-Error -Message "[Error] Miniconda installer has not been downloaded." -Category ResourceUnavailable
        Exit 1
    }
}


function Install-Miniconda() {
    Announce-Step "Install Miniconda."

    $InstallArguments = "/InstallationType=AllUsers /S /AddToPath=1 /RegisterPython=1 /D=$MinicondaPath"

    Start-Process -FilePath $InstallerMiniconda -ArgumentList $InstallArguments -Wait -Passthru
    
    if (Test-Path $MinicondaPath) {
        Write-Host "[Info] Miniconda has been successfully installed to '$MinicondaPath'." -ForegroundColor Green
        Remove-Item $InstallerMiniconda
    }
    else {
        Write-Error -Message "[Info] Miniconda has not been installed to '$MinicondaPath' with error code: '$LastExitCode'."
        Exit 1
    }
    
    $env:PATH = "$env:PATH;$MinicondaPath\Scripts"
}


function Install-MinicondaPackages() {
    Announce-Step "Install Miniconda Packages."
    
    conda config --set always_yes true
    conda create -q -n test-environment python=3.9
    conda install -q -n test-environment -c conda-forge robotframework
    
    activate test-environment
    
    pip install robotframework-httpctrl
    
    $env:PYTHON_INTERPRETER = "$MinicondaPath\envs\test-environment\python.exe";
    $env:PYTHONPATH = "$MinicondaPath\envs\test-environment";

    $env:PATH = "$MinicondaPath\envs\test-environment;$env:PATH";
    $env:PATH = "$MinicondaPath\envs\test-environment\Scripts;$env:PATH";
    $env:PATH = "$MinicondaPath\envs\test-environment\Library\bin;$env:PATH";
    
    conda info -a
}


function Build-Application($Configuration) {
    Announce-Step "Build Application."
    
    msbuild $SolutionName /t:crwproc /p:configuration=$Configuration /p:platform=x64
    if ($LastExitCode -ne 0) {
        Write-Error "[Error] Building crwproc failed with error code '$LastExitCode'."
        Exit 1
    }
}


function Build-UnitTests {
    Announce-Step "Build Unit-Tests."
    
    msbuild /p:RestorePackagesConfig=true /t:restore
    
    msbuild $SolutionName /t:ut /p:configuration=release /p:platform=x64
    if ($LastExitCode -ne 0) {
        Write-Error "[Error] Building unit-test project failed with error code '$LastExitCode'."
        Exit 1
    }
}


function Build-SubjectApplication {
    Announce-Step "Build Subject Application."
    
    msbuild $SolutionName /t:subject\basic_subject /p:configuration=release /p:platform=x64
    if ($LastExitCode -ne 0) {
        Write-Error "[Error] Building basic subject project failed with error code '$LastExitCode'."
        Exit 1
    }
}


function Run-UnitTests {
    Announce-Step "Run Unit-Tests."
    
    & "$BinaryUtPath\$BinaryUtName"
    
    if ($LastExitCode -ne 0) {
        Write-Error "[Error] Unit-testing failed with error code '$LastExitCode'."
        Exit 1
    }
}


function Run-SctTests {
    Announce-Step "Run Sct-Tests."
    
    Build-SubjectApplication
    Download-Miniconda
    Install-Miniconda
    Install-MinicondaPackages
    
    cd tests\sct
    python -m robot.run *.robot

    if ($LastExitCode -ne 0) {
        Write-Error "[Error] Sct-testing failed with error code '$LastExitCode'."
        Exit 1
    }
    
    cd ..\..
}


function Run-BuildTestJob {
    Announce-Step "Run Build Test Job."
    
    Build-Application "Release"
    
    Build-UnitTests
    Run-UnitTests
    Run-SctTests
}


function Run-ContinousIntegration($Job) {
    switch($Job) {
        $JobBuild { 
            Run-BuildTestJob
            break
        }
        default {
            Write-Error "Unknown job is specified '$Job'." -TargetObject $Job
            Exit 1
        }
    }
}


Run-ContinousIntegration $args[0]
