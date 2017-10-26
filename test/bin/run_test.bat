@ECHO OFF
SET HOUR=%time:~0,2%
SET dtStamp9=%date:~-4%%date:~4,2%%date:~7,2%_0%time:~1,1%%time:~3,2%%time:~6,2% 
SET dtStamp24=%date:~-4%%date:~4,2%%date:~7,2%_%time:~0,2%%time:~3,2%%time:~6,2%

if "%HOUR:~0,1%" == " " (SET dtStamp=%dtStamp9%) else (SET dtStamp=%dtStamp24%)

ECHO Date time is Time is %dtStamp%
ECHO 
C:/Users/Administrator/Documents/MCHA3000/Coding/workspace/Project/test/bin/test_project.exe -v > "Unity_%dtStamp%"_Log.txt"
type "Unity_%dtStamp%_Log.txt"