$JobBuild = "test-crwproc-build"

$SolutionPath = (Get-Item .).FullName
$SolutionName = "crwproc.sln"

$ProjectUtPath = "$SolutionPath\tests\ut"
$ProjectUtName = "ut.vcxproj"

$BinaryUtPath = "$SolutionPath\x64\Release"
$BinaryUtName = "ut.exe"

$CppCheckUrlInstaller = "https://sourceforge.net/projects/cppcheck/files/cppcheck/2.6/cppcheck-2.6-x64-Setup.msi"
$CppCheckInstaller = "C:\cppcheck-2.6-x64-Setup.msi"
$CppCheckPath = "C:\CppCheck"
$CppCheckApplication = "cppcheck.exe"


function Announce-Step($Message) {
    Write-Host "[New Step] $Message" -ForegroundColor green
}


function Install-PythonPackages {
    Announce-Step "Install Python Packages."
    
    pip install robotframework robotframework-httpctrl
}


function Build-Application($Configuration) {
    Announce-Step "Build Application."
    
    msbuild /p:RestorePackagesConfig=true /t:restore
    
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
