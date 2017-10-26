@ECHO OFF
ECHO git add . | "C:\Program Files\Git\git-cmd.exe"
set "DT=%date%_%time%"
set logFile=%1
ECHO git commit -m "MAKE COMMIT: %DT%" -m %logFile%  | "C:\Program Files\Git\git-cmd.exe"
ECHO git push -u origin master | "C:\Program Files\Git\git-cmd.exe"
ECHO "Offline committed and pushed to origin!"
