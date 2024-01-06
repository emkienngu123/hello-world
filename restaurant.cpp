#include "main.h"
//Function needed for instruction
//-----------------------------------------------------------------------------------------------
char encrypt(char ch, int s)
{
	char res;

	if (isupper(ch))
		res = char(int(ch + s - 65) % 26 + 65);

	// Encrypt Lowercase letters
	else
		res = char(int(ch + s - 97) % 26 + 97);

	return res;
}
int toDecimal(string str) {
	int ans = 0;
	int exp = 1;
	for (int i = (int)str.size() - 1; i >= 0; i--) {
		ans += exp * (str[i] - 48);
		exp *= 2;
	}
	return ans;
}
vector<vector<int>> C;
int choose(int  n, int k, const int& MOD) {
	//cout << C[n][k] << endl;
	if (C[n][k] != -1) return C[n][k] % MOD;
	else {
		if (k == 0) return C[n][k] = 1;
		else if (n == k) return C[n][k] = 1;
		else return C[n][k] = (choose(n - 1, k - 1, MOD) + choose(n - 1, k, MOD)) % MOD;
	}
}

//------------------------------------------------------------------------------------------------------------
//CLASSES IMPLEMENTATION
int ORDER = 0;
class HuffNode {
public:
	char ch;
	int wgt;
	HuffNode* left;
	HuffNode* right;
	bool isLeaf;
	HuffNode(char ch, int wgt, bool isLeaf, HuffNode* left, HuffNode* right) : ch(ch), wgt(wgt), left(left), right(right), isLeaf(isLeaf) {}
	void print() { if (ch == ' ') cout << wgt << "\n"; else cout << ch << "\n"; }
	~HuffNode() {}
};
class HuffTree {
public:
	int k = 0;
	int order;
	HuffNode* root;
	unordered_map<char, string> m;
public:
	HuffTree(char ch = ' ', int wgt = 0, bool isLeaf = false, HuffNode* left = nullptr, HuffNode* right = nullptr) {
		root = new HuffNode(ch, wgt, isLeaf, left, right);
		order = ORDER++;
	}
	~HuffTree() {}
	void BalanceTree(HuffNode*& root) {
		if (k == 3) return;
		if (root == nullptr) return;
		int bf = BalanceFactor(root);
		if (bf > 1) {
			int Cbf = BalanceFactor(root->left);
			if (Cbf <= -1) { rotateLeft(root->left); rotateRight(root); k += 1; }
			else {
				rotateRight(root); k++;
			}
		}
		else if (bf < -1) {
			int Cbf = BalanceFactor(root->right);
			if (Cbf >= 1) { rotateRight(root->right); rotateLeft(root); k += 1; }
			else {
				rotateLeft(root);
				k++;
			}
		}
		bf = BalanceFactor(root);
		if (bf > 1 || bf < -1) BalanceTree(root);
		else {
			BalanceTree(root->left);
			BalanceTree(root->right);
		}
	}

	bool helper(HuffNode*& root) {
		if (root == nullptr) return false;
		int bf = BalanceFactor(root);
		if (bf > 1) {
			int Cbf = BalanceFactor(root->left);
			if (Cbf <= -1) { rotateLeft(root->left); rotateRight(root); k += 1; }
			else {
				rotateRight(root);
			}
			return true;
		}
		else if (bf < -1) {
			int Cbf = BalanceFactor(root->right);
			if (Cbf >= 1) { rotateRight(root->right); rotateLeft(root); k += 1; }
			else {
				rotateLeft(root);
				k++;
			}
			return true;
		}
		if (helper(root->left)) return true;
		else return helper(root->right);
	}



	void balanceTree(HuffNode*& root) {
		int i;
		for (i = 0; i < 3; i++) {
			if (!helper(root)) break;
		}
		//cout << "number times of rotation: " << i << endl;
		return;
	}


