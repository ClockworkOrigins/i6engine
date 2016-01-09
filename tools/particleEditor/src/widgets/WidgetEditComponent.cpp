#include "widgets/WidgetEditComponent.h"

#include "connections/Connection.h"

#include "factories/AffectorPropertyWindowFactory.h"
#include "factories/BehaviourPropertyWindowFactory.h"
#include "factories/EmitterPropertyWindowFactory.h"
#include "factories/EventHandlerPropertyWindowFactory.h"
#include "factories/ExternPropertyWindowFactory.h"
#include "factories/ObserverPropertyWindowFactory.h"
#include "factories/RendererPropertyWindowFactory.h"

#include "widgets/AffectorPropertyWindow.h"
#include "widgets/BehaviourPropertyWindow.h"
#include "widgets/DialogChooseConnectionType.h"
#include "widgets/EmitterPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindow.h"
#include "widgets/ExternPropertyWindow.h"
#include "widgets/ObserverPropertyWindow.h"
#include "widgets/RendererPropertyWindow.h"
#include "widgets/SystemPropertyWindow.h"
#include "widgets/TechniquePropertyWindow.h"
#include "widgets/WidgetEdit.h"

#include <QDataStream>
#include <QDrag>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	const int MAX_NUMBER_OF_CONNECTIONS = 20;

	WidgetEditComponent::WidgetEditComponent(WidgetEdit * parent, QGraphicsScene * scene, QString name, QString type, QString subType) : QGraphicsWidget(), _parent(parent), _label(nullptr), _name(name), _type(type), _subType(subType), _element(nullptr), _policies(), _uniqueRelations(), _connections(), _selectedPolicy(nullptr), _propertyWindow(nullptr), _oldPropertyWindow(nullptr) {
		QString labelText = type + " (" + subType + ")";
		QWidget * widget = new QWidget();
		QHBoxLayout * hLayout = new QHBoxLayout(widget);
		widget->setLayout(hLayout);
		_label = new QLabel(widget);
		hLayout->addWidget(_label);
		QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(this);
		setLayout(layout);
		layout->addItem(scene->addWidget(widget));
		if (!name.isEmpty()) {
			labelText += " - " + name;
		}
		_label->setText(labelText);

		if (type == CT_SYSTEM) {
			widget->setStyleSheet("background: black");
			_label->setStyleSheet("color: white");
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

		_propertyWindow = createPropertyWindow(_subType);
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

	bool WidgetEditComponent::isConnectionPossible(WidgetEditComponent * component) const {
		if (component) {
			for (std::vector<connections::ConnectionPolicy *>::const_iterator it = _policies.begin(); it != _policies.end(); ++it) {
				// The policy of 'this' component cannot be checked against a relation and relation direction (because there isn't one)
				connections::ConnectionPolicy * connectionPolicy = *it;
				if ((*it)->isConnectionPossible(component->getComponentType(), component->getComponentSubType())) {
					// The second one (the component) can be checked against all four attributes
					if (component->isConnectionPossible(connectionPolicy->getRelation(), getOppositeRelationDirection(connectionPolicy->getRelationDirection()), _type, _subType)) {
						return true;
					}
				}
			}
		}
		return false;
	}

	void WidgetEditComponent::setCaption() {
		QString caption = _type;
		if (_subType != CST_UNDEFINED) {
			caption = caption + " (" + _subType + ") ";
		}
		if (!_name.isEmpty()) {
			caption = caption + " - " + _name;
		}
		_label->setText(caption);
	}

	PropertyWindow * WidgetEditComponent::createPropertyWindow(QString subType, const PropertyWindow * propertyWindow) {
		// Create new propertyWindow and propagate the attributes of the old one.
		// Don't delete the existing one, because it is deleting itself.

		_subType = subType;
		delete _propertyWindow;
		_propertyWindow = nullptr;

		if (_type == CT_SYSTEM) {
			SystemPropertyWindow * pWin = new SystemPropertyWindow(_parent, _name);
			_propertyWindow = pWin;
		} else if (_type == CT_TECHNIQUE) {
			_propertyWindow = new TechniquePropertyWindow(_parent, _name);
		} else if (_type == CT_RENDERER) {
			if (propertyWindow) {
				_propertyWindow = factories::RendererPropertyWindowFactory::createRendererPropertyWindow(_subType, static_cast<RendererPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::RendererPropertyWindowFactory::createRendererPropertyWindow(_parent, _name, _subType);
			}
		} else if (_type == CT_EMITTER) {
			if (propertyWindow) {
				_propertyWindow = factories::EmitterPropertyWindowFactory::createEmitterPropertyWindow(_subType, static_cast<EmitterPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::EmitterPropertyWindowFactory::createEmitterPropertyWindow(_parent, _name, _subType);
			}
		} else if (_type == CT_AFFECTOR) {
			if (propertyWindow) {
				_propertyWindow = factories::AffectorPropertyWindowFactory::createAffectorPropertyWindow(_subType, static_cast<AffectorPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::AffectorPropertyWindowFactory::createAffectorPropertyWindow(_parent, _name, _subType);
			}
		} else if (_type == CT_OBSERVER) {
			if (propertyWindow) {
				_propertyWindow = factories::ObserverPropertyWindowFactory::createObserverPropertyWindow(_subType, static_cast<ObserverPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::ObserverPropertyWindowFactory::createObserverPropertyWindow(_parent, _name, _subType);
			}
		} else if (_type == CT_HANDLER) {
			if (propertyWindow) {
				_propertyWindow = factories::EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(_subType, static_cast<EventHandlerPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(_parent, _name, _subType);
			}
		} else if (_type == CT_BEHAVIOUR) {
			if (propertyWindow) {
				_propertyWindow = factories::BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(_subType, static_cast<BehaviourPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(_parent, _name, _subType);
			}
		} else if (_type == CT_EXTERN) {
			if (propertyWindow) {
				_propertyWindow = factories::ExternPropertyWindowFactory::createExternPropertyWindow(_subType, static_cast<ExternPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::ExternPropertyWindowFactory::createExternPropertyWindow(_parent, _name, _subType);
			}
		}
		return _propertyWindow;
	}

	void WidgetEditComponent::mousePressEvent(QGraphicsSceneMouseEvent * evt) {
		if (evt->button() == Qt::MouseButton::LeftButton) {
			if (_parent->getConnectionMode() == WidgetEdit::CM_CONNECT_ENDING && this != _parent->getStartConnector()) {
				// First make a selection of available policies
				selectPolicy(_parent->getStartConnector());
			} else if (_parent->getConnectionMode() == WidgetEdit::CM_DISCONNECT) {
				// Make a selection of available connections (to be deleted)
				selectConnection(false);
				_parent->notifyConnectionsChanged();
				return;
			} else if (_parent->getConnectionMode() == WidgetEdit::CM_CONNECT_NONE) {
				QDrag drag(this);
				QMimeData * mimeData = new QMimeData();
				QByteArray ba;
				QDataStream ds(&ba, QIODevice::ReadWrite);
				QPointF offset(pos() - evt->scenePos());
				int i = int(this);
				ds << i << offset;
				mimeData->setData("widget", ba);
				drag.setMimeData(mimeData);
				drag.exec(Qt::MoveAction);
			}
			_parent->notifyComponentActivated(this);
		} else if (evt->button() == Qt::MouseButton::RightButton) {
			_parent->resetConnectionMode();
		}
	}

	void WidgetEditComponent::selectConnection(bool viewOnly) {
		//	Display connections. These are actual connections this component has with other components
		QString choices[MAX_NUMBER_OF_CONNECTIONS];
		connections::Connection * connectionsAsArray[MAX_NUMBER_OF_CONNECTIONS];
		int count = 0;
		connections::Connection * connection = nullptr;
		for (std::vector<connections::Connection *>::iterator it = _connections.begin(); it != _connections.end(); ++it) {
			connection = *it;
			connectionsAsArray[count] = connection;
			if (!getName().isEmpty()) {
				choices[count] = getName();
			} else {
				choices[count] = getComponentType();
				if (!getComponentSubType().isEmpty()) {
					if (getComponentSubType() != CST_UNDEFINED) {
						choices[count] += " " + getComponentSubType();
					}
				}
			}
			choices[count] += " " + getRelationDescription(connection->getRelation(), connection->getRelationDirection(), connection->getComponentToBeConnectedWith()->getComponentType(), connection->getComponentToBeConnectedWith()->getComponentSubType()) + " ";
			if (!connection->getComponentToBeConnectedWith()->getName().isEmpty()) {
				choices[count] += connection->getComponentToBeConnectedWith()->getName();
			} else {
				choices[count] += connection->getComponentToBeConnectedWith()->getComponentType();
				if (!connection->getComponentToBeConnectedWith()->getComponentSubType().isEmpty()) {
					if (connection->getComponentToBeConnectedWith()->getComponentSubType() != CST_UNDEFINED) {
						choices[count] += " " + connection->getComponentToBeConnectedWith()->getComponentSubType();
					}
				}
			}
			count++;
		}

		if (viewOnly) { // TODO: (Michael) check the GetParent() calls here
			/*GetParent()->GetParent()->Enable(false); // Disables input from the parent
			wxSingleChoiceDialog choiceWindow(this, _("overview of existing connections"), _("Existing connections"), count, choices, reinterpret_cast<void **>(0), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxOK | wxCENTRE);
			choiceWindow.SetSize(320, 200);
			choiceWindow.ShowModal();
			GetParent()->GetParent()->Enable(true); // Enable input from the parent
			GetParent()->GetParent()->SetFocus();*/
		} else {
			if (count > 0) {
				DialogChooseConnectionType dcct;
				dcct.label->setText("Select a connection to be deleted");
				for (int i = 0; i < count; i++) {
					dcct.comboBox->addItem(choices[i]);
				}
				int res = dcct.exec();
				if (res == QDialog::DialogCode::Accepted) {
					deleteConnection(connectionsAsArray[dcct.comboBox->currentIndex()]);
				}
			}
		}
	}

	connections::ConnectionPolicy * WidgetEditComponent::selectPolicy(WidgetEditComponent * componentToBeConnectedWith) {
		QString choices[MAX_NUMBER_OF_CONNECTIONS];
		connections::ConnectionPolicy * policiesAsArray[MAX_NUMBER_OF_CONNECTIONS];
		int count = 0;
		connections::ConnectionPolicy * policy = nullptr;
		for (std::vector<connections::ConnectionPolicy *>::iterator it = _policies.begin(); it != _policies.end(); ++it) {
			policy = *it;

			/*	Display only available connections. These are connections where the policy of this component allows it to connect
			and if the other component (componentToBeConnectedWith) can also make the same connection.
			**/
			if (componentToBeConnectedWith->isConnectionPossible(policy->getRelation(), getOppositeRelationDirection(policy->getRelationDirection()), _type, _subType) && policy->isConnectionPossible(componentToBeConnectedWith->getComponentType(), componentToBeConnectedWith->getComponentSubType())) {
				policiesAsArray[count] = policy;
				choices[count] = getComponentType() + " ";
				if (!getName().isEmpty()) {
					choices[count] = getName();
				} else {
					choices[count] = getComponentType();
					if (!getComponentSubType().isEmpty()) {
						if (getComponentSubType() != CST_UNDEFINED) {
							choices[count] += " " + getComponentSubType();
						}
					}
				}
				choices[count] += " " + policy->getRelationDescription() + " ";
				if (!componentToBeConnectedWith->getName().isEmpty()) {
					choices[count] += componentToBeConnectedWith->getName();
				} else {
					choices[count] += componentToBeConnectedWith->getComponentType();
					if (!componentToBeConnectedWith->getComponentSubType().isEmpty()) {
						if (componentToBeConnectedWith->getComponentSubType() != CST_UNDEFINED) {
							choices[count] += " " + componentToBeConnectedWith->getComponentSubType();
						}
					}
				}
				count++;
			}
		}

		if (count > 1) {// TODO: (Michael) check the GetParent() calls here
			DialogChooseConnectionType dcct;
			dcct.label->setText("Select a connection type");
			for (int i = 0; i < count; i++) {
				dcct.comboBox->addItem(choices[i]);
			}
			int res = dcct.exec();
			if (res == QDialog::DialogCode::Rejected) {
				_selectedPolicy = nullptr;
			} else {
				_selectedPolicy = policiesAsArray[dcct.comboBox->currentIndex()];
			}
		} else if (count == 1) {
			_selectedPolicy = policiesAsArray[0];
		} else {
			_selectedPolicy = nullptr;
		}

		return _selectedPolicy;
	}

	bool WidgetEditComponent::isConnectionPossible() const {
		for (std::vector<connections::ConnectionPolicy *>::const_iterator it = _policies.begin(); it != _policies.end(); ++it) {
			if (!(*it)->isPolicyLocked()) {
				return true;
			}
		}
		return false;
	}

	bool WidgetEditComponent::isConnectionPossible(ComponentRelation relation, ComponentRelationDirection relationDirection, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith) const {
		for (std::vector<connections::ConnectionPolicy *>::const_iterator it = _policies.begin(); it != _policies.end(); ++it) {
			if ((*it)->isConnectionPossible(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith)) {
				return true;
			}
		}
		return false;
	}

	const QString & WidgetEditComponent::getRelationDescription(ComponentRelation relation, ComponentRelationDirection relationDirection, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith) const {
		for (std::vector<connections::ConnectionPolicy *>::const_iterator it = _policies.begin(); it != _policies.end(); ++it) {
			QString description = (*it)->getRelationDescription(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith);
			if (description != CRD_UNKNOWN) {
				return (*it)->getRelationDescription(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith);
			}
		}
		return CRD_UNKNOWN;
	}

	void WidgetEditComponent::deleteConnection(connections::Connection * connection) {
		if (!connection) {
			return;
		}

		for (std::vector<connections::Connection *>::iterator it = _connections.begin(); it != _connections.end(); ++it) {
			connections::Connection * connectionFromList = *it;
			if (connection == connectionFromList) {
				connection->getComponentToBeConnectedWith()->deleteConnection(this, connection->getRelation(), getOppositeRelationDirection(connection->getRelationDirection()));
				unlockPolicy(connection->getRelation(), connection->getRelationDirection(), connection->getComponentToBeConnectedWith()->getComponentType(), connection->getComponentToBeConnectedWith()->getComponentSubType());
				delete *it;
				_connections.erase(it);
				return;
			}
		}
	}

	void WidgetEditComponent::deleteConnection(WidgetEditComponent * componentConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection) {
		for (std::vector<connections::Connection *>::iterator it = _connections.begin(); it != _connections.end(); ++it) {
			if (componentConnectedWith == (*it)->getComponentToBeConnectedWith() && relation == (*it)->getRelation() && relationDirection == (*it)->getRelationDirection()) {
				_parent->notifyConnectionRemoved(this, componentConnectedWith, relation, relationDirection);
				unlockPolicy(relation, relationDirection, componentConnectedWith->getComponentType(), componentConnectedWith->getComponentSubType());
				delete *it;
				_connections.erase(it);
				return;
			}
		}
	}

	void WidgetEditComponent::unlockPolicy(ComponentRelation relation, ComponentRelationDirection relationDirection, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith) {
		for (std::vector<connections::ConnectionPolicy *>::iterator it = _policies.begin(); it != _policies.end(); ++it) {
			if (isRelationUnique(relation, relationDirection)) {
				(*it)->validateAndLock(relation, relationDirection, false);
			} else {
				(*it)->validateAndLock(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith, false);
			}
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
