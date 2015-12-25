class Solution {
	public:
		int maximalRectangle(vector<vector<char> > &matrix) {
			if (matrix.empty()) return 0;
			const int m
				const int n
				vector<int>
				vector<int>
				vector<int>
				= matrix.size();
			= matrix[0].size();
			H(n, 0);
			L(n, 0);
			R(n, n);
			int ret = 0;
			for (int i = 0; i < m; ++i) {
				int left = 0, right = n;
				// calculate L(i, j) from left to right
				for (int j = 0; j < n; ++j) {
					if (matrix[i][j] == '1') {
						++H[j];
						L[j] = max(L[j], left);
					} else {
						left = j+1;
						H[j] = 0; L[j] = 0; R[j] = n;
					}
				}
				// calculate R(i, j) from right to left
				for (int j = n-1; j >= 0; --j) {
					if (matrix[i][j] == '1') {
						R[j] = min(R[j], right);
						ret = max(ret, H[j]*(R[j]-L[j]));
					} else {
						right = j;
					}
				}
			}
			return ret;
		}
};
