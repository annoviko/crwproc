$JobBuild = "test-crwproc-build"

$SolutionPath = (Get-Item .).FullName
$SolutionName = "crwproc.sln"

$ProjectUtPath = "$SolutionPath\tests\ut"
$ProjectUtName = "ut.vcxproj"

$BinaryUtPath = "$SolutionPath\x64\Release"
$BinaryUtName = "ut.exe"

$CppCheckUrlInstaller = "https://github.com/danmar/cppcheck/releases/download/2.6/cppcheck-2.6-x64-Setup.msi"
$CppCheckInstaller = "cppcheck-2.6-x64-Setup.msi"
$CppCheckPath = "C:\CppCheck"
$CppCheckApplication = "cppcheck.exe"


function Announce-Step($Message) {
    Write-Host "[New Step] $Message" -ForegroundColor green
}


function Download-CppCheck {
    Announce-Step "Download cppcheck for Windows from github release page."

    $WebClient = New-Object System.Net.WebClient

    $Attempts = 3
    for($i = 0; $i -lt $Attempts; $i++){
        try {
            $WebClient.DownloadFile($CppCheckUrlInstaller, $CppCheckInstaller)
            break
        }
        Catch [Exception]{
            Write-Host "[Info] CppCheck downloading failed (attempts: '$i')." -ForegroundColor Green
            Start-Sleep 1
        }
    }
    
    if (Test-Path $CppCheckInstaller) {
        Write-Host "[Info] CppCheck installer file has been download to '$CppCheckInstaller'." -ForegroundColor Green
    }
    else {
        Write-Error -Message "[Error] CppCheck installer has not been downloaded." -Category ResourceUnavailable
        Exit 1
    }
}


function Install-PythonPackages {
    Announce-Step "Install Python Packages."
    
    pip install robotframework robotframework-httpctrl
}


function Install-CppCheck {
    Announce-Step "Install CppCheck."
    
    Download-CppCheck
    
    $InstallArguments = "/InstallationType=AllUsers /S /AddToPath=1 D=$CppCheckPath"
    
    Start-Process -FilePath $CppCheckInstaller -ArgumentList $InstallArguments -Wait -Passthru

    if (Test-Path $CppCheckPath) {
        Write-Host "[Info] CppCheck has been successfully installed to '$CppCheckPath'." -ForegroundColor Green
        Remove-Item $CppCheckInstaller
    }
    else {
        Write-Error -Message "[Info] CppCheck has not been installed to '$CppCheckPath' with error code: '$LastExitCode'."
        Exit 1
    }
    
    $env:PATH = "$env:PATH;$CppCheckPath"
}


function Run-CppCheck {
    Announce-Step "Analyse Application Source Code."
    
    Install-CppCheck
    
    & $CppCheckApplication --inline-suppr --error-exitcode=1 --std=c++17 --inconclusive --enable=warning,style,performance,information,portability core control log crwproc
    if ($LastExitCode -ne 0) {
        Write-Error "[Error] CppCheck analysis failed with error code '$LastExitCode'."
        Exit 1
    }
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
    
    Build-UnitTests
    
    & "$BinaryUtPath\$BinaryUtName"
    
    if ($LastExitCode -ne 0) {
        Write-Error "[Error] Unit-testing failed with error code '$LastExitCode'."
        Exit 1
    }
}


function Run-SctTests {
    Announce-Step "Run Sct-Tests."
    
    Build-SubjectApplication
    Install-PythonPackages
    
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
    
    Run-UnitTests
    Run-CppCheck
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
