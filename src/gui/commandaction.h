/*
    Copyright (c) 2017, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMANDACTION_H
#define COMMANDACTION_H

#include "common/command.h"

#include <QAction>

class ClipboardBrowser;

class CommandAction : public QAction
{
    Q_OBJECT
public:
    enum Type { ClipboardCommand, ItemCommand };

    CommandAction(
            const Command &command,
            const QString &name,
            Type type,
            ClipboardBrowser *browser,
            QObject *parent = nullptr);

    const Command &command() const;

signals:
    void triggerCommand(const Command &command, const QVariantMap &data, int type);

protected:
    bool event(QEvent *event) override;

private slots:
    void onTriggered();

private:
    Command m_command;
    Type m_type;
    ClipboardBrowser *m_browser;
    QString m_triggeredShortcut;
};

#endif // COMMANDACTION_H
