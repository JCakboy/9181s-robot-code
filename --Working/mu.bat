@echo off
echo Making and uploading PROS project...
IF %1.==. (
	set var=1 
) ELSE (
	set var=%1
)
prosv5 make
prosv5 upload --slot %var%