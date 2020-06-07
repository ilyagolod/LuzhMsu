#ifndef RBTREE_H
#define RBTREE_H

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "record.h"

template <typename Element>
class RBTree
{
public:
    enum Color
    {
        BLACK,
        RED
    };

    class Node
    {
        friend class RBTree<Element>;

    public:
        enum WhichChild
        {
            LEFT,
            RIGHT,
            NONE
        };

    public:
        const Node *getLeft() const { return _left; }
        const Node *getRight() const { return _right; }
        const Node *getParent() const { return _parent; }
        Color getColor() const { return _color; }
        bool isBlack() const { return _color == BLACK; }
        bool isRed() const { return _color == RED; }
        bool isDaddyRed() const
        {
            if (!_parent)
                return false;
            return (_parent->_color == RED);
        }
        bool isLeftChild() const
        {
            if (!_parent)
                return false;
            return (_parent->_left == this);
        }
        bool isRightChild() const
        {
            if (!_parent)
                return false;
            return (_parent->_right == this);
        }
        WhichChild getWhichChild() const
        {
            if (!_parent)
                return NONE;
            if (_parent->_left == this)
                return LEFT;
            return RIGHT;
        }
        Element *getKey() const { return _key; }
        void printNode(int lvl)
        {
            if (_left)
                _left->printNode(lvl + 1);
            for (int i = 0; i < lvl; i++)
                std::cout << "   ";
            _key.print();
            if (_color == BLACK)
                std::cout << "black\n";
            else
                std::cout << "red\n";
            if (_right)
                _right->printNode(lvl + 1);
        }

    protected:
        Node(Element *key = Element(),
             Node *left = nullptr,
             Node *right = nullptr,
             Node *parent = nullptr,
             Color col = BLACK)
            : _key(*key), _left(left), _right(right), _parent(parent), _color(col)
        {
            if (_left)
                _left->_parent = this;
            if (_right)
                _right->_parent = this;
        }
        ~Node(); 
    protected:
        Node(const Node *); 
        Node *operator=(Node *);

    protected:
        Node *setLeft(Node *lf);
        Node *setRight(Node *rg);
        void setBlack() { _color = BLACK; }
        void setRed() { _color = RED; }
        Node *getDaddy(bool *isLeftChild)
        {
            if (!_parent)
                return nullptr;
            isLeftChild = (_parent->_left == this);
            return _parent;
        }
        Node *getChild(bool isLeft)
        {
            return isLeft ? _left : _right;
        }
        bool isSpecificChildPrv(bool isLeft) const
        {
            if (isLeft) 
                return (_parent->_left == this);
            return (_parent->_right == this);
        }

    protected:
        Element _key;  
        Color _color;  
        Node *_parent; 
        Node *_left;   
        Node *_right;  
    };                 

    friend class Node;

public:
    RBTree();
    virtual ~RBTree();

public:
    void insert(Element *key);
    void find_select(Node *r, std::vector<Element> &) const;
    void read(std::ifstream &stream_);
    void remove(Element *key);
    const Node *find(Element *key) const;
    bool isEmpty() const { return _root == nullptr; }
    Node *getRoot() const { return _root; }
    void print(int lvl) const;

protected:
    Node *insertNewBstEl(Element *key);
    void rebalance(Node *nd);
    Node *rebalanceDUG(Node *nd);
    void deleteNode(Node *nd);
    void rotLeft(Node *nd);
    void rotRight(Node *nd);

protected:
    RBTree(RBTree *); //ну не сегодня...
    RBTree *operator=(RBTree *);

protected:
    Node *_root;
};
template <typename Element>
RBTree<Element>::Node::~Node()
{
    if (_left)
        delete _left;
    if (_right)
        delete _right;
}
template <typename Element>
typename RBTree<Element>::Node *RBTree<Element>::Node::setLeft(Node *lf)
{
    if (_left == lf)
        return nullptr;
    if (lf)
    {
        if (lf->_parent)
        {
            if (lf->_parent->_left == lf)
                lf->_parent->_left = nullptr;
            else 
                lf->_parent->_right = nullptr;
        }
        lf->_parent = this;
    }
    Node *prevLeft = _left;
    _left = lf;
    if (prevLeft)
        prevLeft->_parent = nullptr;
    return prevLeft;
}

template <typename Element>
typename RBTree<Element>::Node *RBTree<Element>::Node::setRight(Node *rg)
{
    if (_right == rg)
        return nullptr;
    if (rg)
    {
        if (rg->_parent)
        {
            if (rg->_parent->_left == rg)
                rg->_parent->_left = nullptr;
            else
                rg->_parent->_right = nullptr;
        }
        rg->_parent = this;
    }
    Node *prevRight = _right;
    _right = rg;
    if (prevRight)
        prevRight->_parent = nullptr;
    return prevRight;
}

