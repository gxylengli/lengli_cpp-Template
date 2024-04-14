//字符串哈希
struct string_hash{
    int b=13331, P=1000000007, p[N], rh[N],h[N],len;
    vector<int> sd={163227661,217636919,290182597,386910137,515880193,1000000007};
    int inline get(int l, int r){
        return (h[r] - (LL)h[l - 1] * p[r - l + 1] % P + P) % P;
    }
    void inline get_sd(){
        int len=sd.size();
        int t=rand()%len;
        P=sd[t];
    }
    void inline build(int n,string s) {
        p[0] = 1,len=n;
        for(int i = 1; i <= n; i++){
            p[i] = (LL)p[i - 1] * b % P;
            h[i] = ((LL)h[i - 1] * b + s[i-1]) % P;
        }
        for(int i=1;i<=n;i++) rh[i]=((LL)rh[i - 1] * b + s[n-i]) % P;
    }
    bool isprs(int l,int r){
        return (h[r]-(LL)h[l-1]*p[r-l+1]%P+P)%P==(rh[len-l+1]-(LL)rh[len-r]*p[r-l+1]%P+P)%P;
    }
}tr;

//KMP

vector<int> KMP(string a,string b){
    int n=a.size(),m=b.size();
	a="#"+a,b="#"+b;
	vector<int> q(n+1,0);
    vector<int> ne(m+1,0);
	for(int i=2,j=0;i<=m;i++){
		while(j and b[j+1]!=b[i]) j=ne[j];
		if(b[j+1]==b[i]) j++;
		ne[i]=j;
	}
	for(int i=1,j=0;i<=n;i++){
		while(j and a[i]!=b[j+1]) j=ne[j];
		if(a[i]==b[j+1]) j++;
		q[i]=j;
		if(j==m) j=ne[j];
	}
	return q;
}

//Z函数（从i开始的后缀与原串最长公共前缀）

z[1]=s.size()-1;
for(int i=2,l,r=0;i<s.size();i++){
	if(i<=r) z[i]=min(z[i-l+1],r-i+1);
	while(z[i]+i<s.size() and s[z[i]+1]==s[z[i]+i]) z[i]++;
	if(i+z[i]-1>r) r=i+z[i]-1,l=i;
}

//后缀数组（SA）
template<size_t size>
struct SuffixArray {
    bool t[size << 1];
    int sa[size], ht[size], rk[size];//sa.idx:1~n,ht.idx:1~n,rk.idx:0~(n-1)
 
    inline bool islms(const int i, const bool *t) { 
        return i > 0 && t[i] && !t[i - 1]; 
    }
 
    template<class T>
    inline void sort(T s, int *sa, const int len, const int sigma, const int sz, bool *t, int *b, int *cb, int *p) {
        memset(b, 0, sizeof(int) * sigma);
        memset(sa, -1, sizeof(int) * len);
        for (int i = 0; i < len; i++) b[s[i]]++;
        cb[0] = b[0];
        for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
        for (int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
        for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
        for (int i = 0; i < len; i++) if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
        cb[0] = b[0];
        for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
        for (int i = len - 1; i >= 0; i--) if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
    }
 
    //len要字符串长度+1!
    template<class T>
    inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1, const int sigma) {
        int i, j, x, p = -1, cnt = 0, sz = 0, *cb = b + sigma;
        for (t[len - 1] = 1, i = len - 2; i >= 0; i--) t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
        for (i = 1; i < len; i++) if (t[i] && !t[i - 1]) b1[sz++] = i;
        sort(s, sa, len, sigma, sz, t, b, cb, b1);
        for (i = sz = 0; i < len; i++) if (islms(sa[i], t)) sa[sz++] = sa[i];
        for (i = sz; i < len; i++) sa[i] = -1;
        for (i = 0; i < sz; i++) {
            for (x = sa[i], j = 0; j < len; j++) {
                if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) { cnt++, p = x; break; }
                else if (j > 0 && (islms(x + j, t) || islms(p + j, t))) break;
            }
            sa[sz + (x >>= 1)] = cnt - 1;
        }
        for (i = j = len - 1; i >= sz; i--) if (sa[i] >= 0) sa[j--] = sa[i];
        int *s1 = sa + len - sz, *b2 = b1 + sz;
        if (cnt < sz) sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
        else for (i = 0; i < sz; i++) sa[s1[i]] = i;
        for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
        sort(s, sa, len, sigma, sz, t, b, cb, b2);
    }
 
    template<class T>
    inline void getHeight(T s, int n) {//字符串与字符串长度下标0~(n-1),height数组下标1~n
        for (int i = 1; i <= n; i++) rk[sa[i]] = i;
        int j = 0, k = 0;
        for (int i = 0; i < n; ht[rk[i++]] = k)
            for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++);
    }
 
    template<class T>
    inline void init(T s, const int len, const int sigma) {
        sais(s, sa, len, t, rk, ht, sigma), rk[0] = 0;
    }
};
 
SuffixArray<N> SA;


//Tire树
struct Trie {
  int nex[N * 32][2], idx;
  int cnt[N * 32];
  void insert(int x, int k) {
    int p = 0;
    for (int i = 31; i >= 0; i--) {
      int c = (x >> i) & 1;
      if (!nex[p][c]) nex[p][c] = ++idx;
      p = nex[p][c];
      cnt[p] += k;
    }
  }
 
  int find(int x) {
    int p = 0;
    int res = 0;
    for (int i = 31; i >= 0; i--) {
      int c = (x >> i) & 1;
      if (nex[p][c ^ 1] and cnt[nex[p][c ^ 1]]) {
        res += (1 << i);
        p = nex[p][c ^ 1];
      } else p = nex[p][c];
    }
    return res;
  }
} tr;

//Trie 指针版

class Trie {
private:
    int end=0;
    int ans=0;
    Trie* ne[26];
public:
    Trie() {
        end=0;
        memset(ne,0,sizeof ne);
    }
    void insert(string word,int k) {
        Trie* p = this;
        int m=word.size();
        for(auto c:word){
            int t=c-'a';
            if(p->ne[t]==nullptr) p->ne[t]=new Trie();
            p=p->ne[t];
            if(p->ans==0 or m<len[p->ans]) p->ans=k;
        }
        p->end=1;
    }
    int find(string word) {
        Trie* p = this;
        int res=-1;
        for(auto c:word){
            int t=c-'a';
            if(p->ne[t]==nullptr) break;
            p=p->ne[t];
            res=p->ans;
        }
        return res;
    }
};