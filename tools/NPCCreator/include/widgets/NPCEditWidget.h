#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCEDITWIDGET_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCEDITWIDGET_H__

#include "ui_widgetNPCEdit.h"

namespace i6engine {
namespace tools {
namespace npcCreator {
namespace widgets {

	class MainWindow;
	class NPCListWidget;

	class NPCEditWidget : public QWidget, public Ui::npcEditWidget {
		Q_OBJECT

	public:
		NPCEditWidget(QWidget * par = nullptr);
		~NPCEditWidget();

		void parseNPC(const std::vector<std::pair<std::string, std::string>> & npcFileList, int index);

		void setNPCListWidget(NPCListWidget * npcListWidget) {
			_npcListWidget = npcListWidget;
		}

	private slots:
		void changedIdentifier();
		void changedEntry();
		void saveNewNPC();
		void saveChanges();
		void selectModel();

	private:
		MainWindow * _parent;
		NPCListWidget * _npcListWidget;
		bool _changed;
		bool _changedIdentifier;
		QString _windowTitle;
		QString _currentIdentifier;
		std::string _currentFile;
		std::vector<std::pair<std::string, std::string>> _npcFileList;

		void saveToFile(const std::string & file);
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6engine */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCEDITWIDGET_H__ */
