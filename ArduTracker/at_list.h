
class Node {
  public:
    char friend_id[15];
    int seen_moment;
    Node * next;

    Node(const char *);
};

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

Node::Node(const char * rx_msg) {
  strlcpy(this->friend_id, rx_msg, sizeof(this->friend_id));
  this->seen_moment = millis();
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
    Node * getNode();
    void printNodes();
    int getTotalNodes();
    int compactList(int);

};

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

List::List() {
  this->first = NULL;
  this->last = NULL;
  this->count = 1;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void List::addNode(const char * rx_msg) {
  Node * new_friend = new Node(rx_msg);
  if (!this->first) {
    this->first = new_friend;
    this->last = this->first;
  } else {
    this->last->next = new_friend;
    this->last = new_friend;
  }
  this->count++;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

Node * List::getNode() {
  return new Node("");
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void List::printNodes() {
  int count = 1;
  Node * current_node = this->first;
  while (current_node) {
    Serial.print("#");
    Serial.print(count);
    Serial.print(": '");
    Serial.print(current_node->friend_id);
    Serial.print("', ");
    Serial.print(current_node->seen_moment);
    Serial.println(" ;");
    current_node = current_node->next;
    count++;
  }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

int List::getTotalNodes() {
  return this->count;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// deletes duplicates and keeps only encounters x seconds fresh
int List::compactList(int fresh){

    int count = 0;

    for (int i = 0; i < this->getTotalNodes(); i++){
        // Do the thing
    }

    return count;
    
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
