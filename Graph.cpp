//最短路dijkstra

vector<int> dijkstra(int S){
	vector<int> dist(n+1,-1);
	vector<bool> st(n+1,0);
	priority_queue<array<int,2>,vector<array<int,2>>,greater<array<int,2>>> q;
	q.push({0,S});dist[S]=0;
	while(q.size()){
		auto t=q.top();
		q.pop();
		if(st[t[1]]) continue;
		st[t[1]]=1;
		for(auto x:eg[t[1]]){
			if(dist[x]==-1 or dist[x]>t[0]+1){
				dist[x]=t[0]+1;
				q.push({dist[x],x});
			}
		}
	}
	return dist;
}


//次短路dijkstra

void dijkstra2(int u) {
  for (int i = 0; i < N; i++) dis1[i] = dis2[i] = 1e18;
  priority_queue<PII, vector<PII>, greater<PII>> q;
  q.push({0, u});
  dis1[u] = 0;
  while (!q.empty()) {
    auto t = q.top();
    q.pop();
    if (dis2[t.second] < t.first) continue;
    for (auto [w, j] : eg[t.second]) {
      int nw = t.first + w;
      if (nw < dis1[j]) {
        swap(dis1[j], nw);
        q.push({dis1[j], j});
      }
      if (nw < dis2[j]) {
        swap(dis2[j], nw);
        q.push({dis2[j], j});
      }
    }
  }
}

//匈牙利

#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

const int N = 510, M = 100010;

int n1, n2, m;
int h[N], e[M], ne[M], idx;
int match[N];
bool st[N];

bool find(int x) {
  for (int i = h[x]; i != -1; i = ne[i]) {
    int j = e[i];
    if (!st[j]) {
      st[j] = true;
      if (match[j] == 0 || find(match[j])) {
        match[j] = x;
        return true;
      }
    }
  }
  return false;
}
void add(int a, int b) { e[idx] = b, ne[idx] = h[a], h[a] = idx++; }

int main() {
  cin >> n1 >> n2 >> m;
  memset(h, -1, sizeof h);
  while (m--) {
    int a, b;
    cin >> a >> b;
    add(a, b);
  }
  int res = 0;
  for (int i = 1; i <= n1; i++) {
    memset(st, false, sizeof st);
    if (find(i)) res++;
  }

  cout << res;

  return 0;
}

//最小生成树（kruskal）

#include <algorithm>
#include <iostream>

using namespace std;

const int N = 200010;

int n, m;
int p[N];

struct Edge {
  int a, b, w;

  bool operator<(const Edge &x) const { return w < x.w; }
} edges[N];

int find(int x) {
  if (p[x] != x) p[x] = find(p[x]);
  return p[x];
}

int main() {
  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    int a, b, w;
    cin >> a >> b >> w;
    edges[i] = {a, b, w};
  }

  sort(edges, edges + m);

  for (int i = 1; i <= n; i++) p[i] = i;

  int res = 0, cnt = 0;

  for (int i = 0; i < m; i++) {
    int a = edges[i].a, b = edges[i].b, w = edges[i].w;

    a = find(a), b = find(b);

    if (a != b) {
      p[a] = b;
      cnt++;
      res += w;
    }
  }
  if (cnt < n - 1)
    cout << "impossible";
  else
    cout << res;
}

//倍增LCA（最近公共祖先）

vector<int> eg[N];
int dep[N],fa[N][20];
 
void dfs(int u, int f) {
  	for (int i = 1; i < 20; i++) {
    	fa[u][i] = fa[fa[u][i - 1]][i - 1];
  	}
  	for (auto j:eg[u]) {
    	if (j == f) continue;
    	fa[j][0] = u;
    	dep[j] = dep[u] + 1;
    	dfs(j, u);
  	}
}
 
int lca(int a, int b) {
  	if (dep[a] < dep[b]) swap(a, b);
  	for (int i = 19; i >= 0; i--) {
    	if (dep[a] - (1 << i) >= dep[b]) a = fa[a][i];
  	}
  	if (a == b) return a;
  	for (int i = 19; i >= 0; i--) {
    	if (fa[a][i] != fa[b][i]) {
      		a = fa[a][i];
      		b = fa[b][i];
    	}
  	}
  	return fa[a][0];
}

