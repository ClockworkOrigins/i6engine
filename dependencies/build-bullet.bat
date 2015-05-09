call build-common.bat

Set ARCHIVE=bullet-2.82-r2704.tgz
Set BUILD_DIR=%TMP_DIR%/bullet-2.82-r2704
Set PREFIX=%DEP_DIR%/bullet/

echo "Compile Bullet"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

call download-dependency.bat bullet_2_82_patch.zip

winrar.exe x bullet_2_82_patch.zip

echo "Patching Bullet"
REM xcopy /S /Y "%TMP_DIR%/Windows/bullet/Extras" "%BUILD_DIR%/Extras"

echo "Configuring Bullet"
cd %BUILD_DIR%
cmake . -DCMAKE_INSTALL_PREFIX=%PREFIX% -DBUILD_DEMOS=OFF -DINSTALL_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DUSE_DOUBLE_PRECISION=ON -DUSE_GRAPHICAL_BENCHMARK=OFF -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -DBUILD_CPU_DEMOS=OFF -DBUILD_INTEL_OPENCL_DEMOS=OFF -DBUILD_EXTRAS=ON -DBUILD_MINICL_OPENCL_DEMOS=OFF -DINSTALL_EXTRA_LIBS=ON -DUSE_DX11=OFF -DUSE_GLUT=ON

echo "Building Bullet"
MSBuild.exe BULLET_PHYSICS.sln /p:Configuration=Release > NUL

echo "Installing Bullet"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"

RD /S /Q "%TMP_DIR%"