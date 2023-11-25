#include "itemmodeltest.h"


MenuItem::MenuItem(QObject *parent) : QAbstractItemModel(parent)
{
}

MenuItem::MenuItem(MenuItem &parent) : QAbstractItemModel(&parent), m_parentItem(&parent)
{
}

MenuItem::~MenuItem()
{
}

MenuItem* MenuItem::fromIndex(const QModelIndex &item)
{
    if (item.internalPointer())
        return reinterpret_cast<MenuItem *>(item.internalPointer());
    return nullptr;
}

void MenuItem::setParentItem(MenuItem *parent)
{
    setParent(parent);
    m_parentItem = parent;
    Q_EMIT parentItemChanged();
}

QVariant MenuItem::data(const QModelIndex &index, int role) const
{
    MenuItem* item = fromIndex(index);

    if (item)
    {
        switch (static_cast<Role>(role))
        {
        case DisplayNameRole:
            return item->m_name;
        case DescriptionRole:
            return item->m_description;
        case IconUrlRole:
            return item->iconUrl();
        case UnreadCountRole:
            return item->unreadCount();
        case MenuItemRole:
            return QVariant::fromValue(reinterpret_cast<QObject*>(item));
        }
    }
    return {};
}

QModelIndex MenuItem::index(int row, int column, const QModelIndex &parentIndex) const
{
    const MenuItem* parent = fromIndex(parentIndex);

    if (parent)
        return parent->index(row, column);
    return createIndex(row, column, childAt(row));
}

QModelIndex MenuItem::parent(const QModelIndex &index) const
{
    const MenuItem* item = fromIndex(index);

    if (item)
    {
        MenuItem* parentItem = item->parentItem();

        if (parentItem)
            return createIndex(parentItem->row(), 0, parentItem);
    }
    return {};
}

int MenuItem::rowCount(const QModelIndex &index) const
{
    const MenuItem* item = fromIndex(index);

    return item ? item->childrenCount() : childrenCount();
}

int MenuItem::columnCount(const QModelIndex&) const
{
    return 1;
}

QHash<int, QByteArray> MenuItem::roleNames() const {
    return {
        {DisplayNameRole, "displayName"},
        {DescriptionRole, "description"},
        {IconUrlRole,     "iconUrl"},
        {UnreadCountRole, "unreadCount"},
        {MenuItemRole,    "menuItem"}
    };
}

int MenuItem::row() const
{
    if (parentItem())
        return parentItem()->indexOf(this);
    return 0;
}

// Feed

Feed::Feed(QObject *parent) : MenuItem(parent)
{
}

Feed::Feed(MenuItem &parent) : MenuItem(parent)
{
}

// FeedFolder

FeedFolder::FeedFolder(QObject *parent) : MenuItem(parent)
{
}

FeedFolder::FeedFolder(MenuItem &parent) : MenuItem(parent)
{
}

QModelIndex FeedFolder::selfIndex() const
{
    return createIndex(row(), 0, const_cast<FeedFolder *>(this));
}

void FeedFolder::addMenuItem(MenuItem* item)
{
    item->setParentItem(this);
    beginInsertRows(selfIndex(), m_children.size(), m_children.size() + 1);
    m_children.append(item);
    endInsertRows();
}
