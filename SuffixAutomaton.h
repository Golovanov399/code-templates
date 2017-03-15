struct Automaton {
	struct Node {
		int len;
		Node *link;
		vector<Node*> to;

		Node(int _len = 0): len(_len) {
			len = 0;
			to.resize(26, NULL);
			link = NULL;
		}

		Node* go(char c) {
			c -= 'a';
			return to[c];
		}
	};

	Node *root, *last;

	Automaton() {
		root = last = new Node();
	}

	void add(char c) {
		c -= 'a';
		Node *cur = new Node(last->len + 1);
		Node *tmp = last;
		while (tmp) {
			if (tmp->to[c]) {
				break;
			}
			tmp->to[c] = cur;
			tmp = tmp->link;
		}
		if (tmp) {
			Node *nx = tmp->to[c];
			if (nx->len == tmp->len + 1) {
				cur->link = nx;
			} else {
				Node *clone = new Node(tmp->len + 1);
				clone->to = nx->to;
				clone->link = nx->link;
				cur->link = clone;
				nx->link = clone;
				tmp->to[c] = clone;
				while (tmp = tmp->link) {
					if (tmp->to[c] == nx) {
						tmp->to[c] = clone;
					} else {
						break;
					}
				}
			}
		} else {
			cur->link = root;
		}
		last = cur;
	}

	Node* go(const string& s) const {
		Node *result = root;
		for (char c : s) {
			if (!result) {
				break;
			} else {
				result = result->go(c);
			}
		}
		return result;
	}
};
