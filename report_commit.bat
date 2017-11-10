@ECHO OFF
ECHO git add . | "C:\Program Files\Git\git-cmd.exe"
set "DT=%date%_%time%"
ECHO git commit -m "REPORT COMMIT: %DT%" | "C:\Program Files\Git\git-cmd.exe"
ECHO git push -u origin master | "C:\Program Files\Git\git-cmd.exe"
ECHO "Report or Matlab Commit ONLY. No tested code or upload."