	static HuffTree* makeHuffTree(HuffTree* left, HuffTree* right) {
		HuffTree* temp = new HuffTree(' ', left->root->wgt + right->root->wgt, false, left->root, right->root);
		delete left;
		delete right;
		//temp->print2D();
		temp->balanceTree(temp->root);
		return temp;
	}
	bool operator() (HuffTree* A, HuffTree* B) {
		if (A->root->wgt == B->root->wgt) {
			if (islower(A->root->ch) && isupper(B->root->ch)) return true;
			if (islower(A->root->ch) && islower(B->root->ch)) return A->root->ch < B->root->ch;
			if (isupper(A->root->ch) && isupper(B->root->ch)) return A->root->ch < B->root->ch;
			else return false;
		}
		return A->root->wgt > B->root->wgt;
	}
	int weight() {
		return root->wgt;
	}
	void FillTable() {
		queue<pair<HuffNode*, string>> q;
		q.push({ this->root,"" });
		while (!q.empty()) {
			HuffNode* node = q.front().first;
			string s = q.front().second;
			q.pop();
			if (!node->left && !node->right) m[node->ch] = s;
			if (node->left) {
				q.push({ node->left,s + "0" });
			}
			if (node->right) {
				q.push({ node->right,s + "1" });
			}
		}
	}

	string HuffCode(char ch) {
		return m[ch];
	}
	int GetHeight(HuffNode* root) {
		if (root == nullptr) return -1;
		if (root->isLeaf) return 0;
		int lheight = GetHeight(root->left);
		int rheight = GetHeight(root->right);
		return 1 + max(lheight, rheight);
	}

	int BalanceFactor(HuffNode* root) {
		if (root == nullptr) return 0;
		return GetHeight(root->left) - GetHeight(root->right);
	}

	void rotateLeft(HuffNode*& pR) {//update freqency whenever rotate a tree
		HuffNode* p = pR->right;
		pR->right = p->left;
		/*pR->wgt = pR->wgt - p->wgt;
		if (p->left) {
			pR->wgt += p->left->wgt;
			p->wgt -= p->left->wgt;
		}
		p->wgt += pR->wgt;*/
		p->left = pR;
		pR = p;
	}

