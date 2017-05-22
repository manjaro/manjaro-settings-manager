/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Sanchit Samuel <sanchit.samuel@live.com>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JOB_H
#define JOB_H

#include <QThread>
#include <KAuth/KAuthAction>

class Job : public QThread
{
    Q_OBJECT
    
private:
    KAuth::Action m_installAction;
    QString m_lastMessage;
    
public:
    explicit Job(QObject *parent = 0, bool b = false);
    void run();

    // if Stop = true, the thread will break
    // out of the loop, and will be disposed
    bool Stop;
    void setAction (KAuth::Action action);

signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void valueChanged(QString);
    void jobCompeted(bool);    
};

#endif // JOB_H