//树链剖分，修改路径，修改子树
#include <bits/stdc++.h>
#define fastio             \
  ios::sync_with_stdio(0); \
  cin.tie(0);              \
  cout.tie(0)
#pragma GCC optimize(2)
#define endl '\n'
//#define x first
//#define y second

using namespace std;
typedef pair<int, int> PII;
typedef long long LL;

const int N = 100010;

int n, m;
int a[N];
vector<int> eg[N];
int dep[N], son[N], fa[N], sz[N];
int dfn[N], cnt, w[N], id[N], top[N];

void dfs1(int u, int f, int d) {
  dep[u] = d;
  sz[u] = 1;
  fa[u] = f;
  for (auto x : eg[u]) {
    if (x == f) continue;
    dfs1(x, u, d + 1);
    sz[u] += sz[x];
    if (sz[x] > sz[son[u]]) son[u] = x;
  }
}
void dfs2(int u, int f) {
  dfn[u] = ++cnt, w[cnt] = a[u];
  top[u] = f;
  if (!son[u]) return;
  dfs2(son[u], f);
  for (auto x : eg[u]) {
    if (x == fa[u] or x == son[u]) continue;
    dfs2(x, x);
  }
}

struct Node {
  int l, r;
  LL sum, add;
} tr[N * 4];
void pushup(Node &u, Node &l, Node &r) { u.sum = l.sum + r.sum; }
void pushup(int u) { pushup(tr[u], tr[u << 1], tr[u << 1 | 1]); }
void build(int u, int l, int r) {
  tr[u] = {l, r, 0, 0};
  if (l == r)
    tr[u] = {l, r, w[l], 0};
  else {
    int mid = (l + r) / 2;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    pushup(u);
  }
}
void pushdown(int u) {
  if (tr[u].add) {
    tr[u << 1].sum += (tr[u << 1].r - tr[u << 1].l + 1) * tr[u].add,
        tr[u << 1].add += tr[u].add;
    tr[u << 1 | 1].sum += (tr[u << 1 | 1].r - tr[u << 1 | 1].l + 1) * tr[u].add,
        tr[u << 1 | 1].add += tr[u].add;
    tr[u].add = 0;
  }
}
void modify(int u, int l, int r, int d) {
  if (tr[u].l >= l and tr[u].r <= r) {
    tr[u].sum += (tr[u].r - tr[u].l + 1) * d;
    tr[u].add += d;
    return;
  }
  pushdown(u);
  int mid = (tr[u].l + tr[u].r) / 2;
  if (l <= mid) modify(u << 1, l, r, d);
  if (r > mid) modify(u << 1 | 1, l, r, d);
  pushup(u);
}
Node query(int u, int l, int r) {
  if (tr[u].l >= l and tr[u].r <= r) return tr[u];
  pushdown(u);
  int mid = (tr[u].r + tr[u].l) / 2;
  if (l > mid)
    return query(u << 1 | 1, l, r);
  else if (r <= mid)
    return query(u << 1, l, r);
  else {
    Node ll = query(u << 1, l, r), rr = query(u << 1 | 1, l, r);
    Node res;
    pushup(res, ll, rr);
    return res;
  }
}

void modify_tree(int u, int k) { modify(1, dfn[u], dfn[u] + sz[u] - 1, k); }
LL query_tree(int u) { return query(1, dfn[u], dfn[u] + sz[u] - 1).sum; }
void modify_path(int u, int v, int k) {
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    modify(1, dfn[top[u]], dfn[u], k);
    u = fa[top[u]];
  }
  if (dep[u] < dep[v]) swap(u, v);
  modify(1, dfn[v], dfn[u], k);
}
LL query_path(int u, int v) {
  LL res = 0;
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    res += query(1, dfn[top[u]], dfn[u]).sum;
    u = fa[top[u]];
  }
  if (dep[u] < dep[v]) swap(u, v);
  res += query(1, dfn[v], dfn[u]).sum;
  return res;
}

void solve() {
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> a[i];
  for (int i = 1; i < n; i++) {
    int l, r;
    cin >> l >> r;
    eg[l].emplace_back(r);
    eg[r].emplace_back(l);
  }
  dfs1(1, -1, 1);
  dfs2(1, 1);
  build(1, 1, n);

  cin >> m;
  while (m--) {
    int x;
    cin >> x;
    if (x == 1) {
      int u, v, k;
      cin >> u >> v >> k;
      modify_path(u, v, k);
    } else if (x == 2) {
      int u, k;
      cin >> u >> k;
      modify_tree(u, k);
    } else if (x == 3) {
      int u, v;
      cin >> u >> v;
      cout << query_path(u, v) << endl;
    } else {
      int u;
      cin >> u;
      cout << query_tree(u) << endl;
    }
  }
}

