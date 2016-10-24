/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "widgets/WidgetEditComponent.h"

#include "connections/Connection.h"
#include "connections/LineConnector.h"

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

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseBehaviour.h"
#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseExtern.h"
#include "ParticleUniverseObserver.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

#include <QDataStream>
#include <QDrag>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>

namespace i6e {
namespace particleEditor {
namespace widgets {

	const int MAX_NUMBER_OF_CONNECTIONS = 20;
	const QString styleString = "background: %1; border: 2px solid #%2";
	const QString borderColorString = "4f4f51";
	const QString selectedBorderColorString = "0000af";

	WidgetEditComponent::WidgetEditComponent(WidgetEdit * parent, QGraphicsScene * scene, QString name, QString type, QString subType) : QGraphicsWidget(), _parent(parent), _label(nullptr), _name(name), _type(type), _subType(subType), _element(nullptr), _policies(), _uniqueRelations(), _connections(), _lineConnections(), _selectedPolicy(nullptr), _propertyWindow(nullptr), _oldPropertyWindow(nullptr), _connection(nullptr), _backgroundColor(), _widget(nullptr) {
		QString labelText = type + " (" + subType + ")";
		_widget = new QWidget();
		QHBoxLayout * hLayout = new QHBoxLayout(_widget);
		_widget->setLayout(hLayout);
		_label = new QLabel(_widget);
		hLayout->addWidget(_label);
		QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(this);
		setLayout(layout);
		layout->addItem(scene->addWidget(_widget));
		if (!name.isEmpty()) {
			labelText += " - " + name;
		}
		_label->setText(labelText);

		if (type == CT_SYSTEM) {
			_backgroundColor = QColor(0, 0, 0);
			_label->setStyleSheet("color: white");
		} else if (type == CT_TECHNIQUE) {
			_backgroundColor = QColor(102, 152, 255);
		} else if (type == CT_RENDERER) {
			_backgroundColor = QColor(228, 34, 23);
		} else if (type == CT_EMITTER) {
			_backgroundColor = QColor(76, 196, 23);
		} else if (type == CT_OBSERVER) {
			_backgroundColor = QColor(37, 65, 23);
		} else if (type == CT_AFFECTOR) {
			_backgroundColor = QColor(251, 177, 23);
		} else if (type == CT_HANDLER) {
			_backgroundColor = QColor(141, 56, 201);
		} else if (type == CT_BEHAVIOUR) {
			_backgroundColor = QColor(48, 125, 126);
		} else if (type == CT_EXTERN) {
			_backgroundColor = QColor(130, 123, 96);
		}
		_widget->setStyleSheet(styleString.arg(QVariant(_backgroundColor).toString()).arg(borderColorString));

		resize(200, 80);

		_propertyWindow = createPropertyWindow(_subType);
	}

	WidgetEditComponent::~WidgetEditComponent() {
		delete _propertyWindow;
		for (connections::ConnectionPolicy * cp : _policies) {
			delete cp;
		}
		for (connections::UniqueRelation * ur : _uniqueRelations) {
			delete ur;
		}
		for (connections::Connection * c : _connections) {
			delete c;
		}
	}

	void WidgetEditComponent::addPolicy(ComponentRelation relation, ComponentRelationDirection relationDirection, const QString & relationDescription, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith, bool multipleConnectionsPossible, bool ignoreSubType, const QColor & colour, Qt::PenStyle lineStyle) {
		_policies.push_back(new connections::ConnectionPolicy(relation, relationDirection, relationDescription, typeToBeConnectedWith, subTypeToBeConnectedWith, multipleConnectionsPossible, ignoreSubType, colour, lineStyle));
	}

	void WidgetEditComponent::addUniqueRelation(ComponentRelation relation, ComponentRelationDirection relationDirection) {
		_uniqueRelations.push_back(new connections::UniqueRelation(relation, relationDirection));
	}

