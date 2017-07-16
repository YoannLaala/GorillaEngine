@echo off
set _SOLUTION_PATH_="../../Solution/Cooker"
if exist %_SOLUTION_PATH_% (goto :clean_solution)

:clean_solution
rmdir %_SOLUTION_PATH_% /s /q