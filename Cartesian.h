struct Node {
	int value;
	int prior;
	int sz;
	char rev;
	Node *l, *r;

	Node(int value):value(value){
		sz = 1;
		prior = rand();
		rev = 0;
	}

	void relax(){
		sz = 1;
		if (l)
			sz += l->sz;
		if (r)
			sz += r->sz;
	}

	void relrev(){
		if (rev){
			swap(l, r);
			if (l)
				l->rev ^= 1;
			if (r)
				r->rev ^= 1;
			rev = 0;
		}
	}

	pair<Node*, Node*> split(int x){
		if (x == 0)
			return {NULL, this};
		else if (x == sz)
			return {this, NULL};
		else {
			relrev();
			int lsz = l ? l->sz : 0;
			if (x <= lsz){
				auto p = l->split(x);
				l = p.second;
				relax();
				return {p.first, this};
			} else
			if (x == lsz + 1){
				Node* tmp = r;
				r = NULL;
				relax();
				return {this, tmp};
			} else {
				x -= lsz + 1;
				auto p = r->split(x);
				r = p.first;
				relax();
				return {this, p.second};
			}
		}
	}
};

Node* merge(Node *l, Node *r){
	if (!l)
		return r;
	if (!r)
		return l;
	if (l->prior > r->prior){
		l->relrev();
		l->r = merge(l->r, r);
		l->relax();
		return l;
	} else {
		r->relrev();
		r->l = merge(l, r->l);
		r->relax();
		return r;
	}
}

struct Cart{
	Node *root;

	Cart(){
		root = new Node(0);
	}

	void add(int x){
		Node *tmp = new Node(x);
		root = merge(root, tmp);
	}

	void rev(int l, int r){
		auto p = root->split(l - 1);
		auto q = p.second->split(r - l + 1);
		q.first->rev ^= 1;
		root = merge(p.first, merge(q.first, q.second));
	}

	int pop(int v){
		auto p = root->split(v);
		auto q = p.second->split(1);
		int res = q.first->value;
		root = merge(p.first, q.second);
		return res;
	}
};
