/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_SPLASH_H__
#define __I6ENGINE_UTILS_SPLASH_H__

#include <cstdio>
#include <stdint.h>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include "jerror.h"
	#include "jpeglib.h"

	#include <X11/Xatom.h>
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	#include <Windows.h>
#endif /* WIN32 */

#ifndef u_char
	#define u_char unsigned char
#endif

namespace i6e {
namespace utils {

	typedef struct {
		unsigned long  flags;
		unsigned long  functions;
		unsigned long  decorations;
		long      inputMode;
		unsigned long  status;
	} Hints;

	/**
	 * \class Splash
	 * \brief Opens and displays JPEG (Linux) or BMP (Windows) images as splash screens.
	 *
	 * Opens given file, saves its decoded contents in a buffer and then displays it.
	 */
	class ISIXE_UTILS_API Splash {
	public:
		/**
		 * \brief Displays the splash screen
		 *
		 * Displays the given file in the center of the screen.
		 * This file has to be in JPEG format and its color depth has to be 15 or greater for Linux.
		 * This file has to be in BMP format saved by Paint for Windows.
		 */
		void showSplash(const std::string & file, const std::string & name);

		/**
		 * \brief Closes the splash screen
		 */
		void closeSplash();

		/**
		 * \brief Currently empty
		 */
		Splash();

	private:
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		XImage * _img;

		int get_byte_order();
		void jpeg_error_exit(j_common_ptr cinfo);
		u_char * decode_jpeg(const char * filename, int * widthPtr, int * heightPtr);
		XImage * create_image_from_buffer(Display * dis, int screen, u_char * buf, int width, int height);
		Window create_window(Display * dis, int screen, int x, int y, int width, int height, const std::string & name);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		HWND hWnd;
#endif
	};

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_SPLASH_H__ */

/**
 * @}
 */
