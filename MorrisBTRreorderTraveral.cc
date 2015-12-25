class Solution {
	public:
		vector<int> preorderTraversal(TreeNode *root) {
			vector<int> result;
			TreeNode *cur, *prev;
			cur = root;
			while (cur != nullptr) {
				if (cur->left == nullptr) {
					result.push_back(cur->val);
					prev = cur; /* cur 刚刚被访问过 */
					cur = cur->right;
				} else {/* 查找前驱 */
					TreeNode *node = cur->left;
					while (node->right != nullptr && node->right != cur)
						node = node->right;
					if (node->right == nullptr) { /* 还没线索化,则建立线索 */
						result.push_back(cur->val); /* 仅这一行的位置与中序不同 */
						node->right = cur;
						prev = cur; /* cur 刚刚被访问过 */
						cur = cur->left;
					} else {
						node->right = nullptr;
						cur = cur->right;
					}
				}
			}
			return result;
		}
};
