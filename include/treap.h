#ifndef SD_TEMA3_INCLUDE_TREAP_H_
#define SD_TEMA3_INCLUDE_TREAP_H_

#include <iostream>
#include <unordered_map>

template <typename T, typename R> struct Treap {
  T key;
  R priority;
  Treap<T, R> *left, *right;
  bool nil;
  int nr_nodes;
  std::unordered_map<std::string, bool> m_rate;

  Treap() : priority(-1), left(NULL), right(NULL), nil(true), nr_nodes(0) {}
  ~Treap() {
    delete this -> left;
    delete this -> right;
  }

  // Adaugam date, transformand un nod nil intr-un nod obisnuit
  void addData(T key, R priority) {
    this->nil = false;
    this->key = key;
    this->priority = priority;
    this->nr_nodes = 1;
    this->left = new Treap();
    this->right = new Treap();
  }

  // Stergem un nod obisnuit, transformandu-l intr-unul nil
  void delData() {
    this->nil = true;
    this->priority = -1;
    delete this->left;
    delete this->right;
    this->nr_nodes = 0;
  }

  bool isNil() {
    return this->nil;
  }

  bool find(T key) {
    if (this->isNil()) {
      return false;
    }
    // TODO: Completati functia de cautare
    if(this -> key == key) {
        return true;
    }
    if(this -> key > key) {
        return this -> left -> find(key);
    } else {
        return this -> right -> find(key);
    }
    return false;
  }

  int get_priority(T key) {
    if (this->isNil()) {
      return 0;
    }
    // TODO: Completati functia de cautare
    if(this -> key == key) {
        return this -> priority;
    }
    if(this -> key > key) {
        return this -> left -> get_priority(key);
    } else {
        return this -> right -> get_priority(key);
    }
    return 0;
  }

  std::unordered_map<std::string, bool> get_hash(T key) {
    if (this->isNil()) {
      return std::unordered_map<std::string, bool>();
    }
    // TODO: Completati functia de cautare
    if(this -> key == key) {
        return this -> m_rate;
    }
    if(this -> key > key) {
        return this -> left -> get_hash(key);
    } else {
        return this -> right -> get_hash(key);
    }
    return std::unordered_map<std::string, bool>();
  }

  void set_hash(T key, std::unordered_map<std::string, bool> new_hash) {
    if (this->isNil()) {
      return;
    }
    // TODO: Completati functia de cautare
    if(this -> key == key) {
        m_rate = new_hash;
    }
    if(this -> key > key) {
        this -> left -> set_hash(key, new_hash);
    } else {
        this -> right -> set_hash(key, new_hash);
    }
  }

  void rotateRight(Treap<T, R> *&fatherPointer) {
    Treap<T, R> *node = this -> left;
    fatherPointer = this -> left;
    this -> left = this -> left -> right;
    node -> right = this;

  }

  void rotateLeft(Treap<T, R> *&fatherPointer) {
    Treap<T, R> *node = this -> right;
    fatherPointer = this -> right;
    this -> right = this -> right -> left;
    node -> left = this;
  }

  void insert(Treap<T, R> *&fatherPointer, T key, R priority) {
    if (this->isNil()) {
      this->addData(key, priority);

      return ;
    }

    if (key < this -> key) {
        this -> left -> insert(this -> left, key, priority);
    } else {
        this -> right -> insert(this -> right, key, priority);
    }

    if ((this -> left -> priority > this -> priority) ||
        (this -> left -> priority == this -> priority &&
         this -> left -> key < this -> key)){
        this -> rotateRight(fatherPointer);
    } else if ((this -> right -> priority > this->priority) ||
               (this -> right -> priority == this -> priority &&
                this -> right -> key < this -> key)){
        this -> rotateLeft(fatherPointer);
    }
  }

  void erase(Treap<T, R> *&fatherPointer, T key) {
    if (this->isNil()) {
      return ;
    }

    if (key < this->key) {
        this -> left -> erase(this -> left, key);
    } else if (key > this->key) {
        this -> right -> erase(this -> right, key);
    } else if (this->left->isNil() && this->right->isNil()) {
      this->delData();
    } else {
      if (this->left->priority > this->right->priority) {
          this -> rotateRight(fatherPointer);
          fatherPointer -> right -> erase(fatherPointer -> right, key);
      } else {
          this -> rotateLeft(fatherPointer);
          fatherPointer -> left -> erase(fatherPointer -> left, key);
      }
    }
  }

  T peek() {
    return this -> key;
  }

};

#endif  // SD_TEMA3_INCLUDE_TREAP_H_
