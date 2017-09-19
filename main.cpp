#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

template<typename T>
struct BinaryMin:public binary_function<T, T, T>{
    T operator()(const T& x, const T& y) const{return min(x, y);}
    const static T neutral;
};

template <typename T>
const T BinaryMin<T>::neutral = std::numeric_limits<T>::max();


template<typename T>
struct BinaryMax:public binary_function<T, T, T>{
    T operator()(const T& x, const T& y) const{return max(x, y);}
    const static T neutral;
};

template <typename T>
const T BinaryMax<T>::neutral = std::numeric_limits<T>::min();

template<typename T, typename Operation>
class SparseTable{
private:
    vector<vector<int> > arr; //sp-table, contains numbers
    vector<T> v; //contains data
    vector<int> log_count; //log2 pre-counts
    size_t m, n; //sizes of sp-table, are counted in constructor
    void count(); //fills log-count vector
public:
    explicit SparseTable(const vector<T>&);
    pair<int, T> query(int, int);
};

template<typename T, typename Operation>
SparseTable<T, Operation>::SparseTable(const vector<T>& vect){
    Operation op;
    n = vect.size();
    if (n > 1)
        m = (size_t)(floor(log2(n - 0.5)) + 1);
    else
        m = 1;
    arr.resize(m);
    for (auto &it : arr)
        it.resize(n + 1);
    v.push_back(Operation::neutral);
    for (int i = 0; i < n; ++i)
        v.push_back(vect[i]);
    count();
    for (int i = 1; i <= n; ++i)
        arr[0][i] = i;
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j <= n - pow(2, i) + 1; ++j) {
            T val = op(v[arr[i - 1][j]], v[arr[i - 1][j + pow(2, i - 1)]]);
            if (val == v[arr[i - 1][j]])
                arr[i][j] = arr[i - 1][j];
            else
                arr[i][j] = arr[i - 1][j + pow(2, i - 1)];
        }
    }
};

template<typename T, typename Operation>
void SparseTable<T, Operation>::count() {
    for (int i = 0; i <= n; ++i)
        log_count.push_back(0);
    int k = 0;
    while(k < m)
        log_count[pow(2, k)] = k++; //filling 2^k fields
    for (int i = 1; i <= n; ++i)
        if(log_count[i] == 0)
            log_count[i] = log_count[i - 1];
}

template<typename T, typename Operation>
pair<int, T> SparseTable<T, Operation>::query(int x, int y) {
    Operation op;
    int k = log_count[y - x + 1], p = (int)pow(2, k);
    if (op(v[arr[k][x]], v[arr[k][y - p + 1]]) == v[arr[k][x]])
        return make_pair(arr[k][x], v[arr[k][x]]);
    else
        return make_pair(arr[k][y - p + 1], v[arr[k][y - p + 1]]);
}


int main() {
    vector<int> c;
    int n, p, q;
    cin >> n;
    for (int i = 0; i < n; ++i){
        cin >> q;
        c.push_back(q);
    }
    pair<int, int> ans;
    SparseTable<int, BinaryMax<int>> st(c);
    cin >> n;
    for (int i = 0; i < n; ++i){
        cin >> p >> q;
        ans = st.query(p, q);
        cout << ans.second << ' ' << ans.first << endl;
    }
    return 0;
}