template <typename Element>
RBTree<Element>::RBTree()
{
    _root = nullptr;
}

template <typename Element>
RBTree<Element>::~RBTree()
{
    if (_root)
        delete _root;
}

template <typename Element>
void RBTree<Element>::deleteNode(Node *nd)
{
    if (nd == nullptr)
        return;
    delete nd;
}

template <typename Element>
void RBTree<Element>::read(std::ifstream &fp)
{
    std::string buf;
    std::string flag("");
    while (getline(fp, buf))
    {
        Element tmp(buf, flag);
        this->insert(&tmp);
    }
    this->print(0);
    std::cout << "tree created\n";
}

template <typename Element>
void RBTree<Element>::find_select(Node *roo, std::vector<Element> &obj) const
{
    Node *cur = roo;
    if (!cur)
        return;
    bool key = false;
    for (size_t i = 0; i < obj.size(); i++)
    { 
        if ((cur->_key.get_name() == obj[i].get_name()) || (obj[i].get_name()) == "")
        {
            if ((cur->_key.get_group() == obj[i].get_group()) || (obj[i].get_group() == -1))
            {
                if ((cur->_key.get_phone() == obj[i].get_phone()) || (obj[i].get_phone() == -1))
                {
                    key = true;
                    break;
                }
            }
        }
    }
    if (key)
    {
        cur->_key.print();
        std::cout << std::endl;
    }
    find_select(cur->_left, obj);
    find_select(cur->_right, obj);
}

template <typename Element>
void RBTree<Element>::insert(Element *key)
{
    Node *newNode = insertNewBstEl(key);
    newNode->setRed();
    rebalance(newNode);
}

template <typename Element>
void RBTree<Element>::remove(Element *key)
{
    Node *deleting_node = (Node *)find(key);
    if (!deleting_node)
    {
        std::cout << ("Deleted element doesn't exist!");
        return;
    }
    if (deleting_node->getLeft() && deleting_node->getRight())
    {
        Node *replacement = deleting_node;
        Node *current = deleting_node->_right;

        while (current)
        {
            deleting_node = current;
            current = current->_left;
        }
        replacement->_key = deleting_node->_key;
    }
    Node *child = deleting_node->_right ? deleting_node->_right : deleting_node->_left;
    Node *marked = deleting_node;

    if (!deleting_node->getParent())
    {
        if (child)
        {
            child->_parent = nullptr;
            child->setBlack();
        }
        _root = child;

        deleting_node->_left = nullptr;
        deleting_node->_right = nullptr;
        delete deleting_node;
        return;
    }
    if (deleting_node->isRed() || (child && child->isRed()))
    {
        if (child)
            child->setBlack();
        marked = nullptr;
    }
    else
    {
        while (marked)
        {
            if (marked == _root)
                marked = nullptr;
            else if (marked->isLeftChild())
            {
                Node *brother = marked->getParent()->_right; 
                if (brother && brother->isBlack() && ((brother->getLeft() && brother->getLeft()->isRed()) || (brother->getRight() && brother->getRight()->isRed())))
                {
                   
                    if (brother->getRight() && brother->getRight()->isRed())
                    {
                        
                        brother->_right->setBlack();
                        rotLeft(marked->_parent);
                        marked = nullptr;
                    }
                    else 
                    {
                        brother->setRed();
                        brother->_left->setBlack();
                        rotRight(brother);
                    }
                }

                
                else if (!brother || brother->isBlack())
                { 
                    if (brother)
                        brother->setRed();
                    if (marked->getParent()->isBlack()) 
                        marked = marked->_parent;
                    else
                    {
                        marked->_parent->setBlack();
                        marked = nullptr;
                    }
                }

                else
                {
                    brother->setBlack();
                    brother->_parent->setRed();
                    rotLeft(marked->_parent);
                }
            }

            else
            {                            
                Node *brother = marked->getParent()->_left; 

                if (brother && brother->isBlack() && ((brother->getLeft() && brother->getLeft()->isRed()) || (brother->getRight() && brother->getRight()->isRed())))
                {
                    if (brother->getLeft() && brother->getLeft()->isRed())
                    { 
                        brother->_left->setBlack();
                        rotRight(marked->_parent);
                        marked = nullptr;
                    }
                    else
                    { 
                        brother->setRed();
                        brother->_right->setBlack();
                        rotLeft(brother);
                    }
                }

                else if (!brother || brother->isBlack())
                { 
                    if (brother)
                        brother->setRed();

                    if (marked->getParent()->isBlack()) 
                        marked = marked->_parent;
                    else
                    {
                        marked->_parent->setBlack(); 
                        marked = nullptr;
                    }
                }
                else
                {
                    brother->setBlack();
                    brother->_parent->setRed();
                    rotRight(marked->_parent);
                }
            }
        }
    }
    deleting_node->isLeftChild() ? deleting_node->_parent->setLeft(child) : deleting_node->_parent->setRight(child);
    deleting_node->_left = nullptr;
    deleting_node->_right = nullptr;

    delete deleting_node;
}

