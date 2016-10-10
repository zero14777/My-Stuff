class Matrix {
    public :vector<vector<int>> a;
    
    Matrix operator + (Matrix x) {
        vector<vector<int>> result;
        for (int n = 0; n < a.size(); n++) {
            vector<int> temp;
            for (int m = 0; m < a[n].size(); m++) {
                temp.push_back(a[n][m] + x.a[n][m]);
            }
            result.push_back(temp);
        }
        Matrix result_matrix;
        result_matrix.a = result;
        return result_matrix;
    }
};