
class Node {
  public:
    char friend_id[15];
    int seen_moment;
    Node * prev;
    Node * next;

    Node(const char *, Node * );

};

Node::Node(const char * rx_msg, Node * prev_node) {
  strlcpy(this->friend_id, rx_msg, sizeof(this->friend_id));
  this->seen_moment = millis();
  this->prev = prev_node;
  this->next = NULL;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

class List {
  public:
    Node * first;
    Node * last;
    int count;

    List();
    void addNode(const char *);
    void removeNode(Node *);
    void printNodes();
    int getTotalNodes();
    int compactList(int);

};

List::List() {
  this->first = NULL;
  this->last = NULL;
  this->count = 0;
}

void List::addNode(const char * rx_msg) {
  Node * new_friend = new Node(rx_msg, this->last);
  if (!this->first) {
    this->first = new_friend;
    this->last = this->first;
  } else {
    this->last->next = new_friend;
    this->last = new_friend;
  }
  this->count++;
}

void List::removeNode(Node * del_node) {

//  Serial.print("Deleting : ");
//  Serial.print(del_node->friend_id);
//  Serial.print(" , ");
//  Serial.print(del_node->seen_moment);
//  Serial.println(" ;");

  if (this->first == this->last) {
    this->first = NULL;
    this->last = NULL;
    this->count = 0;
    return;
  }

  if (del_node == this->first) {
    this->first = this->first->next;
    this->first->prev = NULL;
  } else {
    del_node->prev->next = del_node->next;
  }

  if (del_node == this->last) {
    this->last = this->last->prev;
    this->last->next = NULL;
  } else {
    del_node->next = del_node->prev;
  }

  delete del_node;

  this->count--;

}

void List::printNodes() {
  int count = 0;
  Node * current_node = this->first;
  while (current_node) {
    count++;
    Serial.print("#");
    Serial.print(count);
    Serial.print(": '");
    Serial.print(current_node->friend_id);
    Serial.print("', ");
    Serial.print(current_node->seen_moment);
    Serial.println(" ;");
    current_node = current_node->next;
  }
}

int List::getTotalNodes() {
  return this->count;
}

int List::compactList(int fresh) {

  Node * current_node = this->first;
  int count = 0;

  while (current_node) {
    if (millis() - current_node->seen_moment > fresh){
        this->removeNode(current_node);
        count++;
    }
    current_node = current_node->next;
  }

  return count;

}
