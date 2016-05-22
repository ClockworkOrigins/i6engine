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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

#include <cstdint>

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	class PropertyWindow;
	class WidgetEdit;
	class WidgetParticleList;
	class WidgetRender;
	class WidgetScript;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	signals:
		void triggerPlay();
		void triggerPause();
		void triggerStop();
		void triggerCreateNewSystem(const QString & particle);
		void updateScript(ParticleUniverse::ParticleSystem *);

	private slots:
		void closeEditor();
		void handleNewAction();
		void handleCloneAction();
		void handlePlayAction();
		void handlePauseAction();
		void handleStopAction();
		void createNewSystem(const QString & particle);
		void tabChanged(int index);
		void setPropertyWindow(PropertyWindow * propertyWindow);
		void renameParticleSystem(QString oldName, QString newName);
		void openOptions();

	private:
		enum class CurrentTab {
			Render,
			Edit,
			Script
		};
		QWidget * _renderWrapper;
		WidgetRender * _renderWidget;
		WidgetParticleList * _particleListWidget;
		WidgetEdit * _editWidget;
		WidgetScript * _scriptWidget;
		QTabWidget * _tabWidget;
		QToolBar * _toolBarEdit;
		bool _playing;
		std::map<QString, QAction *> _toolbarActions;
		QString _currentParticleTemplate;
		CurrentTab _currentTab;
		uint32_t _particleSystemCounter;
		QVBoxLayout * _leftLayout;
		QWidget * _currentPropertyWindow;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__ */
