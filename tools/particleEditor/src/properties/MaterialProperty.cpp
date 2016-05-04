#include "properties/MaterialProperty.h"

#include "OGRE/OgreMaterialManager.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>

namespace i6e {
namespace particleEditor {
namespace properties {

	MaterialProperty::MaterialProperty(QWidget * par, QString label, QString name, QString value) : Property(par, label, name), _value(value), _lineEdit(nullptr) {
		_lineEdit = new QLineEdit(_value, this);
		horizontalLayout->addWidget(_lineEdit);
		QPushButton * materialButton = new QPushButton("...", this);
		horizontalLayout->addWidget(materialButton);
		connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(stringChanged()));
		connect(materialButton, SIGNAL(clicked()), this, SLOT(selectMaterial()));
	}

	MaterialProperty::~MaterialProperty() {
	}

	void MaterialProperty::setString(QString value) {
		_value = value;
		_lineEdit->setText(value);
	}

	void MaterialProperty::stringChanged() {
		if (_value != _lineEdit->text()) {
			_value = _lineEdit->text();
			triggerChangedSignal();
		}
	}

	void MaterialProperty::selectMaterial() {
		QStringList materialList;
		Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
		while (materialIterator.hasMoreElements()) {
			materialList.append(QString::fromStdString(materialIterator.peekNextValue()->getName()));
			materialIterator.moveNext();
		}
		QInputDialog dlg(this);
		dlg.setComboBoxEditable(false);
		dlg.setComboBoxItems(materialList);
		dlg.setWindowTitle(QApplication::tr("Select Material"));
		dlg.setLabelText(QApplication::tr("Select a material:"));
		dlg.setInputMode(QInputDialog::InputMode::TextInput);
		if (QDialog::Accepted == dlg.exec()) {
			_lineEdit->setText(dlg.textValue());
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