signed main() {
  fastio;

  int T;
  T = 1;
  while (T--) solve();

  return 0;
}

//最大流:节点编号(1-n)

struct MF {
  int h[N], e[M], ne[M], w[M], idx = 0;
  int vis[N];
  
  int n, S, T;
  LL maxflow = 0;
  int dep[N], cur[N];

  void init() {
    memset(h, -1, sizeof h);
    idx = 0;
  }

  void add(int a, int b, int c) {
    e[idx] = b, ne[idx] = h[a], w[idx] = c, h[a] = idx++;
    e[idx] = a, ne[idx] = h[b], w[idx] = 0, h[b] = idx++;
  }

  bool bfs() {
    queue<int> q;
    memset(dep, -1, sizeof dep);
    dep[S] = 0;
    q.push(S);
    cur[S] = h[S];
    while (q.size()) {
      int u = q.front();
      q.pop();
      for (int i = h[u]; ~i; i = ne[i]) {
        int j = e[i];
        if (dep[j] == -1 and w[i]) {
          dep[j] = dep[u] + 1;
          cur[j] = h[j];
          if (j == T) return 1;
          q.push(j);
        }
      }
    }
    return 0;
  }

  int dfs(int u, int limit) {
    if (u == T or !limit) return limit;
    int flow = 0;
    for (int i = cur[u]; ~i and flow < limit; i = ne[i]) {
      cur[u] = i;
      int j = e[i];
      if (dep[j] == dep[u] + 1 and w[i]) {
        int t = dfs(j, min(limit - flow, w[i]));
        if (!t) dep[j] = -1;
        w[i] -= t;
        w[i ^ 1] += t;
        flow += t;
      }
    }
    return flow;
  }

  void dinic() {
    while (bfs()) {
      // memcpy(cur, h, sizeof h);
      maxflow += dfs(S, INF);
    }
  }
  void dfs(int u) {//找与源点同一侧的点
	  vis[u] = 1;
	  for (int i = h[u]; ~i; i = ne[i]) {
	    int v = e[i];
	    if (!vis[v] && w[i]) dfs(v);
	  }
  }
} mf;

//最小费用最大流:节点编号(1-n)

namespace dinic{
	const int N=1e5+7,M=2e6+7;
	const int INF=1e9;
	int n,S,T;
	int head[N],ver[M],nex[M],tot,cur[N];
	int dist[N],edge[M],cost[M],maxflow,mincost;
	bool vis[N];
	
	inline void add(int x,int y,int z,int c,bool o=1){
		ver[tot]=y;
		edge[tot]=z;
		cost[tot]=c;
		nex[tot]=head[x];
		head[x]=tot++;
		if(o) add(y,x,0,-c,0);
	}
	inline bool spfa(){
		for(int i=1;i<=n;i++) dist[i]=INF;
		memset(vis,0,sizeof vis);
		queue<int> q;
		q.push(S);
		dist[S]=0,vis[S]=1;
		while(q.size()){
			auto x=q.front();
			q.pop();
			vis[x]=0;
			for(int i=head[x];~i;i=nex[i]){
				int y=ver[i];
				int z=edge[i],c=cost[i];
				if(dist[y]<=dist[x]+c or !z) continue;
				dist[y]=dist[x]+c;
				if(!vis[y]) q.push(y),vis[y]=1;
			}
		}
		return dist[T]!=INF;
	}
	int dfs(int x,int flow=INF){
		if(x==T) return flow;
		int ans=0,k,i;
		vis[x]=1;
		for(int i=cur[x];~i and flow;i=nex[i]){
			cur[x]=i;
			int y=ver[i];
			int z=edge[i],c=cost[i];
			if(!z or (dist[y]!=dist[x]+c) or vis[y]) continue;
			k=dfs(y,min(flow,z));
			if(!k) dist[y]=INF;
			edge[i]-=k;
			edge[i^1]+=k;
			ans+=k;
			mincost+=k*c;
			flow-=k;
		}
		vis[x]=0;
		return ans;
	}
	inline void main(){
		while(spfa()){
			for(int i=1;i<=n;i++){
				cur[i]=head[i];
			}
			int now;
			while((now=dfs(S,INF))) maxflow+=now;
		}
	}
	inline void init(int _n,int _S,int _T){
		n=_n,S=_S,T=_T;
		tot=0,maxflow=0,mincost=0;
		memset(head,-1,sizeof head);
	}
}

