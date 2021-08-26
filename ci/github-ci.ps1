New-Variable -Name JobBuild -Value "test-crwproc-build" -Option ReadOnly -Force
New-Variable -Name SolutionPath -Value crwproc.sln -Option ReadOnly -Force


function Build-Application($Configuration) {
	msbuild $SolutionPath /t:crwproc /p:configuration=$Configuration /p:platform=x64
	if ($LastExitCode -ne 0) {
		Write-Error "Building crwproc failed with error code '$LastExitCode'."
		Exit 1
	}
}


function Build-UnitTests {
	msbuild $SolutionPath /t:ut /p:configuration=release /p:platform=x64
	if ($LastExitCode -ne 0) {
		Write-Error "Building unit-test project failed with error code '$LastExitCode'."
		Exit 1
	}
}


function Run-UnitTests {
	Start-Process -FilePath ".\x64\Release\ut.exe"
	if ($LastExitCode -ne 0) {
		Write-Error "Unit-testing failed with error code '$LastExitCode'."
		Exit 1
	}
}


function Run-BuildTestJob {
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