	void rotateRight(HuffNode*& pR) {
		HuffNode* p = pR->left;
		pR->left = p->right;
		/*pR->wgt = pR->wgt - p->wgt;
		if (p->right) {
			pR->wgt += p->right->wgt;
			p->wgt -= p->right->wgt;
		}
		p->wgt += pR->wgt;*/
		p->right = pR;
		pR = p;
	}
	void HAND(HuffNode* root) {
		if (root == nullptr) return;
		HAND(root->left);
		root->print();
		HAND(root->right);
	}
	void HAND() {
		/*queue<HuffNode*> q;
		q.push(root);
		while (!q.empty()) {
			HuffNode* node = q.front();
			q.pop();
			if (node->isLeaf) node->print();
			if (node->left) q.push(node->left);
			if (node->right) q.push(node->right);
		}*/
		this->HAND(this->root);
	}
	void clear(HuffNode* root) {
		if (root == nullptr) return;
		else {
			clear(root->left);
			clear(root->right);
			delete root;
		}
	}
	void clear() {
		clear(this->root);
	}
	void print2DUtil(HuffNode* root, int space)
	{
		// Base case
		if (root == NULL)
			return;

		// Increase distance between levels
		space += 10;

		// Process right child first
		print2DUtil(root->right, space);

		// Print current node after space
		// count
		cout << endl;
		for (int i = 10; i < space; i++)
			cout << " ";
		if (root->ch != ' ') cout << root->ch << " " << root->wgt << "\n";
		else cout << root->wgt << "\n";

		// Process left child
		print2DUtil(root->left, space);
	}
	void print2D()
	{
		// Pass initial space count as 0
		print2DUtil(root, 0);
	}

};
class Comp {
public:
	bool operator() (const HuffTree* A, const HuffTree* B) const {
		if (A->root->wgt == B->root->wgt) return A->order > B->order;
		return A->root->wgt > B->root->wgt;
	}
};
//BST Tree Implementation
template<class T>
class BST {
protected:
	struct Node {
		T data;
		Node* left;
		Node* right;
		Node() : left(nullptr), right(nullptr) {}
		Node(const T& data, Node* left = nullptr, Node* right = nullptr) : data(data), left(left), right(right) { }
		Node(T&& data, Node* left = nullptr, Node* right = nullptr) : data(data), left(left), right(right) { }
	};
	Node* root;
	void clear(Node* root) {
		if (root == nullptr) return;
		else {
			clear(root->left);
			clear(root->right);
			delete root;
		}
	}
	void insert(const T& data, Node*& root) {
		if (root == nullptr) root = new Node(data);
		else {
			if (data < root->data) insert(data, root->left);
			else insert(data, root->right);
		}
	}
	T* find(const T& data, Node* root) {
		if (root) return root->data == data ? &root->data : (root->data > data ? find(data, root->left) : find(data, root->right));
		else return nullptr;
	}
	void remove(Node*& root, const T& data) {
		if (root) {
			if (root->data == data) {
				if (root->left && root->right) {
					Node* p = root->right;
					while (p->left) {
						p = p->left;
					}
					root->data = p->data;
					remove(root->right, root->data);
				}
				else if (root->left) {
					Node* temp = root;
					root = root->left;
					delete temp;
				}
				else if (root->right) {
					Node* temp = root;
					root = root->right;
					delete temp;
				}
				else {
					delete root;
					root = nullptr;
				}
			}
			else return root->data > data ? remove(root->left, data) : remove(root->right, data);
		}
	}
public:
	BST() : root(nullptr) { }
	~BST() { this->clear(); }
	void clear() {
		this->clear(this->root);
	}
	enum traverseType { DFS_NLR, DFS_NRL, DFS_LNR, DFS_RNL, DFS_LRN, DFS_RLN, BFS_TV };
	void traverse(function<void(T& data)> op, const traverseType& type) {
		switch (type)
		{
		case DFS_NLR:
			NLR(op, root);
			break;
		case DFS_NRL:
			NRL(op, root);
			break;
		case DFS_LNR:
			LNR(op, root);
			break;
		case DFS_RNL:
			RNL(op, root);
			break;
		case DFS_LRN:
			LRN(op, root);
			break;
		case DFS_RLN:
			RLN(op, root);
			break;
		case BFS_TV:
			BFS(op, root);
			break;
		default:
			break;
		}
	}
	void insert(const T& data) {
		//insert(data, this->root);
		Node** p = &root;
		while (*p) {
			if ((*p)->data > data) p = &((*p)->left);
			else p = &((*p)->right);
		}
		*p = new Node(data);
	}
	void remove(const T& data) {
		remove(this->root, data);
	}
	T* find(const T& data) {
		//return find(data,root);
		Node* p = this->root;
		while (p) {
			if (p->data == data) return &p->data;
			else p = p->data > data ? p->left : p->right;
		}
		return nullptr;
	}
protected:
	static void NLR(function<void(T& data)> op, Node* root) {
		if (root) {
			op(root->data);
			NLR(op, root->left);
			NLR(op, root->right);
		}
	}
	static void NRL(function<void(T& data)> op, Node* root) {
		if (root) {
			op(root->data);
			NLR(op, root->right);
			NLR(op, root->left);
		}
	}
	static void LNR(function<void(T& data)> op, Node* root) {
		if (root) {
			LNR(op, root->left);
			op(root->data);
			LNR(op, root->right);
		}
	}
	static void RNL(function<void(T& data)> op, Node* root) {
		if (root) {
			RNL(op, root->right);
			op(root->data);
			RNL(op, root->left);
		}
	}
	static void LRN(function<void(T& data)> op, Node* root) {
		if (root) {
			LRN(op, root->left);
			LRN(op, root->right);
			op(root->data);
		}
	}
	static void RLN(function<void(T& data)> op, Node* root) {
		if (root) {
			RLN(op, root->right);
			RLN(op, root->left);
			op(root->data);
		}
	}
	static void BFS(function<void(T& data)> op, Node* root) {
		queue<Node*> q;
		if (root) q.push(root);
		while (!q.empty()) {
			Node* front = q.front();
			q.pop();
			op(front->data);
			if (front->left) q.push(front->left);
			if (front->right) q.push(front->right);
		}
	}
	int NumOfPermutation(Node* root, const int& MAXSIZE) {
		if (root == nullptr) return 1;
		else {
			int m = this->size(root->left);
			int n = this->size(root->right);
			return (choose(m + n, n, MAXSIZE) * (NumOfPermutation(root->left, MAXSIZE) % MAXSIZE) * (NumOfPermutation(root->right, MAXSIZE) % MAXSIZE)) % MAXSIZE;
		}
	}

	int	size(Node* root) {
		queue<Node*> q;
		if (root == nullptr) return 0;
		q.push(root);
		int ans = 0;
		while (!q.empty()) {
			Node* temp = q.front();
			q.pop();
			ans++;
			if (temp->left) q.push(temp->left);
			if (temp->right) q.push(temp->right);
		}
		return ans;
	}
public:
	int NumOfPermutation(const int& MAXSIZE) {
		return NumOfPermutation(this->root, MAXSIZE);
	}

	int size() {
		return size(this->root);
	}

};


