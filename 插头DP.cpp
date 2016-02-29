//HDU4796 插头DP
//最小表示法, 相同连通块的插头用相同数字，独立插头为1，没有插头为0
#include <bits/stdc++.h>
#define ll long long
#define mst(a,x) memset(a,x,sizeof(a))
#define For(i, t) for(int i = 0; i < t; i++)
#define Debug(x) cerr << #x << " = "  << x << endl;
using namespace std;

const int M = 10;
const int N = 20;
const int MOD = 30007;
const int STATE = 2000005;
const int INF = 1e9 + 7;
char a[N + 5][M + 5];
//s表示轮廓线上插头，左->右，0开始标号
int n, m, s[M + 5], vis[M + 5], cost[N + 5][M + 5], cur;


//储存dp到当前格，插头为sta对应的dp值
//sta很大，用hash来存
struct HashMap{
	int fst[MOD], tot, nxt[STATE], dp[STATE];
	ll v[STATE];
	void init() {
		mst(fst, -1);
		tot = 0;
	}

	void push(ll sta, int cost) {
		bool Find = 0;
		int hashS = sta % MOD;

		for(int p = fst[hashS]; ~p; p = nxt[p]) {
			if(v[p] == sta) {
				dp[p] = min(dp[p], cost);
				Find = 1;
				break;
			}
		}
		if(!Find) {
			v[tot] = sta;
			dp[tot] = cost;
			nxt[tot] = fst[hashS];
			fst[hashS] = tot++;
		}
	}
}HM[2]; 

void sdebug() {
	puts("s");
	for(int t = 0; t <= m; t++) printf(" %d", s[t]);
	puts("");
}

void encode(int *s, ll &sta) {
	int Xor = 0, cnt = 2;

	For(i, m + 1) Xor ^= s[i];
	mst(vis, -1);
	vis[0] = 0;
	vis[Xor] = 1;

	sta = 0;
	For(i, m + 1) {
		if(vis[s[i]] == -1) vis[s[i]] = cnt++;
		s[i] = vis[s[i]];
		sta <<= 3;
		sta |= s[i];
	}
}

void decode(ll sta, int *s) {
	for(int i = m; ~i; i--) {
		s[i] = sta & 7;
		sta >>= 3;
	}
}

bool check(int i, int *s) {

	int cnt = 0;
	for(int j = 1; j <= m; j++) {
		if(s[j - 1]) cnt++;
		if(a[i][j] == 'W' && (cnt & 1)) return 0;
		if(a[i][j] == 'L' && (cnt & 1 ^ 1)) return 0;
	}
	return 1;
}

void inline push(int* s, int cost) {
	ll sta;
	
	encode(s, sta);
	HM[cur].push(sta, cost);
}



int main() {
	ll sta;

	while(~scanf("%d%d", &n, &m)) {
		for(int i = 1; i <= n; i++)	 scanf("%s", a[i] + 1);
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= m; j++) 
				cost[i][j] = a[i][j] >= '0' && a[i][j] <= '9' ? a[i][j] - '0' : INF;

		cur = 0;
		HM[cur].init();
		for(int i = 1; i <= m; i++) {
			mst(s, 0);
			s[i - 1] = 1;
			encode(s, sta);
			HM[cur].push(sta, 0);
		}
		for(int i = 1; i <= n; i++) 
			for(int j = 0; j <= m; j++) {
				cur ^= 1;
				HM[cur].init();
				For(x, MOD) 							//!别不小心写成tot
					for(int p = HM[cur ^ 1].fst[x]; ~p; p = HM[cur ^ 1].nxt[p]) {
						sta = HM[cur ^ 1].v[p];
						int pre = HM[cur ^ 1].dp[p];
						if(pre >= INF) continue;
						decode(sta, s);
						if(!j) {
							if(s[m]) continue;
							if(!check(i - 1, s)) continue;
							for(int t = m; t >= 1; t--) s[t] = s[t - 1];
							s[0] = 0;
							push(s, pre);
						}
						else {
							//  __        
							// |    -> __|  
							int L = s[j - 1];
							int U = s[j];
							//00 -> 00  xx
							if(!L && !U) {
								push(s, pre);
								s[j] = s[j - 1] = M;
								push(s, pre + cost[i][j]);
							}

							//0x x0 -> 0x x0
							if(!L && U || L && !U) {
								push(s, pre + cost[i][j]);
								swap(s[j - 1], s[j]);
								push(s, pre + cost[i][j]);
							}

							//12 -> 00 余下2变成0
							if(L == 1 && U > 1) {
								s[j] = s[j - 1] = 0;
								for(int t = 0; t <= m; t++) if(s[t] == U) s[t] = 1;
								push(s, pre + cost[i][j]);
							}
							//21 -> 00 ...
							if(L > 1 && U == 1) {
								s[j] = s[j - 1] = 0;
								for(int t = 0; t <= m; t++) if(s[t] == L) s[t] = 1;
								push(s, pre + cost[i][j]);
							}

							//23 -> 00, 余下23变成0
							if(L > 1 && U > 1) {
								if(L == U) continue;
								s[j] = s[j - 1] = 0;
								for(int t = 0; t <= m; t++) if(s[t] == L || s[t] == U) s[t] = M;
								push(s, pre + cost[i][j]);
							}
						}
				}
			}

		int ans = INF;
		For(x, MOD) 
			for(int p = HM[cur].fst[x]; ~p; p = HM[cur].nxt[p]) {
				sta = HM[cur].v[p];
				decode(sta, s);
				if(check(n, s) && !s[m] && (sta == (sta & (-sta)))) ans = min(ans, HM[cur].dp[p]);
			}
		if(ans == INF) ans = -1;
		printf("%d\n", ans);
	}
	return 0;
}
