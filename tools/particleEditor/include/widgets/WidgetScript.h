#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETSCRIPT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETSCRIPT_H__

#include "ui_widgetScript.h"

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	class WidgetScript : public QWidget, public Ui::scriptWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetScript(QWidget * par);

		/**
		 * \brief destructor
		 */
		~WidgetScript();

		QString getScript() const;

		void parseScript();

	signals:
		void notifyChanged();

	public slots:
		void loadScript(ParticleUniverse::ParticleSystem * system);

	private slots:
		void changedText();

	private:
		bool _changeable;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETSCRIPT_H__ */
