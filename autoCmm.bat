@echo off
REM This autocommit tool is support for lazy ferson
REM Author : CDP.Darkness
REM Company: DP-Greens
REM	Version: 0.1
REM	Date   : 20/10/2024

echo *====================This product from DP-Green=================*
echo *=                   DP-Green autocommit V0.1                  =*
echo *=-------------------------------------------------------------=*
echo *=  -@@ Tool   : AutoCommit                                    =*
echo *=  -@@ Version: 0.1                                           =*
echo *=  -@@ Date: 20/10/2024                                       =*
echo *=-------------------------------------------------------------=*
echo *=  -@@ Author : CDP.Darkness                                  =*
echo *=  -@@ Company: DP-Green                                      =*
echo *===============================================================*

echo off
echo off
echo *==/Loading: 10% define states' flag-----------------------------*
REM define flags
set   F_FALSE = 0
set   F_TRUE  = 1
echo *====/Loading: 20% define project's parameters-------------------*
REM define porject
set  /A Project= FM_V03_01
echo *=========/Loading: 30% define git flags-------------------------*
REM define git Flags
set   Flag_git_added  = %F_FALSE%
set   Flag_git_commit = %F_FALSE%
set   Flag_git_Push   = %F_FALSE%
set   Flag_git_pull   = %F_FALSE%
echo *============/Loading: 40% define hastag commit------------------*
REM define hastag 

echo *===============/Loading: 50% define directory-------------------*
REM Define Directory
set Directory=C:\00_WorkSpace\FM_Master\FM_V03_01\FM_V03_01
echo *===================/Loading: 60% Call FM_V0x_Function (1)-------*
echo *=====================/Loading: 70% Call FM_V0x_Function (2)-----*
echo *=======================/Loading: 80% Call FM_V0x_Function (3)---*
echo *========================/Loading: 90% Call FM_V0x_Function (4)--*
echo *=========================/Loading: 100% Call FM_V0x_Function (5)*
cd %Directory%

call :FM_V0x_Go_Dir 0

call :FM_V0x_Add_Files 0 

call :FM_V0x_Commit_Files 
REM define functions
:FM_V0x_Go_Dir

	if %Flag_git_pull% == %F_FALSE% (
	    echo Pulling data from server
		git pull	
		echo To Pull Data completed from server
	) else (
		echo Failed to pull data from server 
		)	
	exit /b

:FM_V0x_Add_Files
	if %Flag_git_added% == %F_FALSE% (
		git add .
		echo Add files to repository.
	) else (
		echo Failed to add files to repository. )
	exit /b

:FM_V0x_Commit_Files
	if %Flag_git_commit% == %F_FALSE% (
		setlocal
		
		for /f "tokens=1-4 delims=/.- " %%a in ("%date%") do (
			set year=%%c
			set month=%%a
			set day=%%b
		)
		for /f "tokens=1-3 delims=:., " %%a in ("%time%") do (
			set hour=%%a
			set minute=%%b
			set second=%%c
		)
		set timestamp=%year%%month%%day%_%hour%%minute%%second%
		git commit -m "FM_V0x_Farming commit with timestamp is: %timestamp%"
		echo Add files to repository.
	) else (
		echo Failed to add files to repository. )
	exit /b