/****************************************************************************
**
** Copyright (C) 2014 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dynamicclient.h"
 #include <QMetaMethod>

// constructor
DynamicClient::DynamicClient(QSharedPointer<QRemoteObjectDynamicReplica> ptr) :
    QObject(nullptr), reptr(ptr)
{
    //connect signal for replica valid changed with signal slot initialization
    QObject::connect(reptr.data(), SIGNAL(initialized()), this, SLOT(initConnection_slot()));
}

//destructor
DynamicClient::~DynamicClient()
{

}

// Function to initialize connections between slots and signals
void DynamicClient::initConnection_slot()
{

    // connect source replica signal currStateChanged() with client's recSwitchState() slot to receive source's current state
   QObject::connect(reptr.data(), SIGNAL(currStateChanged(bool)), this, SLOT(recSwitchState_slot(bool)));
   // connect client's echoSwitchState(..) signal with replica's server_slot(..) to echo back received state
   QObject::connect(this, SIGNAL(echoSwitchState(bool)),reptr.data(), SLOT(server_slot(bool)));
}


void DynamicClient::recSwitchState_slot(bool value)
{
   clientSwitchState = reptr->property("currState").toBool(); // use replica property to get "currState" from source
   qDebug() << "Received source state " << value << clientSwitchState;
   Q_EMIT echoSwitchState(clientSwitchState); // Emit signal to echo received state back to server
}

