/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/utils/Profiling.h"

#include <iostream>

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include "sys/times.h"
	#include "sys/vtimes.h"
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	#include <Windows.h>
#endif

#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ptree.hpp"

// for future windows support: http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

namespace i6e {
namespace utils {
namespace profiling {

	static clock_t lastCPU, lastSysCPU, lastUserCPU;
	static uint32_t numProcessors;

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	static HANDLE self;
#endif

	bool packetDump = false;
	bool fps = false;
	bool fpsCrit = false;
	double fpsRed = 0.9;
	bool waitingMsg = false;
	bool countMsg = false;
	bool timeStamp = false;
	bool numberObjects = false;
	bool numberMessages = false;
	bool profileCPU = false;
	bool profileMemory = false;

	uint32_t parseLine(char * line) {
		size_t i = strlen(line);
		while (*line < '0' || *line > '9') {
			line++;
		}
		line[i - 3] = '\0';
		i = size_t(atoi(line));
		return uint32_t(i);
	}

	uint32_t getVRAMValue() { // Note: this value is in KB!
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		FILE * file = fopen("/proc/self/status", "r");
		uint32_t result = UINT32_MAX;
		char line[128];

		while (fgets(line, 128, file) != nullptr) {
			if (strncmp(line, "VmSize:", 7) == 0) {
				result = parseLine(line);
				break;
			}
		}
		fclose(file);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		uint32_t result = uint32_t(memInfo.ullTotalPageFile - memInfo.ullAvailPageFile);
#endif
		return result;
	}

	uint32_t getPRAMValue() { // Note: this value is in KB!
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		FILE * file = fopen("/proc/self/status", "r");
		uint32_t result = UINT32_MAX;
		char line[128];

		while (fgets(line, 128, file) != nullptr) {
			if (strncmp(line, "VmRSS:", 6) == 0) {
				result = parseLine(line);
				break;
			}
		}
		fclose(file);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		uint32_t result = uint32_t(memInfo.ullTotalPhys - memInfo.ullAvailPhys);
#endif
		return result;
	}

	void init() {
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		FILE * file;
		struct tms timeSample;
		char line[128];

		lastCPU = times(&timeSample);
		lastSysCPU = timeSample.tms_stime;
		lastUserCPU = timeSample.tms_utime;

		file = fopen("/proc/cpuinfo", "r");
		numProcessors = 0;
		while(fgets(line, 128, file) != nullptr) {
			if (strncmp(line, "processor", 9) == 0) {
				numProcessors++;
			}
		}
		fclose(file);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
#endif
	}

	double getCurrentValue() {
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		struct tms timeSample;
		clock_t now;
		double percent = 0.0;

		now = times(&timeSample);
		if (now <= lastCPU || timeSample.tms_stime < lastSysCPU || timeSample.tms_utime < lastUserCPU) {
			// Overflow detection. Just skip this value.
			percent = -1.0;
		} else {
			percent = (timeSample.tms_stime - lastSysCPU) + (timeSample.tms_utime - lastUserCPU);
			percent /= (now - lastCPU);
			percent /= numProcessors;
			percent *= 100;
		}
		lastCPU = now;
		lastSysCPU = timeSample.tms_stime;
		lastUserCPU = timeSample.tms_utime;
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		double percent = 0.0;
#endif

		return percent;
	}

	void parse(const std::string & file) {
		boost::property_tree::ptree pt;
		boost::property_tree::read_info(file, pt);

		packetDump = pt.get("packetDump", packetDump);
		fps = pt.get("fps", fps);
		fpsCrit = pt.get("fpsCrit", fpsCrit);
		fpsRed = pt.get("fpsRed", fpsRed);
		waitingMsg = pt.get("waitingMsg", waitingMsg);
		countMsg = pt.get("countMsg", countMsg);
		timeStamp = pt.get("timeStamp", timeStamp);
		numberObjects = pt.get("numberObjects", numberObjects);
		numberMessages = pt.get("numberMessages", numberMessages);
		profileCPU = pt.get("profileCPU", profileCPU);
		profileMemory = pt.get("profileMemory", profileMemory);

		init();
	}

	void calculateMemoryUsage() {
		std::cout << "PRAM: " << getPRAMValue() << "KB" << std::endl;
		std::cout << "VRAM: " << getVRAMValue() << "KB" << std::endl;
	}

	void calculateCPUUsage() {
		std::cout << "CPU: " << getCurrentValue() << "%" << std::endl;
	}

} /* namespace profiling */
} /* namespace utils */
} /* namespace i6e */
