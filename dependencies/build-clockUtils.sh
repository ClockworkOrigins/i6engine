cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

ARCHIVE="clockutils-0.5.0-src.zip"
BUILD_DIR="${BUILD_ROOT}/clockutils-0.5.0-src"

PREFIX="${DEP_DIR}/clockUtils"
DEBUG_FLAG="DEBUG"
RELEASE_FLAG="RELEASE"
PARALLEL_FLAG=""
BUILD_TYPE="Release"

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ ! -z "${CLEAN}" ]; then
	rm -rf "${PREFIX}"
	exit 0
fi

if [ -d ${PREFIX} ]; then
	exit 0
fi

rm -rf "${PREFIX}"

./download-dependency.sh ${ARCHIVE} http://clockwork-origins.de/clockUtils/downloads/


cd "${BUILD_ROOT}"
unzip "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

if [ -e "CMakeCache.txt" ]; then
	rm "CMakeCache.txt"
	make clean
fi

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DWITH_TESTING=OFF\
 -DWITH_LIBRARY_COMPRESSION=ON\
 -DWITH_LIBRARY_SOCKETS=ON\
 -DWITH_LIBRARY_INIPARSER=ON\
 -DWITH_LIBRARY_ARGPARSER=OFF\
 -DCMAKE_CXX_COMPILER=g++\
 .

make -j 8

make install

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"
