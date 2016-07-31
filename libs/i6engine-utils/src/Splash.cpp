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

#include "i6engine/utils/Splash.h"

#include "i6engine/utils/Logger.h"

namespace i6e {
namespace utils {

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	LRESULT CALLBACK fnWndProc(HWND hwnd, unsigned int msg, WPARAM wParam, LPARAM lParam) {
		return (DefWindowProc(hwnd, msg, wParam, lParam));
	}
#endif

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	static Display * dis = XOpenDisplay(nullptr);
#endif

	Splash::Splash() {
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		_img = nullptr;
#endif
	}

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	int Splash::get_byte_order() {
		union {
			char c[sizeof(short)];
			short s;
		} order;

		order.s = 1;

		if (1 == order.c[0]) {
			return LSBFirst;
		} else {
			return MSBFirst;
		}
	}

	void Splash::jpeg_error_exit(j_common_ptr cinfo) {
		cinfo->err->output_message(cinfo);
		exit(EXIT_FAILURE);
	}

	u_char * Splash::decode_jpeg(const char * filename, int * widthPtr, int * heightPtr) {
		/* This returns an array for a 24 bit image. */
		JSAMPARRAY lineBuf;
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr err_mgr;
		int bytesPerPix;
		FILE * inFile;
		u_char * retBuf;

		inFile = fopen(filename, "rb");

		if (nullptr == inFile) {
			perror(nullptr);
			return nullptr;
		}

		cinfo.err = jpeg_std_error(&err_mgr);
		// err_mgr.error_exit = jpeg_error_exit;

		jpeg_CreateDecompress((&cinfo), JPEG_LIB_VERSION, static_cast<size_t>(sizeof(struct jpeg_decompress_struct)));
		jpeg_stdio_src(&cinfo, inFile);
		jpeg_read_header(&cinfo, 1);
		cinfo.do_fancy_upsampling = 0;
		cinfo.do_block_smoothing = 0;
		jpeg_start_decompress(&cinfo);

		*widthPtr = static_cast<int>(cinfo.output_width);
		*heightPtr = static_cast<int>(cinfo.output_height);
		bytesPerPix = cinfo.output_components;

		lineBuf = cinfo.mem->alloc_sarray(reinterpret_cast<j_common_ptr>(&cinfo), JPOOL_IMAGE, static_cast<JDIMENSION>(*widthPtr * bytesPerPix), 1);
		retBuf = reinterpret_cast<unsigned char *>(malloc(3 * static_cast<size_t>(*widthPtr * *heightPtr)));

		if (nullptr == retBuf) {
			perror(nullptr);
			return nullptr;
		}

		if (3 == bytesPerPix) {
			unsigned int lineOffset = (static_cast<unsigned int>(*widthPtr) * 3);
			unsigned int x;
			unsigned int y;

			for (y = 0; y < cinfo.output_height; ++y) {
				jpeg_read_scanlines(&cinfo, lineBuf, 1);

				for (x = 0; x < lineOffset; ++x) {
					retBuf[(lineOffset * y) + x] = lineBuf[0][x];
					++x;
					retBuf[(lineOffset * y) + x] = lineBuf[0][x];
					++x;
					retBuf[(lineOffset * y) + x] = lineBuf[0][x];
				}
			}
		} else if (1 == bytesPerPix) {
			unsigned int col;
			unsigned int lineOffset = (static_cast<unsigned int>(*widthPtr) * 3);
			unsigned int lineBufIndex;
			unsigned int x;
			unsigned int y;

			for (y = 0; y < cinfo.output_height; ++y) {
				jpeg_read_scanlines(&cinfo, lineBuf, 1);

				lineBufIndex = 0;
				for (x = 0; x < lineOffset; ++x) {
					col = lineBuf[0][lineBufIndex];

					retBuf[(lineOffset * y) + x] = col;
					++x;
					retBuf[(lineOffset * y) + x] = col;
					++x;
					retBuf[(lineOffset * y) + x] = col;

					++lineBufIndex;
				}
			}
		} else {
			ISIXE_LOG_ERROR("Splash", "The number of color channels is " << bytesPerPix << ". This program only handles 1 or 3.");
			free(retBuf);
			return nullptr;
		}

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(inFile);
		return retBuf;
	}

