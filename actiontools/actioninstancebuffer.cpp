/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "actioninstancebuffer.h"

namespace ActionTools
{
	QDataStream &operator << (QDataStream &s, const ActionInstanceBuffer &actionInstanceBuffer)
	{
		s << actionInstanceBuffer.actionInstanceId();
		s << actionInstanceBuffer.action();

		return s;
	}

	QDataStream &operator >> (QDataStream &s, ActionInstanceBuffer &actionInstanceBuffer)
	{
		QString actionId;
		ActionInstance action;

		s >> actionId;
		s >> action;

		actionInstanceBuffer.setActionInstanceId(actionId);
		actionInstanceBuffer.setAction(action);

		return s;
	}
}