	void WidgetEditComponent::addConnection(WidgetEditComponent * componentToBeConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection, connections::LineConnector * connection) {
		_connection = connection;

		_connections.push_back(new connections::Connection(componentToBeConnectedWith, relation, relationDirection));
		_lineConnections.push_back(connection);

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
			SystemPropertyWindow * pWin = new SystemPropertyWindow(_parent, this, _name);
			_propertyWindow = pWin;
		} else if (_type == CT_TECHNIQUE) {
			_propertyWindow = new TechniquePropertyWindow(_parent, this, _name);
		} else if (_type == CT_RENDERER) {
			if (propertyWindow) {
				_propertyWindow = factories::RendererPropertyWindowFactory::createRendererPropertyWindow(_subType, this, static_cast<RendererPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::RendererPropertyWindowFactory::createRendererPropertyWindow(_parent, this, _name, _subType);
			}
		} else if (_type == CT_EMITTER) {
			if (propertyWindow) {
				_propertyWindow = factories::EmitterPropertyWindowFactory::createEmitterPropertyWindow(_subType, this, static_cast<EmitterPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::EmitterPropertyWindowFactory::createEmitterPropertyWindow(_parent, this, _name, _subType);
			}
		} else if (_type == CT_AFFECTOR) {
			if (propertyWindow) {
				_propertyWindow = factories::AffectorPropertyWindowFactory::createAffectorPropertyWindow(_subType, this, static_cast<AffectorPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::AffectorPropertyWindowFactory::createAffectorPropertyWindow(_parent, this, _name, _subType);
			}
		} else if (_type == CT_OBSERVER) {
			if (propertyWindow) {
				_propertyWindow = factories::ObserverPropertyWindowFactory::createObserverPropertyWindow(_subType, this, static_cast<ObserverPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::ObserverPropertyWindowFactory::createObserverPropertyWindow(_parent, this, _name, _subType);
			}
		} else if (_type == CT_HANDLER) {
			if (propertyWindow) {
				_propertyWindow = factories::EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(_subType, this, static_cast<EventHandlerPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(_parent, this, _name, _subType);
			}
		} else if (_type == CT_BEHAVIOUR) {
			if (propertyWindow) {
				_propertyWindow = factories::BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(_subType, this, static_cast<BehaviourPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(_parent, this, _name, _subType);
			}
		} else if (_type == CT_EXTERN) {
			if (propertyWindow) {
				_propertyWindow = factories::ExternPropertyWindowFactory::createExternPropertyWindow(_subType, this, static_cast<ExternPropertyWindow *>(_propertyWindow));
			} else {
				_propertyWindow = factories::ExternPropertyWindowFactory::createExternPropertyWindow(_parent, this, _name, _subType);
			}
		}
		connect(_propertyWindow, SIGNAL(replacePropertyWindow(QString)), this, SLOT(replacePropertyWindow(QString)), Qt::QueuedConnection);
		connect(_propertyWindow, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(_propertyWindow, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()));
		return _propertyWindow;
	}

	void WidgetEditComponent::deselect() {
		_widget->setStyleSheet(styleString.arg(QVariant(_backgroundColor).toString()).arg(borderColorString));
	}

	void WidgetEditComponent::replacePropertyWindow(QString subType) {
		PropertyWindow * tmp = _propertyWindow;
		_propertyWindow = nullptr;
		_parent->notifyComponentActivated(this);
		_subType = subType;
		setCaption();
		delete tmp;
		createPropertyWindow(subType);
		if (_type == CT_SYSTEM) {
			dynamic_cast<SystemPropertyWindow *>(_propertyWindow)->copyAttributesFromSystem(dynamic_cast<ParticleUniverse::ParticleSystem *>(_element));
		} else if (_type == CT_TECHNIQUE) {
			dynamic_cast<TechniquePropertyWindow *>(_propertyWindow)->copyAttributesFromTechnique(dynamic_cast<ParticleUniverse::ParticleTechnique *>(_element));
		} else if (_type == CT_RENDERER) {
			dynamic_cast<RendererPropertyWindow *>(_propertyWindow)->copyAttributesFromRenderer(dynamic_cast<ParticleUniverse::ParticleRenderer *>(_element));
		} else if (_type == CT_EMITTER) {
			dynamic_cast<EmitterPropertyWindow *>(_propertyWindow)->copyAttributesFromEmitter(dynamic_cast<ParticleUniverse::ParticleEmitter *>(_element));
		} else if (_type == CT_AFFECTOR) {
			dynamic_cast<AffectorPropertyWindow *>(_propertyWindow)->copyAttributesFromAffector(dynamic_cast<ParticleUniverse::ParticleAffector *>(_element));
		} else if (_type == CT_OBSERVER) {
			dynamic_cast<ObserverPropertyWindow *>(_propertyWindow)->copyAttributesFromObserver(dynamic_cast<ParticleUniverse::ParticleObserver *>(_element));
		} else if (_type == CT_HANDLER) {
			dynamic_cast<EventHandlerPropertyWindow *>(_propertyWindow)->copyAttributesFromEventHandler(dynamic_cast<ParticleUniverse::ParticleEventHandler *>(_element));
		} else if (_type == CT_BEHAVIOUR) {
			dynamic_cast<BehaviourPropertyWindow *>(_propertyWindow)->copyAttributesFromBehaviour(dynamic_cast<ParticleUniverse::ParticleBehaviour *>(_element));
		} else if (_type == CT_EXTERN) {
			dynamic_cast<ExternPropertyWindow *>(_propertyWindow)->copyAttributesFromExtern(dynamic_cast<ParticleUniverse::Extern *>(_element));
		}
		_parent->notifyComponentActivated(this);
	}

	void WidgetEditComponent::triggerRenameParticleSystem(QString oldName, QString newName) {
		emit renameParticleSystem(oldName, newName);
	}

	void WidgetEditComponent::triggerNotifyChanged() {
		emit notifyChanged();
	}

	void WidgetEditComponent::mousePressEvent(QGraphicsSceneMouseEvent * evt) {
		if (evt->button() == Qt::MouseButton::LeftButton) {
			_widget->setStyleSheet(styleString.arg(QVariant(_backgroundColor).toString()).arg(selectedBorderColorString));
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
				quint64 i = quint64(this);
				ds << i << offset;
				mimeData->setData("widget", ba);
				drag.setMimeData(mimeData);
				drag.exec(Qt::MoveAction);
			}
			_parent->notifyComponentActivated(this);
		} else if (evt->button() == Qt::MouseButton::RightButton) {
			_widget->setStyleSheet(styleString.arg(QVariant(_backgroundColor).toString()).arg(borderColorString));
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

		if (viewOnly) {
			/*GetParent()->GetParent()->Enable(false); // Disables input from the parent
			wxSingleChoiceDialog choiceWindow(this, _("overview of existing connections"), _("Existing connections"), count, choices, reinterpret_cast<void **>(0), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxOK | wxCENTRE);
			choiceWindow.SetSize(320, 200);
			choiceWindow.ShowModal();
			GetParent()->GetParent()->Enable(true); // Enable input from the parent
			GetParent()->GetParent()->SetFocus();*/
		} else {
			if (count > 0) {
				DialogChooseConnectionType dcct;
				dcct.label->setText(QApplication::tr("Select a connection to be deleted"));
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
			dcct.label->setText(QApplication::tr("Select a connection type"));
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

		std::remove_if(_lineConnections.begin(), _lineConnections.end(), [this](connections::LineConnector * lc) {
			return lc->getFrom() == this || lc->getTo() == this;
		});

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
} /* namespace i6e */
