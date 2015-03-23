Set FILE=%1

call build-common.bat

mkdir %EX_DIR%

IF not exist %EX_DIR%\%FILE% (bitsadmin /transfer "myDownloadJob%FILE%" /download /priority normal http://www.clockwork-origins.de/dependencies/%FILE% %EX_DIR%\%FILE%)