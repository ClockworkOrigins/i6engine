Set FILE=%1

call build-common.bat

IF not exist %TMP_DIR% (mkdir %TMP_DIR%)

IF not exist %TMP_DIR%\%FILE% (bitsadmin /transfer "myDownloadJob%FILE%" /download /priority normal http://www.clockwork-origins.de/dependencies/%FILE% %TMP_DIR%\%FILE%)
