@echo off

SET VSWHERE="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere"

:: See https://github.com/microsoft/vswhere/wiki/Find-VC
for /f "usebackq delims=*" %%i in (`%VSWHERE% -latest -property installationPath`) do (
  call "%%i"\Common7\Tools\vsdevcmd.bat %*
)

:: Loop over all environment variables and make them global using set-env.
:: See: https://help.github.com/en/articles/development-tools-for-github-actions#set-an-environment-variable-set-env
:: See: https://stackoverflow.com/questions/39183272/loop-through-all-environmental-variables-and-take-actions-depending-on-prefix
setlocal
for /f "delims== tokens=1,2" %%a in ('set') do (
  echo ::set-env name=%%a::%%b
)
endlocal
