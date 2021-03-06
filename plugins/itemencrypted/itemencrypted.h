/*
    Copyright (c) 2014, Lukas Holecek <hluk@email.cz>

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

#ifndef ITEMENCRYPTED_H
#define ITEMENCRYPTED_H

#include "item/itemwidget.h"
#include "gui/icons.h"

#include <QProcess>
#include <QWidget>

#include <memory>

namespace Ui {
class ItemEncryptedSettings;
}

class QIODevice;

class ItemEncrypted : public QWidget, public ItemWidget
{
    Q_OBJECT

public:
    ItemEncrypted(QWidget *parent);

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const override;

    void setTagged(bool tagged) override;
};

class ItemEncryptedSaver : public QObject, public ItemSaverInterface
{
    Q_OBJECT

public:
    bool saveItems(const QAbstractItemModel &model, QIODevice *file) override;

signals:
    void error(const QString &);

private:
    void emitEncryptFailed();
};

class ItemEncryptedScriptable : public ItemScriptable
{
    Q_OBJECT
public:
    explicit ItemEncryptedScriptable(QObject *parent) : ItemScriptable(parent) {}

public slots:
    bool isEncrypted();
    QByteArray encrypt();
    QByteArray decrypt();

    QString generateTestKeys();
    bool isGpgInstalled();
};

class ItemEncryptedLoader : public QObject, public ItemLoaderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID COPYQ_PLUGIN_ITEM_LOADER_ID)
    Q_INTERFACES(ItemLoaderInterface)

public:
    ItemEncryptedLoader();

    ~ItemEncryptedLoader();

    ItemWidget *create(const QModelIndex &index, QWidget *parent, bool) const override;

    QString id() const override { return "itemencrypted"; }
    QString name() const override { return tr("Encryption"); }
    QString author() const override { return QString(); }
    QString description() const override { return tr("Encrypt items and tabs."); }
    QVariant icon() const override { return QVariant(IconLock); }

    QStringList formatsToSave() const override;

    QVariantMap applySettings() override;

    void loadSettings(const QVariantMap &settings) override { m_settings = settings; }

    QWidget *createSettingsWidget(QWidget *parent) override;

    bool canLoadItems(QIODevice *file) const override;

    bool canSaveItems(const QAbstractItemModel &model) const override;

    ItemSaverPtr loadItems(QAbstractItemModel *model, QIODevice *file) override;

    ItemSaverPtr initializeTab(QAbstractItemModel *model) override;

    QObject *tests(const TestInterfacePtr &test) const override;

    const QObject *signaler() const override { return this; }

    ItemScriptable *scriptableObject(QObject *parent) override;

    QList<Command> commands() const override;

signals:
    void error(const QString &);
    void addCommands(const QList<Command> &commands);

private slots:
    void setPassword();
    void terminateGpgProcess();
    void onGpgProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void addCommands();

private:
    enum GpgProcessStatus {
        GpgNotInstalled,
        GpgNotRunning,
        GpgGeneratingKeys,
        GpgChangingPassword
    };

    void updateUi();

    void emitDecryptFailed();

    ItemSaverPtr createSaver();

    std::unique_ptr<Ui::ItemEncryptedSettings> ui;
    QVariantMap m_settings;

    GpgProcessStatus m_gpgProcessStatus;
    QProcess *m_gpgProcess;
};

#endif // ITEMENCRYPTED_H