// Tarjan求边双连通分量E_DCC

struct E_DCC{
	int n,cnt,cnt_edge;
	int dfn[N], low[N];
	vector<pair<int, int>> e[N];
	vector<vector<int>> ans;
	stack<int> st;
	vector<array<int,2>> bridge;
    int is_bridge[N];
	
	void init(int nn){
		n=nn,cnt_edge=cnt=0;
		st=stack<int>();
		ans.clear();bridge.clear();
		for(int i=1;i<=n;i++) {
			e[i].clear();
			dfn[i]=low[i]=0;
			is_bridge[i]=0;
		}
	}
	
	void add(int u,int v){
		cnt_edge++;
		e[u].push_back(make_pair(v, cnt_edge<<1));
		e[v].push_back(make_pair(u, cnt_edge<<1|1));
	}
	
	void dfs(int x, int las){
		low[x] = dfn[x] = ++cnt;
		st.push(x);
		for (auto i: e[x]){
			if (i.second == (las ^ 1)) continue;
			if (!dfn[i.first]){
				dfs(i.first, i.second);
				low[x] = min(low[x], low[i.first]);
				if(low[i.first]>dfn[x]){
                    bridge.push_back({x,i.first});
                    is_bridge[x]=1;
                    is_bridge[i.first]=1;
                }
			}else low[x] = min(low[x], dfn[i.first]);
		}
		if (dfn[x] == low[x]){
			vector<int> vec;
			vec.push_back(x);
			while (st.top() != x){
				vec.push_back(st.top());
				st.pop();
			}
			st.pop();
			ans.push_back(vec);
		}
	}
	
	void tarjan(){
		for(int i=1;i<=n;i++) 
			if(!dfn[i]) dfs(i,0);
	}
}tj;

// Tarjan求点双连通分量V_DCC

struct V_DCC{
    vector<int> eg[N];
    int dfn[N],low[N],timespace;
    int stk[N],top;
    bool cut[N];
    int dcc_cnt,n;
    vector<int> dcc[N];
    void init(int x){
        timespace=1;
        n=x;
    }
    void add(int a,int b){
        eg[a].push_back(b);
        eg[b].push_back(a);
    }
    void dfs(int u,int root){
        dfn[u]=low[u]=timespace++;
        stk[++top]=u;
        
        if(root==u and eg[u].empty()){
            dcc_cnt++;
            dcc[dcc_cnt].push_back(u);
            return;
        }
        int cnt=0;
        for(auto j:eg[u]){
            if(!dfn[j]){
                dfs(j,root);
                low[u]=min(low[u],low[j]);
                if(dfn[u] <= low[j]){
                    cnt++;
                    if(u!=root or cnt > 1) cut[u]=1;
                    dcc_cnt++;
                    int y;
                    do{
                        y=stk[top--];
                        dcc[dcc_cnt].push_back(y);
                    }while(y!=j);
                    dcc[dcc_cnt].push_back(u);
                }
            }
            else low[u]=min(low[u],dfn[j]);
        }
    }
    void tarjan(){
        for(int i=1;i<=n;i++){
            if(!dfn[i]) dfs(i,i);
        }
    }
}tj;

// Tarjan求有向图强连通分量SCC

struct Tarjan{
    vector<int> eg[N];
    int dfn[N],low[N],timespace;
    int stk[N],top;
    bool st[N];
    int color[N];
    int scc_cnt,n;
    vector<int> scc[N];
    void init(int x){
        timespace=1;scc_cnt=0;
        top=0;
        n=x;
        for(int i=1;i<=n;i++) eg[i].clear(),scc[i].clear();
        for(int i=1;i<=n;i++){
        	dfn[i]=0,low[i]=0;
        	color[i]=0;
        }
    }
    void add(int a,int b){
        eg[a].push_back(b);
    }
    void dfs(int u){
		low[u]=dfn[u]=++timespace;
		stk[++top]=u;
		st[u]=1;
		for(auto v:eg[u]){
			if (!dfn[v]){
				dfs(v);
				low[u]=min(low[u],low[v]);
			}else if(st[v]) low[u]=min(low[u],low[v]);
		}
		if(dfn[u]==low[u]){
			scc_cnt++;
			int y;
			while (y=stk[top--])
			{
				scc[scc_cnt].push_back(y);
				color[y]=scc_cnt;
				st[y]=0;
				if (u==y) break;
			}
		}
	}
    void tarjan(){
        for(int i=1;i<=n;i++){
            if(!dfn[i]) dfs(i);
        }
    }
}tj;