//RESTAURANT G IMPLEMENTATION
class RestaurantG {
private:
	int MAXSIZE;
	unordered_map<int, BST<int>*> m;
	unordered_map<int, list<int>> FIFO;
	queue<int> bin;
public:
	~RestaurantG() {
		/*for (int i = 1; i <= MAXSIZE; i++) {
			if (m[i] != nullptr) {
				delete m[i];
				m[i] = nullptr;
			}
		}*/
		while (!bin.empty()) {
			BST<int>* temp = m[bin.front()];
			delete temp;
			bin.pop();
		}
	}
	RestaurantG(int MAXSIZE) :MAXSIZE(MAXSIZE) {
	}
	int ID(int result) {
		return result % MAXSIZE + 1;
	}
	void insert(int result) {
		int id = ID(result);
		//cout << "insert sukuna restaurant: " << id << "-" << result << endl;
		if (m[id] == nullptr) {
			m[id] = new BST<int>();
			m[id]->insert(result);
			bin.push(id);
			FIFO[id].push_back(result);
			return;
		}
		m[id]->insert(result);
		FIFO[id].push_back(result);
		return;
	}
	void KOKUSEN() {
		for (auto it = m.begin(); it != m.end(); it++) {
			int id = it->first;
			BST<int>* tree = it->second;
			int Y = tree->NumOfPermutation(MAXSIZE) % MAXSIZE;
			if (Y > (int)FIFO[id].size()) Y = (int)FIFO[id].size();
			while (Y > 0) {
				tree->remove(FIFO[id].front());
				FIFO[id].pop_front();
				Y--;
			}
		}
	}

	void LIMITLESS(int NUM) {
		if (NUM <= 0 || NUM > MAXSIZE) return;
		if (FIFO[NUM].empty()) return;
		m[NUM]->traverse([](int& data) {cout << data << "\n"; }, BST<int>::DFS_LNR);
	}


};


//RESTAURANT S IMPLEMENTATION
class RestaurantS {
private:
	struct HeapNode {
		int order;
		int LABEL;
		list<int> q;
		HeapNode(int label = 0, int init = -1, int order = 0) : order(order), LABEL(label) {
			if (init != -1) q.push_back(init);
		}
		void print() {
			cout << LABEL << "-" << q.size() << "\n";
		}
	};
	int MAXSIZE;
	vector<HeapNode*> pD;
	int nE;
	unordered_map<int, HeapNode*> m;
	list<int> l;
	int k;
public:
	int ID(int result) {
		return result % MAXSIZE + 1;
	}
	RestaurantS(int MAXSIZE) : MAXSIZE(MAXSIZE), nE(0), k(0) {
	}
	~RestaurantS() {
		nE = 0;
		while (!l.empty()) {
			int id = l.front();
			delete m[id];
			l.pop_front();
		}
	}
	bool Comparator(HeapNode* A, HeapNode* B) { // A is less than B aka B is greater than A 
		if (A->q.size() == B->q.size()) return A->order < B->order;
		return A->q.size() < B->q.size();
	}
	int customerNum() {
		int ans = 0;
		for (int i = 0; i < nE; i++) {
			ans += (int)pD[i]->q.size();
		}
		return ans;
	}
	int GetTime(HeapNode* A) {
		int i = 0;
		for (auto it = l.begin(); it != l.end(); it++) {
			if (*it == A->LABEL) return i;
			i++;
		}
		return -1;
	}

	void UpdateList(int id) {//update list whenever insert or remove a customer
		l.remove(id);
		if (m[id] != nullptr && m[id]->q.size() != 0) {
			l.push_front(id); m[id]->order = k++;
		}
		return;
	}


