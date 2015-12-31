#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCLISTWIDGET_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCLISTWIDGET_H__

#include "ui_widgetNPCList.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6engine {
namespace tools {
namespace npcCreator {
namespace widgets {

	class NPCEditWidget;

	class NPCListWidget : public QWidget, public Ui::npcListWidget {
		Q_OBJECT

	public:
		NPCListWidget(QWidget * par = nullptr);
		~NPCListWidget();

		void setNPCEditWidget(NPCEditWidget * npcEditWidget) {
			_npcEditWidget = npcEditWidget;
		}

		void refreshNPCList();

	private slots:
		void selectIndex(QModelIndex index);
		void changedFilter(QString filter);

	private:
		clockUtils::iniParser::IniParser _iniParser;
		std::vector<std::pair<std::string, std::string>> _npcFileList;
		std::vector<std::pair<std::string, std::string>> _filteredNpcFileList;
		QString _filter;
		NPCEditWidget * _npcEditWidget;
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6engine */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCLISTWIDGET_H__ */
