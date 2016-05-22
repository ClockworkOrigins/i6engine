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

#include "connections/Connection.h"

namespace i6e {
namespace particleEditor {
namespace connections {

	widgets::ComponentRelation ConnectionPolicy::getRelation() const {
		return _relation;
	}

	widgets::ComponentRelationDirection ConnectionPolicy::getRelationDirection() const {
		return _relationDirection;
	}

	const QString & ConnectionPolicy::getRelationDescription(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, const QString & typeToBeConnectedWith, const QString & subTypeToBeConnectedWith) const {
		if (relation == _relation && relationDirection == _relationDirection && typeToBeConnectedWith == _typeToBeConnectedWith) {
			if (_ignoreSubType) {
				return _relationDescription;
			} else {
				if (subTypeToBeConnectedWith == _subTypeToBeConnectedWith && subTypeToBeConnectedWith != widgets::CST_UNDEFINED) {
					return _relationDescription;
				}
			}
		}
		return widgets::CRD_UNKNOWN;
	}

	QString ConnectionPolicy::getTypeToBeConnectedWith() const {
		return _typeToBeConnectedWith;
	}

	QString ConnectionPolicy::getSubTypeToBeConnectedWith() const {
		return _subTypeToBeConnectedWith;
	}

	const QColor & ConnectionPolicy::getColour() const {
		return _colour;
	}

	Qt::PenStyle ConnectionPolicy::getLineStyle() const {
		return _lineStyle;
	}

	bool ConnectionPolicy::isPolicyLocked() const {
		return _policyLocked;
	}

	bool ConnectionPolicy::areMultipleConnectionsPossible() const {
		return _multipleConnectionsPossible;
	}

	bool ConnectionPolicy::ignoreSubType() const {
		return _ignoreSubType;
	}

	const QString & ConnectionPolicy::getRelationDescription() const {
		return _relationDescription;
	}

	void ConnectionPolicy::validateAndLock(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, QString componentType, QString componentSubType, bool lock) {
		if (_multipleConnectionsPossible) {
			return;
		}

		if (componentType == _typeToBeConnectedWith && relation == _relation  && relationDirection == _relationDirection) {
			if (_ignoreSubType) {
				_policyLocked = lock;
			} else {
				if (componentSubType == _subTypeToBeConnectedWith && componentSubType != widgets::CST_UNDEFINED) {
					_policyLocked = lock;
				}
			}
		}
	}

	void ConnectionPolicy::validateAndLock(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, bool lock) {
		if (relation == _relation  && relationDirection == _relationDirection) {
			_policyLocked = lock;
		}
	}

	bool ConnectionPolicy::isConnectionPossible(QString typeToBeConnectedWith, QString subTypeToBeConnectedWith) {
		if (_ignoreSubType) {
			return (typeToBeConnectedWith == _typeToBeConnectedWith && !_policyLocked);
		} else {
			if (typeToBeConnectedWith == _typeToBeConnectedWith && !_policyLocked) {
				if (subTypeToBeConnectedWith == _subTypeToBeConnectedWith && subTypeToBeConnectedWith != widgets::CST_UNDEFINED) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	}

	bool ConnectionPolicy::isConnectionPossible(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith) {
		return (isConnectionPossible(typeToBeConnectedWith, subTypeToBeConnectedWith) && relation == _relation && relationDirection == _relationDirection);
	}

	widgets::ComponentRelation Connection::getRelation() const {
		return _relation;
	}

	widgets::ComponentRelationDirection Connection::getRelationDirection() const {
		return _relationDirection;
	}

	widgets::WidgetEditComponent * Connection::getComponentToBeConnectedWith() {
		return _componentToBeConnectedWith;
	}

	bool UniqueRelation::isRelationUnique(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection) {
		return (relation == _relation && relationDirection == _relationDirection);
	}

} /* namespace connections */
} /* namespace particleEditor */
} /* namespace i6e */
