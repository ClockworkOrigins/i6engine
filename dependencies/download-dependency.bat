Set FILE=%1

call build-common.bat

mkdir %EX_DIR%

bitsadmin /transfer "myDownloadJob%FILE%" /download /priority normal http://www.clockwork-origins.de/dependencies/%FILE% %EX_DIR%\%FILE%