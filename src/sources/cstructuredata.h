#ifndef CSTRUCTUREDATA_H
#define CSTRUCTUREDATA_H

#include<QList>
#include<QString>
#include<QThread>
#include <QList>
#include <QTreeWidgetItem>

//for structure
class CLeaf{
public:
    QString level;
    QString title;
    int position;
    CLeaf* parent;
    QList<CLeaf*> children;

    CLeaf(){
        position = -1;
        parent = NULL;
        children.clear();

    }

    void AddChild(CLeaf* chd){
        chd->parent = this;
        children.append(chd);
    }

};


class CStructureData
{
    QList<CLeaf*> AllLeafs;


    CLeaf* guessParent(QString level, int position);

    bool isHigherLevel(QString lv1, QString lv0);

public:
    CStructureData();

    void Clear();

    void addLeaf(CLeaf* leaf, CLeaf* parent=NULL){
        if(parent != NULL){
            leaf->parent = parent;
            parent->children.append(leaf);
        }
        AllLeafs.append(leaf);
    }

    void constructTree();

    QTreeWidgetItem * createWidgetItem(QList<QTreeWidgetItem *> *itemlist, CLeaf *aleaf);

    void BuildQTree(QList<QTreeWidgetItem*>* itemlist);

    void getLeaf(QString& cnt, QString tag);

    void Parse(QString &cnt);


};


class StructureThread:public QThread
{
    Q_OBJECT

    QString content;
    CStructureData data;
    QList<QTreeWidgetItem*>* itemlist;

public :
    explicit StructureThread(QObject *parent = 0);

    StructureThread(QString& cnt, QList<QTreeWidgetItem*>* itmlst);

    void setString(QString& cnt);

    void setItemList(QList<QTreeWidgetItem*>* itmlst);

    void run();

signals:
    void nowShowTheTree();

protected:

};


#endif // CSTRUCTUREDATA_H
