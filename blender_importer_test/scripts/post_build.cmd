::	%1	SolutionDir
::  %2	OutDir
::  %3  Configuration


xcopy %1"bin/blender_importer.x64."%3"\*.dll" %2 /d /s /y 1> nul