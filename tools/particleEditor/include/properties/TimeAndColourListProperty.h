#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_TIMEANDCOLOURLISTPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_TIMEANDCOLOURLISTPROPERTY_H__

#include "properties/Property.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class TimeAndColourProperty;

	class TimeAndColourListProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TimeAndColourListProperty(QWidget * par, QString label, QString name);

		/**
		 * \brief destructor
		 */
		~TimeAndColourListProperty();

		void setTimeAndColourList(std::vector<std::pair<double, Vec4>> value) override;

		std::vector<std::pair<double, Vec4>> getTimeAndColourList() const override;

	private slots:
		void addTimeAndColour();
		void changedValue();

	private:
		QVBoxLayout * _layout;
		std::vector<TimeAndColourProperty *> _timeAndColours;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_TIMEANDCOLOURLISTPROPERTY_H__ */
