/*
 * SocketPerformanceTester
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of SocketPerformanceTester; SocketPerformanceTester is free software; you can redistribute it and/or
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

#ifndef __I6ENGINE_TOOLS_COMMON_ABOUTDIALOG_H__
#define __I6ENGINE_TOOLS_COMMON_ABOUTDIALOG_H__

#include <QDialog>

namespace i6e {
namespace tools {
namespace common {

	class AboutDialog : public QDialog {
		Q_OBJECT

	public:
		AboutDialog(QWidget * parent);
		~AboutDialog();
	};

} /* namespace common */
} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_COMMON_ABOUTDIALOG_H__ */
