cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

ARCHIVE="openal-soft-1.16.0.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/openal-soft-1.16.0"

PREFIX="${DEP_DIR}/OpenAL"
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

./download-dependency.sh ${ARCHIVE}


cd "${BUILD_ROOT}"
tar xfvj "${ARCHIVE}" > /dev/null

cd "${BUILD_DIR}"

if [ -e "CMakeCache.txt" ]; then
	rm "CMakeCache.txt"
	make clean
fi

cd "build"

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DALSOFT_NO_CONFIG_UTIL=ON\
 -DCMAKE_CXX_COMPILER=g++\
 ..

make -j 8 > /dev/null

make install > /dev/null

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" > /dev/null

touch "${PREFIX}"
