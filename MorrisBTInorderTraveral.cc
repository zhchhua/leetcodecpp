class Solution {
	public:
		vector<int> inorderTraversal(TreeNode *root) {
			vector<int> result;
			TreeNode *cur, *prev;
			cur = root;
			while (cur != nullptr) {
				if (cur->left == nullptr) {
					result.push_back(cur->val);
					prev = cur;
					cur = cur->right;
				} else {
					/* 查找前驱 */
					TreeNode *node = cur->left;
					while (node->right != nullptr && node->right != cur)
						node = node->right;
					if (node->right == nullptr) { /* 还没线索化,则建立线索 */
						node->right = cur;
						/* prev = cur; 不能有这句,cur 还没有被访问 */
						cur = cur->left;
					} else {/* 已经线索化,则访问节点,并删除线索 */
						result.push_back(cur->val);
						node->right = nullptr;
						prev = cur;
						cur = cur->right;
					}
				}
			}
			return result;
		}
};
