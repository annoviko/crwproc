New-Variable -Name JobBuild -Value "test-crwproc-build" -Option ReadOnly -Force
New-Variable -Name SolutionName -Value "crwproc.sln" -Option ReadOnly -Force
New-Variable -Name ProjectUtPath -Value ".\tests\ut\" -Option ReadOnly -Force
New-Variable -Name ProjectUtName -Value "ut.vcxproj" -Option ReadOnly -Force


function Announce-Step($Message) {
	Write-Host "[New Step] $Message" -ForegroundColor green
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
	
	msbuild "$ProjectUtPath$ProjectUtName" /t:Restore
	msbuild $SolutionName /t:ut /p:configuration=release /p:platform=x64
	if ($LastExitCode -ne 0) {
		Write-Error "[Error] Building unit-test project failed with error code '$LastExitCode'."
		Exit 1
	}
}


function Run-UnitTests {
	Announce-Step "Run Unit-Tests."
	
	Start-Process -FilePath ".\x64\Release\ut.exe"
	if ($LastExitCode -ne 0) {
		Write-Error "[Error] Unit-testing failed with error code '$LastExitCode'."
		Exit 1
	}
}


function Run-BuildTestJob {
	Announce-Step "Run Build Test Job."
	
	Build-Application "Release"
	
	Build-UnitTests
	Run-UnitTests
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
