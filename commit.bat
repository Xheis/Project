@ECHO OFF
ECHO git add . | "C:\Program Files\Git\git-cmd.exe"
set "DT=%date%_%time%"
ECHO git commit -m "PROGRAM: %DT%" | "C:\Program Files\Git\git-cmd.exe"