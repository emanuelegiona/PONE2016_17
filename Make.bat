@ECHO OFF

set DIR=%CD%
IF "%1%"=="clean" (
	del /s *.obj
	del /s *.exe
)


IF "%1%"=="" (
	cl -openmp client.c win/wclient.c common/*.c
	cl -openmp win/wserver.c common/*.c
)
