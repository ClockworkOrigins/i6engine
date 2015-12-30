#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

#include "i6engine/rpg/RPGApplication.h"

namespace i6engine {
namespace tools {
namespace npcCreator {
namespace widgets {

	class NPCEditWidget;
	class NPCListWidget;
	class NPCRenderWidget;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow, public rpg::RPGApplication {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	private slots:
		void closeCreator();

	private:
		NPCListWidget * _npcListWidget;
		NPCEditWidget * _npcEditWidget;
		NPCRenderWidget * _npcRenderWidget;

		void closeEvent(QCloseEvent * evt) override;

		void AfterInitialize() override;

		void Finalize() override;
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6engine */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_MAINWINDOW_H__ */