//线段树优化建图区间连边
struct segment_tree_graph{
	int cnt=0;
	int root_in,root_out;
	int idx[N];//i点对应in_tree编号
	int tr[N*20][2];
	vector<PII> eg[N*20];
	
	PII build(int l,int r){
		int rt_in=++cnt;
		int rt_out=++cnt;
		if(l==r){
			if(l==s) S=rt_out;
			idx[l]=rt_in;
			eg[rt_out].pb(rt_in,0);
			eg[rt_in].pb(rt_out,0);
			return {rt_in,rt_out};
		}
		int mid=(l+r)/2;
		auto t=build(l,mid);
		tr[rt_in][0]=(t.first),tr[rt_out][0]=(t.second);
		eg[rt_in].pb(t.first,0);
		eg[t.second].pb(rt_out,0);
		t=build(mid+1,r);
		tr[rt_in][1]=(t.first),tr[rt_out][1]=(t.second);
		eg[rt_in].pb(t.first,0);
		eg[t.second].pb(rt_out,0);
		root_in=rt_in,root_out=rt_out;
		return {rt_in,rt_out};
	}
	
	void add(int u,int l,int r,int pl,int pr,int x,int w,int flag){
		if(l>=pl and r<=pr){
			if(flag) eg[u].pb(x,0);
			else eg[x].pb(u,w);
			return;
		}
		int mid=(l+r)/2;
		if(pl<=mid) add(tr[u][0],l,mid,pl,pr,x,w,flag);
		if(pr>mid) add(tr[u][1],mid+1,r,pl,pr,x,w,flag);
	}
	
	void modify(int l1,int r1,int l2,int r2,int w){
		int nt=++cnt;
		add(root_out,1,n,l1,r1,nt,w,1);
		add(root_in,1,n,l2,r2,nt,w,0);
	}
	void clear(){
		for(int i=0;i<=cnt;i++){
			tr[i][0]=tr[i][1]=0;
			idx[i]=0;
			eg[i].clear();
		}
		cnt=0;
	}
}tr;

//虚树dp

struct virtual_tree{
	vector<vector<int>> veg;
	vector<int> f,st;
	vector<int> stk;
	int top;
	void init(int n){
		veg.resize(n+1);
		f.resize(n+1,0);
		st.resize(n+1,0);
		top=0;
	}
	void clear(){
		stk.clear();top=0;
		function<void(int,int)> dclear=[&](int u,int la){
			for(auto x:veg[u]) if(x!=la) dclear(x,u);
			veg[u].clear();
			f[u]=0;st[u]=0;
		};
		dclear(1,0);
	}
	void add_edge(int a,int b){
		veg[a].push_back(b);
	}
	void build(vector<int> &q){
		sort(all(q),[&](int u,int v){
			return dfn[u]<dfn[v];
		});
		stk.push_back(0);
		stk.push_back(1),top++;
		for(auto x:q){
			st[x]=1;
			if(x==1) continue;
			int t=lca(x,stk[top]);
			if(t!=stk[top]){
				while(dfn[t]<dfn[stk[top-1]]){
					add_edge(stk[top-1],stk[top]);
					top--;stk.pop_back();
				}
				if(dfn[t]>dfn[stk[top-1]]){
					add_edge(t,stk[top]);
					stk.pop_back();
					stk.push_back(t);
				}else{
					add_edge(t,stk[top]);
					top--;stk.pop_back();
				}
			}
			top++;stk.push_back(x);
		}
		while(stk.size()>=2){
			auto t=stk.back();
			stk.pop_back();
			if(stk.back()==0) break;
			else add_edge(stk.back(),t);
		}
	}


	int dp(int u,int la){

	}
}vtr;