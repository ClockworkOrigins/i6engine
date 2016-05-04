#include "EditorTypeQuaternion.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

namespace i6e {
namespace plugins {

	EditorTypeQuaternionWidget::EditorTypeQuaternionWidget(QWidget * parent) : TypeWidgetInterface(parent), _lineEditX(nullptr), _lineEditY(nullptr), _lineEditZ(nullptr), _spinBox(nullptr) {
		QGridLayout * layout = new QGridLayout(this);
		_lineEditX = new QLineEdit(this);
		QLabel * labelX = new QLabel("X", this);
		_lineEditY = new QLineEdit(this);
		QLabel * labelY = new QLabel("Y", this);
		_lineEditZ = new QLineEdit(this);
		QLabel * labelZ = new QLabel("Z", this);
		_spinBox = new QDoubleSpinBox(this);
		_spinBox->setMinimum(-360.0);
		_spinBox->setMaximum(360.0);
		QLabel * labelAngle = new QLabel("Angle", this);
		layout->addWidget(labelX, 0, 0);
		layout->addWidget(_lineEditX, 0, 1);
		layout->addWidget(labelY, 0, 2);
		layout->addWidget(_lineEditY, 0, 3);
		layout->addWidget(labelZ, 0, 4);
		layout->addWidget(_lineEditZ, 0, 5);
		layout->addWidget(labelAngle, 1, 2);
		layout->addWidget(_spinBox, 1, 3);
		setLayout(layout);
	}

	void EditorTypeQuaternionWidget::setValues(const std::string & values) {
		Quaternion quat(values);
		Vec3 vec;
		double angle;
		quat.toAxisAngle(vec, angle);
		_lineEditX->setText(QString::number(vec.getX()));
		_lineEditY->setText(QString::number(vec.getY()));
		_lineEditZ->setText(QString::number(vec.getZ()));
		_spinBox->setValue(angle);
	}

	std::string EditorTypeQuaternionWidget::getValues() const {
		Quaternion quat(Vec3((_lineEditX->text() + " " + _lineEditY->text() + " " + _lineEditZ->text()).toStdString()), _spinBox->value());
		return (std::to_string(quat.getW()) + " " + std::to_string(quat.getX()) + " " + std::to_string(quat.getY()) + " " + std::to_string(quat.getZ()));
	}

	void EditorTypeQuaternionWidget::setReadOnly(bool readOnly) {
		_lineEditX->setReadOnly(readOnly);
		_lineEditY->setReadOnly(readOnly);
		_lineEditZ->setReadOnly(readOnly);
		_spinBox->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeQuaternion::createWidget(QWidget * parent) {
		return new EditorTypeQuaternionWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
