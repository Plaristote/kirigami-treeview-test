#ifndef ITEMMODELTEST_H
#define ITEMMODELTEST_H

#include <QAbstractItemModel>
#include <QQmlEngine>

class MenuItem : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
    Q_PROPERTY(QString description MEMBER m_description NOTIFY descriptionChanged)
    Q_PROPERTY(QUrl iconUrl READ iconUrl NOTIFY iconUrlChanged)
public:
    enum Role
    {
        DisplayNameRole = Qt::UserRole + 1,
        DescriptionRole,
        IconUrlRole,
        UnreadCountRole,
        MenuItemRole
    };

    MenuItem(QObject *parent = nullptr);
    MenuItem(MenuItem &parent);
    virtual ~MenuItem();

    static MenuItem* fromIndex(const QModelIndex& item);
    MenuItem* parentItem() const { return m_parentItem; }
    void setParentItem(MenuItem* parent);

    QVariant data(const QModelIndex& index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = {}) const override;
    QModelIndex parent(const QModelIndex &item) const override;
    int rowCount(const QModelIndex &item) const override;
    int columnCount(const QModelIndex &) const override;
    QHash<int, QByteArray> roleNames() const override;
    int row() const;

    virtual QUrl iconUrl() const { return QUrl(); }
    virtual int childrenCount() const { return 0; }
    virtual int unreadCount() const { return 0; }
    virtual MenuItem* childAt(int) const { return nullptr; }
    virtual int indexOf(const MenuItem *) const { return -1; }

Q_SIGNALS:
    void nameChanged();
    void descriptionChanged();
    void iconUrlChanged();
    void parentItemChanged();

private:
    QString m_name;
    QString m_description;
    MenuItem* m_parentItem = nullptr;
};

class Feed : public MenuItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl xmlPath MEMBER m_xmlPath NOTIFY xmlPathChanged)
public:
    Feed(QObject *parent = nullptr);
    Feed(MenuItem &parent);

    QUrl iconUrl() const override { return m_faviconUrl; }

Q_SIGNALS:
    void xmlPathChanged();

private:
    QUrl m_xmlPath;
    QUrl m_faviconUrl;
};

class FeedFolder : public MenuItem
{
    Q_OBJECT
public:
    FeedFolder(QObject *parent = nullptr);
    FeedFolder(MenuItem &parent);

    QUrl iconUrl() const override { return QUrl(QStringLiteral("qrc:/folder.png")); }
    int childrenCount() const override { return m_children.size(); }
    MenuItem* childAt(int index) const override { return m_children.at(index); }
    int indexOf(const MenuItem *item) const override { return m_children.indexOf(const_cast<MenuItem*>(item)); }

    QModelIndex selfIndex() const;
    void addMenuItem(MenuItem* item);

private:
    QList<MenuItem*> m_children;
};

#endif // ITEMMODELTEST_H
