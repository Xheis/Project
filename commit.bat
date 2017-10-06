@ECHO OFF
git add .
set /p commitMsg=Enter Commit Message:
git commit -m "%commitMsg%"