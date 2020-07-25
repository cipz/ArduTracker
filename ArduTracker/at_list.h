#pragma once

class Node {
  public:
    char friend_id[15];
    int seen_millis;
    time_t seen_time;
    Node * prev;
    Node * next;

    Node(const char *);
    Node(const char *, int, time_t, Node *, Node *);

};

Node::Node(const char * new_friend_id) {
  strlcpy(this->friend_id, new_friend_id, sizeof(this->friend_id));
  this->seen_millis = millis();
  this->seen_time = time(nullptr);
  this->prev = NULL;
  this->next = NULL;
}

Node::Node(const char * new_friend_id, int new_seen_millis, time_t new_seen_time, Node * new_prev, Node * new_next) {
  strlcpy(this->friend_id, new_friend_id, sizeof(this->friend_id));
  this->seen_millis = new_seen_millis;
  this->seen_time = new_seen_time;
  this->prev = new_prev;
  this->next = new_next;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

class List {
  public:
    Node * first;
    Node * last;
    int count;

    List();
    void appendNode(const char *);
    void appendNode(Node *);
    void appendList(List *);
    void removeNode(Node *);
    void deleteList();
    void removeDuplicates();
    void removeDuplicates(Node *);
    void printNodes();
    int getTotalNodes();
    int compactList(int);
    int compactList(int, Node *);

};

List::List() {
  this->first = NULL;
  this->last = NULL;
  this->count = 0;
}

void List::appendNode(const char * rx_msg) {

  Node * new_friend = new Node(rx_msg);
  if (!this->first) {
    this->first = new_friend;
    this->last = this->first;
  } else {
    this->last->next = new_friend;
    new_friend->prev = this->last;
    this->last = new_friend;
  }
  this->count++;

}

void List::appendNode(Node * new_node) {

  Node * new_friend = new Node(new_node->friend_id, new_node->seen_millis, new_node->seen_time, new_node->prev, new_node->next);
  if (!this->first) {
    this->first = new_friend;
    this->last = this->first;
  } else {
    this->last->next = new_friend;
    new_friend->prev = this->last;
    this->last = new_friend;
  }
  this->count++;

}

void List::appendList(List * new_list) {

  if(!new_list->first)
    return;

  if (this->first) {
    this->last->next = new_list->first;
    this->last = new_list->last;
  } else {
    Node * current_node = new_list->first;
    while (current_node) {
      this->appendNode(current_node);
      current_node = current_node->next;
    }
  }

}

void List::removeDuplicates() {

  if (this->count < 2)
    return;

  this->removeDuplicates(this->first);

}

void List::removeDuplicates(Node * starting_node) {

  if (!starting_node)
    return;

  Node * current_node = starting_node;
  while (current_node) {
    Node * next_node = current_node->next;
    while (next_node) {
      if (strcmp(current_node->friend_id, next_node->friend_id) == 0) {
        Node * tmp = next_node->next;
        this->removeNode(next_node);
        next_node = tmp;
      } else {
        next_node = next_node->next;
      }
    }
    current_node = current_node->next;
  }

}

void List::deleteList() {

  if (!this->count)
    return;

  Node * current_node = this->first;
  while (current_node) {
    Node * del = current_node;
    current_node = current_node->next;
    this->removeNode(del);
  }
}

void List::removeNode(Node * del_node) {

  if (DEBUG_MODE) {
    Serial.print("Deleting : ");
    Serial.print(del_node->friend_id);
    Serial.print(" , ");
    Serial.print(del_node->seen_millis);
    Serial.println(" ;");
  }

  if (this->first == del_node)
    this->first = del_node->next;

  if (del_node->next != NULL)
    del_node->next->prev = del_node->prev;
  else
    this->last = del_node->prev;

  if (del_node->prev != NULL)
    del_node->prev->next = del_node->next;

  if (this->last == del_node) {
    this->first = NULL;
    this->last = NULL;
  }

  delete del_node;
  this->count--;

}

void List::printNodes() {

  Serial.println();

  Serial.print("There are ");
  Serial.print(this->count);
  Serial.println(" nodes");

  if (!this->count)
    return;

  Serial.print("FIRST #1: ");
  Serial.print(this->first->friend_id);
  Serial.print("', ");
  Serial.print(this->first->seen_millis);
  Serial.println(" ;");

  Serial.print("LAST #");
  Serial.print(this->count);
  Serial.print(": '");
  Serial.print(this->last->friend_id);
  Serial.print("', ");
  Serial.print(this->last->seen_millis);
  Serial.println(" ;");

  Serial.println();

  int count = 0;
  Node * current_node = this->first;
  while (current_node) {
    count++;
    Serial.print("#");
    Serial.print(count);
    Serial.print(": '");
    Serial.print(current_node->friend_id);
    Serial.print("', ");
    Serial.print(current_node->seen_millis);
    Serial.println(" ;");
    current_node = current_node->next;
  }
}

int List::getTotalNodes() {
  return this->count;
}

int List::compactList(int fresh) {

  if (!this->count)
    return 0;

  return compactList(fresh, this->first);

}

int List::compactList(int fresh, Node * starting_node) {

  if (!starting_node)
    return 0;

  Node * current_node = starting_node;
  int count = 0;

  while (current_node) {
    if (millis() - current_node->seen_millis > fresh) {
      // Serial.println(millis() - current_node->seen_millis);
      Node * tmp = current_node->next;
      this->removeNode(current_node);
      current_node = tmp;
      count++;
    } else {
      current_node = current_node->next;
    }
  }

  this->last->next = NULL;
}
