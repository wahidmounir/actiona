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

#include "udp.h"
#include "code/rawdata.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Udp::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Udp *udp = new Udp;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "onConnected")
				udp->mOnConnected = it.value();
			else if(it.name() == "onDisconnected")
				udp->mOnDisconnected = it.value();
			else if(it.name() == "onReadyRead")
				udp->mOnReadyRead = it.value();
		}

		return udp->mThisObject = engine->newQObject(udp, QScriptEngine::ScriptOwnership);
	}
	
	Udp::Udp()
		: QObject(),
		QScriptable()
	{
		QObject::connect(&mUdpSocket, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(&mUdpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		QObject::connect(&mUdpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	}
	
	Udp::~Udp()
	{
		mUdpSocket.disconnectFromHost();
	}
	
	QScriptValue Udp::connect(const QString &hostname, quint16 port, OpenMode openMode)
	{
		mUdpSocket.connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
		
		return context()->thisObject();
	}
	
	QScriptValue Udp::waitForConnected(int waitTime)
	{
		if(!mUdpSocket.waitForConnected(waitTime))
			context()->throwError(tr("Cannot establish a connection to the host"));
		
		return context()->thisObject();
	}
	
	QScriptValue Udp::waitForReadyRead(int waitTime)
	{
		if(!mUdpSocket.waitForReadyRead(waitTime))
			context()->throwError(tr("Waiting for ready read failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue Udp::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *rawData = qobject_cast<RawData*>(object))
		{
			if(mUdpSocket.write(rawData->byteArray()) == -1)
				context()->throwError(tr("Write failed"));
		}
		else
		{
			if(mUdpSocket.write(data.toVariant().toByteArray()) == -1)
				context()->throwError(tr("Write failed"));
		}
	
		return context()->thisObject();
	}
	
	QScriptValue Udp::writeText(const QString &data, Code::Encoding encoding)
	{
		if(mUdpSocket.write(Code::toEncoding(data, encoding)) == -1)
			context()->throwError(tr("Write failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue Udp::read()
	{
		return RawData::constructor(mUdpSocket.readAll(), context(), engine());
	}
	
	QString Udp::readText(Code::Encoding encoding)
	{
		return Code::fromEncoding(mUdpSocket.readAll(), encoding);
	}
	
	QScriptValue Udp::disconnect()
	{
		mUdpSocket.disconnectFromHost();
		
		return context()->thisObject();
	}
	
	void Udp::connected()
	{
		if(mOnConnected.isValid())
			mOnConnected.call(mThisObject);
	}

	void Udp::disconnected()
	{
		if(mOnDisconnected.isValid())
			mOnDisconnected.call(mThisObject);
	}

	void Udp::readyRead()
	{
		if(mOnReadyRead.isValid())
			mOnReadyRead.call(mThisObject);
	}
}