	XImage * Splash::create_image_from_buffer(Display * displ, int screen, u_char * buf, int width, int height) {
		int depth;
		XImage * img = nullptr;
		Visual * vis;
		double rRatio;
		double gRatio;
		double bRatio;
		int outIndex = 0;
		int i;
		int numBufBytes = (3 * (width * height));

		depth = ((&(reinterpret_cast<_XPrivDisplay>(displ))->screens[screen])->root_depth);
		vis = ((&(reinterpret_cast<_XPrivDisplay>(displ))->screens[screen])->root_visual);

		rRatio = vis->red_mask / 255.0;
		gRatio = vis->green_mask / 255.0;
		bRatio = vis->blue_mask / 255.0;

		u_int32_t * newBuf;

		if (depth >= 24) {
			size_t numNewBufBytes = (4 * (static_cast<size_t>(width) * static_cast<size_t>(height)));
			newBuf = reinterpret_cast<u_int32_t *>(malloc(numNewBufBytes));

			for (i = 0; i < numBufBytes; ++i) {
				unsigned int r, g, b;
				r = (buf[i] * rRatio);
				++i;
				g = (buf[i] * gRatio);
				++i;
				b = (buf[i] * bRatio);

				r &= vis->red_mask;
				g &= vis->green_mask;
				b &= vis->blue_mask;

				newBuf[outIndex] = r | g | b;
				++outIndex;
			}

			img = XCreateImage(displ, CopyFromParent, static_cast<unsigned int>(depth), ZPixmap, 0, reinterpret_cast<char *>(newBuf), static_cast<unsigned int>(width), static_cast<unsigned int>(height), 32, 0);
		} else {
			ISIXE_LOG_ERROR("Splash", "This program does not support displays with a depth less than 24.");
			return nullptr;
		}

		XInitImage(img);
		/* Set the client's byte order, so that XPutImage knows what to do with the data. */
		/* The default in a new X image is the server's format, which may not be what we want. */
		if (LSBFirst == get_byte_order()) {
			img->byte_order = LSBFirst;
		} else {
			img->byte_order = MSBFirst;
		}

		/* The bitmap_bit_order doesn't matter with ZPixmap images. */
		img->bitmap_bit_order = MSBFirst;

		return img;
	}

	Window Splash::create_window(Display * displ, int screen, int x, int y, int width, int height, const std::string & name) {
		Window win;

		unsigned long windowMask;
		XSetWindowAttributes winAttrib;

		windowMask = CWBackPixel | CWBorderPixel;

		win = XCreateWindow(displ, ((&(reinterpret_cast<_XPrivDisplay>(displ))->screens[((reinterpret_cast<_XPrivDisplay>(displ))->default_screen)])->root), x, y, static_cast<unsigned int>(width), static_cast<unsigned int>(height), 0, ((&(reinterpret_cast<_XPrivDisplay>(displ))->screens[screen])->root_depth), InputOutput, CopyFromParent, windowMask, &winAttrib);
		XStoreName(displ, win, name.c_str());

		return win;
	}
#endif

	void Splash::closeSplash() {
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		XCloseDisplay(dis);
		XDestroyImage(_img);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		DestroyWindow(hWnd);
#endif
	}

