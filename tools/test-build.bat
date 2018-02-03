REM This script is used for testing the build, primarily for use
REM with appveyor, but may be used by hand as well.

set build=%1

if [%build%] == [] exit /b 2

if [%build%] == [maven] (
  REM Test the maven build
  mvn install || exit /b 1
)
