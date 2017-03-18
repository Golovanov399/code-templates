template <typename T>
struct Cartesian {
	struct Node {
		T data;
		int prior;
		int size;
		Node *left, *right;
		bool rev;

		explicit Node(const T& val) {
			data = val;
			prior = rand();
			left = right = nullptr;
			size = 1;
			rev = false;
		}

		~Node() {
			if (left) {
				delete left;
			}
			if (right) {
				delete right;
			}
		}
	};

	void relax(Node* node) {
		node->size = 1;
		// update your statistics here
		if (node->left) {
			node->size += node->left->size;
		}
		if (node->right) {
			node->size += node->right->size;
		}
	}

	void relax_rev(Node* node) {
		if (node->rev) {
			swap(node->left, node->right);
			if (node->left) {
				node->left->rev ^= 1;
			}
			if (node->right) {
				node->right->rev ^= 1;
			}
			node->rev = 0;
		}
	}

	pair<Node*, Node*> split(Node* node, const T& val) { // (<=, >)
		if (!node) {
			return {nullptr, nullptr};
		}
		// no reverse!
		if (node->data <= val) {
			auto tmp = split(node->right, val);
			node->right = tmp.first;
			relax(node);
			return {node, tmp.second};
		} else {
			auto tmp = split(node->left, val);
			node->left = tmp.second;
			relax(node);
			return {tmp.first, node};
		}
	}

	pair<Node*, Node*> split_by_cnt(Node* node, int cnt) { // (cnt, n - cnt)
		if (!node) {
			assert(cnt == 0);
			return {nullptr, nullptr};
		}
		relax_rev(node);
		int lsz = (node->left ? node->left->size : 0);
		if (cnt > lsz) {
			auto tmp = split_by_cnt(node->right, cnt - lsz - 1);
			node->right = tmp.first;
			relax(node);
			return {node, tmp.second};
		} else {
			auto tmp = split_by_cnt(node->left, cnt);
			node->left = tmp.second;
			relax(node);
			return {tmp.first, node};
		}
	}

	Node* merge(Node* first, Node* second) {
		if (!first) {
			return second;
		} else if (!second) {
			return first;
		} else {
			if (first->prior < second->prior) {
				relax_rev(first);
				first->right = merge(first->right, second);
				relax(first);
				return first;
			} else {
				relax_rev(second);
				second->left = merge(first, second->left);
				relax(second);
				return second;
			}
		}
	}

	template <typename Callback>
	void in_order(Node* node, Callback callback) {
		if (!node) {
			return;
		}
		relax_rev(node);
		in_order(node->left, callback);
		callback(node->data);
		in_order(node->right, callback);
	}

	// Cartesian interface starts here

	Node *root;

	Cartesian() {
		root = nullptr;
	}

	~Cartesian() {
		delete root;
	}

	void append(const T& val) {
		auto new_node = new Node(val);
		root = merge(root, new_node);
	}

	void push_back(const T& val) {
		append(val);
	}

	void add(const T& val, int pos) {
		auto p = split_by_cnt(root, pos);
		root = merge(p.first, merge(new Node(val), p.second));
	}

	void reverse(int l, int r) { // [l, r)
		if (r <= l) {
			return;
		}
		auto p = split_by_cnt(root, r);
		auto q = split_by_cnt(p.first, l);
		q.second->rev ^= 1;
		root = merge(merge(q.first, q.second), p.second);
	}

	vector<T> get_all() {
		vector<T> result;
		in_order(root, [&result](const T& data){
			result.push_back(data);
		});
		return result;
	}
};