	void Splash::showSplash(const std::string & file, const std::string & name) {
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		int imageWidth;
		int imageHeight;
		unsigned int display_width, display_height;	/* height and width of the X display.    */
		Window mainWin;
		int screen, x, y;
		u_char * buf;
		GC copyGC;
		/* Filename of Splash Image (must be jpeg) */
		const char * splash_image_jpg = file.c_str();

		buf = decode_jpeg(splash_image_jpg, &imageWidth, &imageHeight);

		if (buf == nullptr) {
			ISIXE_LOG_FATAL("Splash", "unable to decode JPEG");
			exit(EXIT_FAILURE);
		}

		screen = ((reinterpret_cast<_XPrivDisplay>(dis))->default_screen);
		_img = create_image_from_buffer(dis, screen, buf, imageWidth, imageHeight);

		/* create_image_from_buffer creates a new buffer after translation, so we can free. */
		free(buf);

		if (nullptr == _img) {
			exit(EXIT_FAILURE);
		}

		/* get center coordinates */
		display_width = static_cast<unsigned int>(((&(reinterpret_cast<_XPrivDisplay>(dis))->screens[static_cast<unsigned int>(screen)])->width));
		display_height = static_cast<unsigned int>(((&(reinterpret_cast<_XPrivDisplay>(dis))->screens[static_cast<unsigned int>(screen)])->height));
		x = static_cast<int>((static_cast<unsigned int>(display_width) / 2) - (static_cast<unsigned int>(imageWidth) / 2));
		y = static_cast<int>((static_cast<unsigned int>(display_height) / 2) - (static_cast<unsigned int>(imageHeight) / 2));

		mainWin = create_window(dis, screen, x, y, imageWidth, imageHeight, name);
		copyGC = XCreateGC(dis, mainWin, 0, nullptr);
		XMapRaised(dis, mainWin);

		XSelectInput(dis, mainWin, ExposureMask);

		XEvent event;
		XNextEvent(dis, &event);

		// move window to center
		// XMoveResizeWindow(dis, mainWin, x, y, imageWidth, imageHeight);

		// move window to bottom left corner
		XMoveResizeWindow(dis, mainWin, 0, (static_cast<int>(display_height) - imageHeight), static_cast<unsigned int>(imageWidth), static_cast<unsigned int>(imageHeight));

		switch (event.type) {
			case Expose:
				XPutImage(dis, mainWin, copyGC, _img, 0, 0, 0, 0, static_cast<unsigned int>(imageWidth), static_cast<unsigned int>(imageHeight));
				XFlush(dis);
				break;
			default:
				break;
		}

		XFreeGC(dis, copyGC);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		TCHAR * szClassName = const_cast<char *>(name.c_str());
		WNDCLASSEX wc;
		BITMAP qB;

		wc.lpszClassName = szClassName;                     //Important Field!  Character string identifying window class
		wc.lpfnWndProc = fnWndProc;                       //Important Field!  Function Pointer.  Address of Window Procedure
		wc.cbSize = sizeof(WNDCLASSEX);             //Those top two fields I just listed are very important.  The
		wc.style = 0;                               //others are of course necessary too, but fully understanding all
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //the implications of the .szClassName and .lpfnWndProc fields will
		wc.hInstance = nullptr;                       //go a long way to helping you understand Win32 coding. The
		wc.hIconSm = 0;                               //.hBrushBackground field will be the color of the Window's
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);      //background.  The .cbWndExtra field is very useful as it allows
		//wc.hbrBackground = (HBRUSH) COLOR_BTNSHADOW;         //you to associate object (Window) data to the instantiated Window's
		wc.hbrBackground = CreatePatternBrush((HBITMAP)::LoadImage(nullptr, (LPCSTR) file.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		wc.cbWndExtra = 0;                               //internal structure, i.e., accomodate member data.
		wc.cbClsExtra = 0;
		wc.lpszMenuName = NULL;
		RegisterClassEx(&wc);
		HBITMAP hBitmap = (HBITMAP)::LoadImage(nullptr, (LPCSTR) file.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (!hBitmap) {
			return;
		}
		GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qB));

		hWnd = CreateWindowEx(0, szClassName, szClassName, WS_POPUP, 0, 0, qB.bmWidth, qB.bmHeight, HWND_DESKTOP, 0, nullptr, 0);

		RECT rc;
		GetWindowRect(hWnd, &rc);

		int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
		int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

		::SetForegroundWindow(hWnd);

		SetWindowPos(hWnd, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_NOSIZE);

		ShowWindow(hWnd, SW_SHOW);
#endif
	}

} /* namespace utils */
} /* namespace i6e */
