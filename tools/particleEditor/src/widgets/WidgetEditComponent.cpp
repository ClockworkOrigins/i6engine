#include "widgets/WidgetEditComponent.h"

#include "connections/Connection.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetEditComponent::WidgetEditComponent(QGraphicsScene * scene, QString name, QString type, QString subType) : QGraphicsWidget(), _name(name), _type(type), _subType(subType), _element(nullptr), _policies(), _uniqueRelations(), _connections() {
		QString labelText = type + " (" + subType + ")";
		QWidget * widget = new QWidget();
		QHBoxLayout * hLayout = new QHBoxLayout(widget);
		widget->setLayout(hLayout);
		QLabel * label = new QLabel(widget);
		hLayout->addWidget(label);
		QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(this);
		setLayout(layout);
		layout->addItem(scene->addWidget(widget));
		if (!name.isEmpty()) {
			labelText += " - " + name;
		}
		label->setText(labelText);

		if (type == CT_SYSTEM) {
			widget->setStyleSheet("background: black");
			label->setStyleSheet("color: white");
		} else if (type == CT_TECHNIQUE) {
			widget->setStyleSheet("background: " + QVariant(QColor(102, 152, 255)).toString() + ";");
		} else if (type == CT_RENDERER) {
			widget->setStyleSheet("background: " + QVariant(QColor(228, 34, 23)).toString() + ";");
		} else if (type == CT_EMITTER) {
			widget->setStyleSheet("background: " + QVariant(QColor(76, 196, 23)).toString() + ";");
		} else if (type == CT_OBSERVER) {
			widget->setStyleSheet("background: " + QVariant(QColor(37, 65, 23)).toString() + ";");
		} else if (type == CT_AFFECTOR) {
			widget->setStyleSheet("background: " + QVariant(QColor(251, 177, 23)).toString() + ";");
		} else if (type == CT_HANDLER) {
			widget->setStyleSheet("background: " + QVariant(QColor(141, 56, 201)).toString() + ";");
		} else if (type == CT_BEHAVIOUR) {
			widget->setStyleSheet("background: " + QVariant(QColor(48, 125, 126)).toString() + ";");
		} else if (type == CT_EXTERN) {
			widget->setStyleSheet("background: " + QVariant(QColor(130, 123, 96)).toString() + ";");
		}

		resize(200, 80);
	}

	WidgetEditComponent::~WidgetEditComponent() {
	}

	void WidgetEditComponent::addPolicy(ComponentRelation relation, ComponentRelationDirection relationDirection, const QString & relationDescription, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith, bool multipleConnectionsPossible, bool ignoreSubType, const QColor & colour, Qt::PenStyle lineStyle) {
		_policies.push_back(new connections::ConnectionPolicy(relation, relationDirection, relationDescription, typeToBeConnectedWith, subTypeToBeConnectedWith, multipleConnectionsPossible, ignoreSubType, colour, lineStyle));
	}

	void WidgetEditComponent::addUniqueRelation(ComponentRelation relation, ComponentRelationDirection relationDirection) {
		_uniqueRelations.push_back(new connections::UniqueRelation(relation, relationDirection));
	}

	void WidgetEditComponent::addConnection(WidgetEditComponent * componentToBeConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection) {
		_connections.push_back(new connections::Connection(componentToBeConnectedWith, relation, relationDirection));

		// Loop through the policies and possibly lock it
		for (std::vector<connections::ConnectionPolicy *>::iterator it = _policies.begin(); it != _policies.end(); ++it) {
			if (isRelationUnique(relation, relationDirection)) {
				// Ignore a possible policy setting of mMultipleConnectionsPossible = true and just override
				(*it)->validateAndLock(relation, relationDirection);
			} else {
				(*it)->validateAndLock(relation, relationDirection, componentToBeConnectedWith->getComponentType(), componentToBeConnectedWith->getComponentSubType());
			}
		}
	}
	
	bool WidgetEditComponent::isRelationUnique(ComponentRelation relation, ComponentRelationDirection relationDirection) const {
		for (std::vector<connections::UniqueRelation *>::const_iterator it = _uniqueRelations.begin(); it != _uniqueRelations.end(); ++it) {
			if ((*it)->isRelationUnique(relation, relationDirection)) {
				return true;
			}
		}
		return false;
	}

	bool WidgetEditComponent::isConnected(WidgetEditComponent * componentToBeConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection) const {
		connections::Connection * connection;
		for (std::vector<connections::Connection *>::const_iterator it = _connections.begin(); it != _connections.end(); ++it) {
			connection = *it;
			if (connection->getComponentToBeConnectedWith() == componentToBeConnectedWith && connection->getRelation() == relation && connection->getRelationDirection() == relationDirection) {
				return true;
			}
		}
		return false;
	}
	
	connections::ConnectionPolicy * WidgetEditComponent::getPolicy(ComponentRelation relation, ComponentRelationDirection relationDirection, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith) const {
		connections::ConnectionPolicy * policy = nullptr;
		for (std::vector<connections::ConnectionPolicy *>::const_iterator it = _policies.begin(); it != _policies.end(); ++it) {
			policy = *it;
			if (policy->getRelation() == relation && policy->getRelationDirection() == relationDirection && policy->getTypeToBeConnectedWith() == typeToBeConnectedWith) {
				if (policy->ignoreSubType()) {
					break;
				} else if (policy->getSubTypeToBeConnectedWith() == subTypeToBeConnectedWith) {
					break;
				}
			}
		}
		return policy;
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