	void insert(int result) {
		int id = ID(result);
		//cout << "insert sukuna: " << id << endl;
		//this->linearPrint();
		if (m[id] == nullptr) {
			m[id] = new HeapNode(id, result, k++);
			if (nE == MAXSIZE) return;
			if (nE == 0) {
				nE++;
				pD.push_back(m[id]);
				l.push_front(id);
				return;
			}
			else {
				nE++;
				pD.push_back(m[id]);
				l.push_front(id);
				HeapUp(nE - 1);
				return;
			}
		}
		else {
			int i;
			for (i = 0; i < nE; i++) {
				if (pD[i]->LABEL == id) break;
			}
			pD[i]->q.push_back(result);
			UpdateList(id);
			HeapDown(i);
			return;
		}
	}
	void HeapUp(int idx) {
		while (idx > 0) {
			int pIdx = (idx - 1) / 2;
			if (!Comparator(pD[idx], pD[pIdx])) return;
			else swap(pD[pIdx], pD[idx]);
			idx = pIdx;
		}
	}
	void HeapDown(int idx = 0) {
		while (idx < nE) {
			int cIdx = idx * 2 + 1;
			if (cIdx >= nE) return;
			if (cIdx + 1 < nE) {
				if (Comparator(pD[cIdx + 1], pD[cIdx])) cIdx++;
			}
			if (!Comparator(pD[cIdx], pD[idx])) return;
			else {
				swap(pD[idx], pD[cIdx]);
				idx = cIdx;
			}
		}
	}
	void CLEAVE(int num) {
		if (nE == 0) return;
		stack<int> st;
		st.push(0);
		//this->linearPrint();
		while (!st.empty()) {
			int idx = st.top();
			st.pop();
			int k = num;
			if (k > (int)pD[idx]->q.size()) k = (int)pD[idx]->q.size();
			for (auto it = pD[idx]->q.rbegin(); it != pD[idx]->q.rend(); it++) {
				if (k == 0) break;
				cout << pD[idx]->LABEL << "-" << *it << "\n";
				k--;
			}
			if (2 * idx + 2 < nE) st.push(2 * idx + 2);
			if (2 * idx + 1 < nE) st.push(2 * idx + 1);

		}
	}

	void removeFromHeap(int idx = 0) {
		if (nE == 0) return;
		if (nE == 1) {
			nE--;
			pD.pop_back();
			return;
		}
		pD[idx] = pD[--nE];
		HeapUp(idx);
		HeapDown(idx);
		pD.pop_back();
		return;
	}

	void Adjust(int ID) {
		int i = -1;
		for (i = 0; i < nE; i++) {
			if (pD[i]->LABEL == ID) break;
		}
		if (i == -1) return;
		if (pD[i]->q.size() == 0) {
			removeFromHeap(i);
			HeapNode* temp = m[ID];
			m[ID] = nullptr;
			delete temp;
		}
		else {
			HeapUp(i);
			HeapDown(i);
		}
	}
	void RemoveCustomerAt(int ID, int num = 0) {
		HeapNode* Area = m[ID];
		if ((int)Area->q.size() < num) num = (int)Area->q.size();
		while (!Area->q.empty() && num--) {
			cout << Area->q.front() << "-" << ID << "\n";
			Area->q.pop_front();
		}
		UpdateList(ID);
		Adjust(ID);
		return;
	}

	void KEITEIKEN(int num) { //kick num customers at 1 area
		if (nE == 0) return;
		int k = num;
		if (k > nE) k = nE;
		int timing = 0;
		list<int> clone(l.begin(), l.end());
		priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> q;
		for (auto it = l.rbegin(); it != l.rend(); it++) {
			q.push({ (int)m[*it]->q.size(),{timing++,*it} });
		}
		while (k--) {
			/*int MinCus = INT_MAX;
			int MinID = -1;
			for (auto it = clone.rbegin(); it != clone.rend(); it++) {
				if (m[*it]->q.size() < MinCus) {
					MinCus = (int)m[*it]->q.size();
					MinID = *it;
				}
			}
			if (MinID == -1) break;*/
			//cout << q.top().first << " " << q.top().second.first << " " << q.top().second.second << endl;
			int MinID = q.top().second.second;
			RemoveCustomerAt(MinID, num);
			//clone.remove(MinID);
			q.pop();
		}
	}
	void linearPrint() {
		for (int i = 0; i < nE; i++) {
			pD[i]->print();
		}
	}
};


