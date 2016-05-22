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
