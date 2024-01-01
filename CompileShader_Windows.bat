@echo off
setlocal enabledelayedexpansion

echo ==============================================================
echo compiling shader

set "batchDir=%~dp0Shaders"
echo serching shader in : %batchDir%
set "fileExtensionFrag=.frag"
set "fileExtensionVert=.vert"
set "targetExtension=.spv"

for %%Z in ("%batchDir%\*%fileExtensionFrag%") do (
    echo Processing: %%Z
    set "fileName=%%~nZFrag%targetExtension%"
    echo %batchDir%\spv\!fileName!
    C:\VulkanSDK\1.3.268.0\Bin\glslc.exe %%Z -o %batchDir%\spv\!fileName!
)

for %%Z in ("%batchDir%\*%fileExtensionVert%") do (
    echo Processing: %%Z
    set "fileName=%%~nZVert%targetExtension%"
    echo %batchDir%\spv\!fileName!
    C:\VulkanSDK\1.3.268.0\Bin\glslc.exe %%Z -o %batchDir%\spv\!fileName!
)

rem "-------------------------------------------------------------------------"

set "batchDir=%~dp0src\Samples"

for /d %%i in ("%batchDir%\*") do (
    set "folderPath=%%i"
    echo serching shader in : !folderPath!

    for %%Z in ("!folderPath!\*%fileExtensionFrag%") do (
	    echo Processing: %%Z
	    set "fileName=%%~nZFrag%targetExtension%"
	    echo %folderPath%\spv\!fileName!
	    C:\VulkanSDK\1.3.268.0\Bin\glslc.exe %%Z -o %~dp0Shaders\spv\!fileName!
	)

	for %%Z in ("!folderPath!\*%fileExtensionVert%") do (
	    echo Processing: %%Z
	    set "fileName=%%~nZVert%targetExtension%"
	    echo %folderPath%\spv\!fileName!
	    C:\VulkanSDK\1.3.268.0\Bin\glslc.exe %%Z -o %~dp0Shaders\spv\!fileName!
	)
)

echo ==============================================================
echo complete.

pause