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

#ifndef __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_CONNECTION_H__
#define __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_CONNECTION_H__

#include "widgets/WidgetEditComponent.h"

namespace i6e {
namespace particleEditor {
namespace connections {

	/**	ConnectionPolicy: Defines the possible relations between 2 component types. Example:
	 * A 'technique' component may include an 'emitter' component. A ConnectionPolicy for the 'emitter' component
	 * is defined by:
	 * emitter included by technique
	 * Also subtypes can be used:
	 * affector handled by handler:do affector
	 */
	class ConnectionPolicy {
	public:
		ConnectionPolicy(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, QString relationDescription, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith = widgets::CST_UNDEFINED, bool multipleConnectionsPossible = false, bool ignoreST = true, QColor colourCode = QColor(0, 0, 0), Qt::PenStyle lineStyle = Qt::SolidLine) : _relation(relation), _relationDirection(relationDirection), _relationDescription(relationDescription), _typeToBeConnectedWith(typeToBeConnectedWith), _subTypeToBeConnectedWith(subTypeToBeConnectedWith), _multipleConnectionsPossible(multipleConnectionsPossible), _policyLocked(false), _ignoreSubType(ignoreST), _colour(colourCode), _lineStyle(lineStyle) {
		}

		~ConnectionPolicy() {
		}

		// Getters
		widgets::ComponentRelation getRelation() const;
		widgets::ComponentRelationDirection getRelationDirection() const;
		const QString & getRelationDescription(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, const QString & typeToBeConnectedWith, const QString & subTypeToBeConnectedWith) const;
		const QString & getRelationDescription() const;
		QString getTypeToBeConnectedWith() const;
		QString getSubTypeToBeConnectedWith() const;
		const QColor & getColour() const;
		Qt::PenStyle getLineStyle() const;
		bool isPolicyLocked() const;
		bool areMultipleConnectionsPossible() const;
		bool ignoreSubType() const;

		/**	Determines if a connection with this type of component possible
		*/
		bool isConnectionPossible(QString typeToBeConnectedWith, QString subTypeToBeConnectedWith = widgets::CST_UNDEFINED);

		/**	This function not only determines whether a connection is possible, but also if a specific connection is possible
		*/
		bool isConnectionPossible(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, QString typeToBeConnectedWith, QString subTypeToBeConnectedWith = widgets::CST_UNDEFINED);

		/**	This function determines the impact on the policy and locks or unlocks if necessary
		*/
		void validateAndLock(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, QString componentType, QString componentSubType, bool lock = true);

		/**	This function locks or unlocks a policy if it has a specific relation
		*/
		void validateAndLock(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection, bool lock = true);

	protected:
		widgets::ComponentRelation _relation;
		widgets::ComponentRelationDirection _relationDirection;
		QString _relationDescription;
		QString _typeToBeConnectedWith;
		QString _subTypeToBeConnectedWith;
		bool _multipleConnectionsPossible;
		bool _policyLocked;
		bool _ignoreSubType;
		QColor _colour;
		Qt::PenStyle _lineStyle;
	};

	/**	Connection: Defines the actual relation between the component and another component.
	*/
	class Connection {
	public:
		Connection(widgets::WidgetEditComponent * componentToBeConnectedWith, widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection) : _componentToBeConnectedWith(componentToBeConnectedWith), _relation(relation), _relationDirection(relationDirection) {
		}
		~Connection() {
		}

		// Getter
		widgets::WidgetEditComponent * getComponentToBeConnectedWith();
		widgets::ComponentRelation getRelation() const;
		widgets::ComponentRelationDirection getRelationDirection() const;

	protected:
		widgets::WidgetEditComponent * _componentToBeConnectedWith;
		widgets::ComponentRelation _relation;
		widgets::ComponentRelationDirection _relationDirection;
	};

	/**	For storage of unique combinations
	*/
	class UniqueRelation {
	public:
		UniqueRelation(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection) : _relation(relation), _relationDirection(relationDirection) {
		}
		~UniqueRelation() {
		}

		bool isRelationUnique(widgets::ComponentRelation relation, widgets::ComponentRelationDirection relationDirection);

	protected:
		widgets::ComponentRelation _relation;
		widgets::ComponentRelationDirection _relationDirection;
	};

} /* namespace connections */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_CONNECTION_H__ */
