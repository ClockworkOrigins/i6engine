#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_PROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_PROPERTYWINDOW_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class PropertyWindow : public QWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PropertyWindow(QWidget * par, QString name);

		/**
		 * \brief destructor
		 */
		virtual ~PropertyWindow();

		void setComponentName(QString name) {
			_name = name;
		}

		QString getComponentName() const {
			return _name;
		}

	private:
		QString _name;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_PROPERTYWINDOW_H__ */
