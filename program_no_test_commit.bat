@ECHO OFF
ECHO git add . | "C:\Program Files\Git\git-cmd.exe"
set "DT=%date%_%time%"
ECHO git commit -m "MAKE NOT TESTED PROGRAM: %DT%" | "C:\Program Files\Git\git-cmd.exe"
ECHO git push -u origin master | "C:\Program Files\Git\git-cmd.exe"
ECHO "WARNING NOT TESTED BUT Programmed, committed and pushed to origin!"
