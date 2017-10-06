@ECHO OFF
ECHO git add . | "C:\Program Files\Git\git-cmd.exe"
set "DT=%date%_%time%"
ECHO git commit -m "MAKE PROGRAM: %DT%" | "C:\Program Files\Git\git-cmd.exe"
ECHO git git push -u origin master | "C:\Program Files\Git\git-cmd.exe"