//---------------------------------------------------------------------------------------------------------------------------------------
//INSTRUCTION ACCORDING TO ASSIGNMENT
int LAPSE(string name, list<HuffTree*>& SoFar) {
	int n = (int)name.size();
	string str(name.begin(), name.end());
	unordered_map<char, int> m;
	for (int i = 0; i < n; i++) {
		m[name[i]]++;
	}
	if (m.size() < 3) return -1;
	vector<pair<char, int>> arr;
	int k = 0;
	for (int i = 0; i < n; i++) {
		str[i] = encrypt(name[i], m[name[i]]);
	}
	//cout << str << endl;
	m.clear();
	for (int i = 0; i < n; i++) {
		m[str[i]]++;
	}
	for (auto it = m.begin(); it != m.end(); it++) {
		arr.push_back({ it->first,it->second });
		k++;
	}
	sort(arr.begin(), arr.begin() + k, [](pair<char, int> A, pair<char, int> B) { if (A.second == B.second) {
		if (islower(A.first) && isupper(B.first)) return true;
		if (islower(A.first) && islower(B.first)) return A.first < B.first;
		if (isupper(A.first) && isupper(B.first)) return A.first < B.first;
		return false;
	}; return A.second < B.second; });
	/*for (int i = 0; i < k; i++) {
		cout << arr[i].first << ": " << arr[i].second << endl;
	}*/
	vector<HuffTree*> TreeArray(k, nullptr);
	for (int i = 0; i < k; i++) {
		TreeArray[i] = new HuffTree(arr[i].first, arr[i].second, true);
	}
	priority_queue<HuffTree*, vector<HuffTree*>, Comp> q(TreeArray.begin(), TreeArray.end());
	while (q.size() > 1) {
		HuffTree* temp1 = q.top();
		/*cout << "pop the first tree\n";
		temp1->print2D();
		cout << "-----------------------\n";*/
		//cout << temp1->root->wgt << " " << temp1->root->ch << endl;
		q.pop();
		HuffTree* temp2 = q.top();
		q.pop();
		/*cout << "pop the second tree\n";
		temp2->print2D();
		cout << "-------------------\n";*/
		//cout << temp2->root->wgt << " " << temp2->root->ch << endl;
		HuffTree* temp3 = HuffTree::makeHuffTree(temp1, temp2);
		//cout << temp3->root->wgt << " " << temp3->root->ch << endl;
		/*cout << "cay sau khi tao\n";
		cout << "-------------------------------\n";
		temp3->print2D();
		cout << "---------------------------------\n";*/
		q.push(temp3);
	}
	HuffTree* tree = q.top();
	q.pop();
	//tree->print2D();
	//tree->HAND();
	SoFar.push_front(tree);
	tree->FillTable();
	string s = "";
	for (int i = 0; i < n; i++) {
		//cout << str[i] << " " << tree->HuffCode(str[i]) << endl;
		s += tree->HuffCode(str[i]);
	}
	string binaryStr = "";
	for (int i = (int)s.size() - 1, j = 9; i >= 0 && j >= 0; i--, j--) {
		binaryStr += s[i];
	}
	//cout << binaryStr << endl;
	int result = toDecimal(binaryStr);
	//cout << result<<endl;
	return result;
}

//----------------------------------------------------------------------------------------------------------------------
RestaurantG* Gojou = nullptr;
RestaurantS* Sukuna = nullptr;
void simulate(string filename)
{
	ifstream ss(filename);
	string str, maxsize, name, num;
	list<HuffTree*> SoFar;
	while (ss >> str)
	{
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			//cout << "MAXSIZE" << " " << maxsize << endl;
			Gojou = new RestaurantG(stoi(maxsize));
			Sukuna = new RestaurantS(stoi(maxsize));
			C.resize(500, vector<int>(500, -1));
		}
		else if (str == "LAPSE") // RED <NAME> <ENERGY>
		{
			ss >> name;
			//cout << "LAPSE" << " " << name << endl;
			int result = LAPSE(name, SoFar);
			//cout << result << endl;
			if (result == -1) continue;
			if (result % 2) Gojou->insert(result);
			else Sukuna->insert(result);
		}
		else if (str == "KOKUSEN") // BLUE <NUM>
		{
			//cout << "KOKUSEN\n";
			Gojou->KOKUSEN();
		}
		else if (str == "KEITEIKEN") // PURPLE
		{
			ss >> num;
			//cout << "KEITEIKEN " << num << endl;
			Sukuna->KEITEIKEN(stoi(num));
		}
		else if (str == "HAND") // REVERSAL
		{
			//cout << "HAND\n";
			if (!SoFar.empty()) SoFar.front()->HAND();
		}
		else if (str == "LIMITLESS") // UNLIMITED_VOID
		{
			ss >> num;
			//cout << "LIMITLESS" << " " << num << '\n';
			Gojou->LIMITLESS(stoi(num));
		}
		else if (str == "CLEAVE") // DOMAIN_EXPANSION
		{
			ss >> num;
			//cout << "CLEAVE " << num << endl;
			Sukuna->CLEAVE(stoi(num));
		}
	}
	delete Gojou;
	Gojou = nullptr;
	delete Sukuna;
	Sukuna = nullptr;
	while (!SoFar.empty()) {
		HuffTree* tree = SoFar.front();
		SoFar.pop_front();
		tree->clear();
		delete tree;
	}
	C.clear();
}