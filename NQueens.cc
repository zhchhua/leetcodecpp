class Solution {
	public:10.4 N-Queens
	       vector<vector<string> > solveNQueens(int n) {
		       this->columns = vector<int>(n, 0);
		       this->main_diag = vector<int>(2 * n, 0);
		       this->anti_diag = vector<int>(2 * n, 0);
		       vector<vector<string> > result;
		       vector<int> C(n, 0); // C[i] 表示第 i 行皇后所在的列编号
		       dfs(C, result, 0);
		       return result;
	       }
	private:
	       vector<int> columns; // 表示已经放置的皇后占据了哪些列
	       vector<int> main_diag; // 占据了哪些主对角线
	       vector<int> anti_diag; // 占据了哪些副对角线
	       void dfs(vector<int> &C, vector<vector<string> > &result, int row) {
		       const int N = C.size();
		       if (row == N) { // 终止条件,也是收敛条件,意味着找到了一个可行解
			       vector<string> solution;
			       for (int i = 0; i < N; ++i) {
				       string s(N, '.');
				       for (int j = 0; j < N; ++j) {
					       if (j == C[i]) s[j] = 'Q';
				       }
				       solution.push_back(s);
			       }
			       result.push_back(solution);
			       return;
		       }
		       for (int j = 0; j < N; ++j) { // 扩展状态,一列一列的试
			       const bool ok = columns[j] == 0 && main_diag[row + j] == 0 &&
				       anti_diag[row - j + N] == 0;
			       if (!ok) continue; // 剪枝:如果合法,继续递归
			       // 执行扩展动作
			       C[row] = j;
			       columns[j] = main_diag[row + j] = anti_diag[row - j + N] = 1;
			       dfs(C, result, row + 1);
			       // 撤销动作
			       columns[j] = main_diag[row + j] = anti_diag[row - j + N] = 0;
		       }
	       }
};