template <typename Element>
const typename RBTree<Element>::Node *RBTree<Element>::find(Element *key) const
{
    Node *current = _root;

    while (current && current->_key != *key)
    {
        if (key < &current->_key)
            current = current->_left;
        else
            current = current->_right;
    }
    return current;
}

template <typename Element>
typename RBTree<Element>::Node *
RBTree<Element>::insertNewBstEl(Element *key)
{
    Node *current = _root;
    Node *parent = nullptr;
    Node *inserted = new Node(key);
    while (current)
    {
        if (current->_key == *key)
        { 
            std::cout << ("Already exists!");
            return nullptr;
        }
        parent = current;
        if (key < &current->_key)
            current = current->_left;
        else
            current = current->_right;
    }
    if (!parent) 
        _root = inserted;
    else if (key < &parent->_key) 
        parent->setLeft(inserted);
    else
        parent->setRight(inserted);
    return inserted;
}

template <typename Element>
typename RBTree<Element>::Node *
RBTree<Element>::rebalanceDUG(Node *nd)
{
    if (nd->getParent()->isLeftChild())
    {
        Node *uncle = nd->getParent()->getParent()->_right;

        
        if (!uncle || uncle->isBlack())
        {
            if (nd->isRightChild())
            { 
                rotLeft(nd->_parent);
                return nd->_left;
            }
            else
            {
                nd->_parent->setBlack();
                nd->getParent()->_parent->setRed();
                rotRight(nd->getParent()->_parent);
            }
        }
        else
        { 
            nd->_parent->setBlack();
            uncle->setBlack();
            nd->getParent()->_parent->setRed(); 
            return nd->getParent()->_parent;
        }
    }
    else
    { 
        Node *uncle = nd->getParent()->getParent()->_left;
        if (!uncle || uncle->isBlack())
        {
            if (nd->isLeftChild())
            { 
                rotRight(nd->_parent);
                return nd->_right; 
            }
            else
            {
                nd->_parent->setBlack();
                nd->getParent()->_parent->setRed();

                rotLeft(nd->getParent()->_parent);
            }
        }
        else
        { 
            nd->_parent->setBlack();
            uncle->setBlack();

            nd->getParent()->_parent->setRed(); 
            return nd->getParent()->_parent;
        }
    }

    return nd;
}
template <typename Element>
void RBTree<Element>::rebalance(Node *nd)
{
    while (nd->isDaddyRed())
        nd = rebalanceDUG(nd);
    _root->setBlack(); 
}

template <typename Element>
void RBTree<Element>::rotLeft(typename RBTree<Element>::Node *nd)
{
    Node *y = nd->_right;
    if (!y)
    {
        std::cout << ("Can't rotate left since the right child is nil");
        return;
    }
    if (y->_left)
        nd->setRight(y->_left);
    else
    {
        nd->setRight(nullptr);
        y->_parent = nullptr;
    }
    if (nd == _root)
    {
        _root = y;
        y->_parent = nullptr;
        y->setLeft(nd);
    } 

    else
    { 
        if (nd->isRightChild())
            nd->_parent->setRight(y);
        else
            nd->_parent->setLeft(y);
        y->setLeft(nd);
    }
}

template <typename Element>
void RBTree<Element>::rotRight(typename RBTree<Element>::Node *nd)
{
    Node *y = nd->_left;
    if (!y)
    {
        std::cout << ("Can't rotate left since the left child is nil");
        return;
    }
    if (y->_right) 
        nd->setLeft(y->_right);

    else
    {
        nd->setLeft(nullptr);
        y->_parent = nullptr;
    }
    if (nd == _root)
    {
        _root = y;
        y->_parent = nullptr;
        y->setRight(nd);
    } 
    else
    {
        if (nd->isLeftChild())
            nd->_parent->setLeft(y);
        else
            nd->_parent->setRight(y);
        y->setRight(nd); 
    }
}

template <typename Element>
void RBTree<Element>::print(int lvl) const
{
    if (!_root)
        std::cout << "no elements" << std::endl;
    else
    {
        if (_root->_left)
            _root->_left->printNode(lvl + 1);
        for (int i = 0; i < lvl; i++)
            std::cout << "   ";
        _root->_key.print();
        if (_root->_color == BLACK)
            std::cout << "black ";
        else
            std::cout << "red\n";
        if (_root->_right)
            _root->_right->printNode(lvl + 1);
    }
    std::cout << std::endl;
    return;
}

#endif 
