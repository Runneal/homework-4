#include "../Queue.h"
#include "../Stack.h"
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //����״̬
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //���ڱ�����������������

template <typename Tv, typename Te> //�������͡�������
class Graph { //ͼGraphģ����
private:
    void reset() { //���ж��㡢�ߵĸ�����Ϣ��λ
        for ( int i = 0; i < n; i++ ) {//���ж����
            status ( i ) = UNDISCOVERED; dTime ( i ) = fTime ( i ) = -1; //״̬��ʱ���ǩ{dTime, fTime}
            parent ( i ) = -1; priority ( i ) = INT_MAX; //���ڱ������еģ����ڵ㣬���ȼ���
            for ( int j = 0; j < n; j++ ) //���бߵ�
                if ( exists ( i, j ) ) type ( i, j ) = UNDETERMINED; //����
        }
    }
public:
// ����
    int n; //��������
    virtual int insert ( Tv const& ) = 0; //���붥�㣬���ر��
    virtual Tv remove ( int ) = 0; //ɾ�����㼰������ߣ����ظö�����Ϣ
    virtual Tv& vertex ( int ) = 0; //����v�����ݣ��ö����ȷ���ڣ�
    virtual int inDegree ( int ) = 0; //����v����ȣ��ö����ȷ���ڣ�
    virtual int outDegree ( int ) = 0; //����v�ĳ��ȣ��ö����ȷ���ڣ�
    virtual int firstNbr ( int ) = 0; //����v���׸��ڽӶ���
    virtual int nextNbr ( int, int ) = 0; //����v�ģ�����ڶ���j�ģ���һ�ڽӶ���
    virtual VStatus& status ( int ) = 0; //����v��״̬
    virtual int& dTime ( int ) = 0; //����v��ʱ���ǩdTime
    virtual int& fTime ( int ) = 0; //����v��ʱ���ǩfTime
    virtual int& parent ( int ) = 0; //����v�ڱ������еĸ���
    virtual int& priority ( int ) = 0; //����v�ڱ������е����ȼ���
// �ߣ�����Լ��������߾�ͳһת��Ϊ�������һ������ߣ��Ӷ�������ͼ��������ͼ������
    int e; //������
    virtual bool exists ( int, int ) = 0; //��(v, u)�Ƿ����
    virtual void insert ( Te const&, int, int, int ) = 0; //�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
    virtual Te remove ( int, int ) = 0; //ɾ������v��u֮��ı�e�����ظñ���Ϣ
    virtual EType& type ( int, int ) = 0; //��(v, u)������
    virtual Te& edge ( int, int ) = 0; //��(v, u)�����ݣ��ñߵ�ȷ���ڣ�
    virtual int& weight ( int, int ) = 0; //��(v, u)��Ȩ��
// �㷨
    void bfs ( int ); //������������㷨
    void dfs ( int ); //������������㷨
    void bcc ( int ); //����DFS��˫��ͨ�����ֽ��㷨
    Stack<Tv>* tSort ( int ); //����DFS�����������㷨
    void BFS ( int, int& ); //����ͨ�򣩹�����������㷨
    void DFS ( int, int& ); //����ͨ��������������㷨
    void BCC ( int, int&, Stack<int>& ); //����ͨ�򣩻���DFS��˫��ͨ�����ֽ��㷨
    bool TSort ( int, int&, Stack<Tv>* ); //����ͨ�򣩻���DFS�����������㷨
    template <typename PU> void PFS ( int, PU ); //����ͨ�����ȼ��������
    void prim ( int ); //��С֧����Prim�㷨
    void dijkstra ( int ); //���·��Dijkstra�㷨
    template <typename PU> void pfs ( int, PU ); //���ȼ��������
};

// �����ڽӾ����ʵ��
template <typename Tv> class Vertex { //�������
private:

public:
    Tv data; int inDegree, outDegree; VStatus status; //���ݡ����������״̬
    int dTime, fTime; //ʱ���ǩ
    int parent; int priority; //�ڱ������еĸ��ڵ㡢���ȼ���
    Vertex ( Tv const& d = ( Tv ) 0 ) : //�����¶���
        data ( d ), inDegree ( 0 ), outDegree ( 0 ), status ( UNDISCOVERED ),
        dTime ( -1 ), fTime ( -1 ), parent ( -1 ), priority ( INT_MAX ) {} //�ݲ�����Ȩ�����
};

template <typename Te> class Edge { //�߶���
private:
    
public:
    Te data; int weight; EType type; //���ݡ�Ȩ�ء�����
    Edge ( Te const& d, int w ) : data ( d ), weight ( w ), type ( UNDETERMINED ) {} //����
};

template <typename Tv, typename Te> //�������͡�������
class GraphMatrix : public Graph<Tv, Te> { //�������������ڽӾ�����ʽʵ�ֵ�ͼ
private:
    Vector< Vertex< Tv > > V; //���㼯��������
    Vector< Vector< Edge< Te >* > > E; //�߼����ڽӾ���
public:
    int n ; //��������
    int e ; //������
    GraphMatrix() { n = e = 0; } //����
    ~GraphMatrix() { //����
        for ( int j = 0; j < n; j++ ) //���ж�̬������
            for ( int k = 0; k < n; k++ ) //�߼�¼
                delete E[j][k]; //�������
    }
// ����Ļ������� : ��ѯ��i������( 0 <= i < n )
    virtual Tv& vertex ( int i ) { return V[i].data; } //����
    virtual int inDegree ( int i ) { return V[i].inDegree; } //���
    virtual int outDegree ( int i ) { return V[i].outDegree; } //����
    virtual int firstNbr ( int i ) { return nextNbr ( i, n ); } //�׸��ڽӶ���
    virtual int nextNbr ( int i, int j ) //����ڶ���j����һ�ڽӶ��㣨�����ڽӱ�����Ч�ʣ�
    { while ( ( -1 < j ) && ( !exists ( i, --j ) ) ); return j; } //����������̽
    virtual VStatus& status ( int i ) { return V[i].status; } //״̬
    virtual int& dTime ( int i ) { return V[i].dTime; } //ʱ���ǩdTime
    virtual int& fTime ( int i ) { return V[i].fTime; } //ʱ���ǩfTime
    virtual int& parent ( int i ) { return V[i].parent; } //�ڱ������еĸ���
    virtual int& priority ( int i ) { return V[i].priority; } //�ڱ������е����ȼ���
// ����Ķ�̬����
    virtual int insert ( Tv const& vertex ) { //���붥�㣬���ر��
        for ( int j = 0; j < n; j++ ) E[j].insert ( NULL ); n++; //������Ԥ��һ��Ǳ�ڵĹ�����
        E.insert ( Vector<Edge<Te>*> ( n, n, ( Edge<Te>* ) NULL ) ); //�����¶����Ӧ�ı�����
        return V.insert ( Vertex<Tv> ( vertex ) ); //������������һ������
    }
    virtual Tv remove ( int i ) { //ɾ����i�����㼰�������( 0 <= i < n )
        for ( int j = 0; j < n; j++ )
            if ( exists ( i, j ) ) { delete E[i][j]; V[j].inDegree--; } //����ɾ�����г���
        E.remove ( i ); n--; //ɾ����i��
        Tv vBak = vertex ( i ); V.remove ( i ); //ɾ������i
        for ( int j = 0; j < n; j++ ) //�������
            if ( Edge<Te>* e = E[j].remove ( i ) ) { delete e; V[j].outDegree--; } //����ɾ��
        return vBak; //���ر�ɾ���������Ϣ
    }
// �ߵ�ȷ�ϲ���
    virtual bool exists ( int i, int j ) //��(i, j)�Ƿ����
    { return ( 0 <= i ) && ( i < n ) && ( 0 <= j ) && ( j < n ) && E[i][j] != NULL; }
// �ߵĻ������� : ��ѯ����i��j֮�������( 0 <= i, j < n��exists(i, j) )
    virtual EType& type ( int i, int j ) { return E[i][j]->type; } //��(i, j)������
    virtual Te& edge ( int i, int j ) { return E[i][j]->data; } //��(i, j)������
    virtual int& weight ( int i, int j ) { return E[i][j]->weight; } //��(i, j)��Ȩ��
// �ߵĶ�̬����
    virtual void insert ( Te const& edge, int w, int i, int j ) { //����Ȩ��Ϊw�ı�e = (i, j)
        if ( exists ( i, j ) ) return; //ȷ���ñ��в�����
        E[i][j] = new Edge<Te> ( edge, w ); //�����±�
        e++; V[i].outDegree++; V[j].inDegree++; //���±߼������������Ķ���
    }
    virtual Te remove ( int i, int j ) { //ɾ������i��j֮�������( exists(i, j) )
        Te eBak = edge ( i, j ); delete E[i][j]; E[i][j] = NULL; //���ݺ�ɾ���߼�¼
        e--; V[i].outDegree--; V[j].inDegree--; //���±߼������������Ķ���
        return eBak; //���ر�ɾ���ߵ���Ϣ
    }
};

template <typename Tv, typename Te> //�����������BFS�㷨��ȫͼ��
void Graph<Tv, Te>::bfs (int s) { //����0 <= s < n
    reset(); int clock = 0; int v = s; //��ʼ��
    do //��һ������ж���
        if (UNDISCOVERED == status(v)) //һ��������δ���ֵĶ���
            BFS(v, clock); //���Ӹö����������һ��BFS
    while (s != (v = (++v % n))); //����ż�飬�ʲ�©����
}

template <typename Tv, typename Te> //�����������BFS�㷨��������ͨ��
void Graph<Tv, Te>::BFS (int v, int& clock) { //����0 <= v < n
    Queue<int> Q; //���븨������
    status(v) = DISCOVERED; Q.enqueue(v); //��ʼ�����
    while (!Q.isEmpty()) { //��Q���֮ǰ������
        int v = Q.dequeue(); dTime(v) = ++clock; //ȡ�����׶���v�����ڴ˸���ʱ���ǩ
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //ö��v�������ھ�u
            if (UNDISCOVERED == status(u)) { //��u��δ�����֣���
                status(u) = DISCOVERED; Q.enqueue(u); //���ָö���
                type(v, u) = TREE; parent(u) = v; //����������չ֧����
            } else { //��u�ѱ����֣����������ѷ�����ϣ���
                type(v, u) = CROSS; //��(v, u)�����ڿ��
            }
        status(v) = VISITED; //���ˣ���ǰ����������
    }
}

template <typename Tv, typename Te> //�����������DFS�㷨��ȫͼ��
void Graph<Tv, Te>::dfs ( int s ) {
    reset(); int clock = 0; int v = s; //��ʼ��
    do //��һ������ж���
        if ( UNDISCOVERED == status ( v ) ) //һ��������δ���ֵĶ���
            DFS ( v, clock ); //���Ӹö����������һ��DFS
    while ( s != ( v = ( ++v % n ) ) ); //����ż�飬�ʲ�©����
}

template <typename Tv, typename Te> //�����������DFS�㷨��������ͨ��
void Graph<Tv, Te>::DFS ( int v, int& clock ) { //����0 <= s < n
    dTime ( v ) = ++clock; status ( v ) = DISCOVERED; //���ֶ���v
    for ( int u = firstNbr ( v ); -1 < u; u = nextNbr ( v, u ) ) //ö��v�������ھ�u
        switch ( status ( u ) ) //������״̬�ֱ���
        {
        case UNDISCOVERED: //u��δ�����֣���ζ��֧�������ڴ���չ
            type ( v, u ) = TREE; parent ( u ) = v; DFS ( u, clock ); break;
            break;
        case DISCOVERED: //u�ѱ����ֵ���δ������ϣ�Ӧ�������ָ�������
            type ( v, u ) = BACKWARD; break;
        default: //u�ѷ�����ϣ�VISITED������ͼ�������ӳ�Ϯ��ϵ��Ϊǰ��߻���
            type ( v, u ) = ( dTime ( v ) < dTime ( u ) ) ? FORWARD : CROSS; break;
        }
    status ( v ) = VISITED; fTime ( v ) = ++clock; //����v�������ھӶ��������
}

template <typename Tv, typename Te> //����DFS�����������㷨
Stack<Tv>* Graph<Tv, Te>::tSort ( int s ) {
    reset(); int clock = 0; int v = s; //��ʼ��
    Stack<Tv>* S = new Stack<Tv>; //��ջ��¼���򶥵�
    do {
        if ( UNDISCOVERED == status (v))
            if ( !TSort ( v, clock, S ) ) { //clock���Ǳ���
            while ( !S->empty() ) //��һ��ͨ���༴��ͼ����DAG
            S->pop();  break; //�򲻱ؼ������㣬��ֱ�ӷ���
        }
    } while ( s != ( v = ( ++v % n ) ) ); //����ż�飬�ʲ�©����
    return S; //������ΪDAG����S�ڸ������Զ�������򣻷��򣨲������������򣩣�S��
}

template <typename Tv, typename Te> //����DFS�����������㷨(����)
bool Graph<Tv, Te>::TSort ( int v, int& clock, Stack<Tv>* S ) { //����0 <= v < n
    dTime ( v ) = ++clock; status ( v ) = DISCOVERED; //���ֶ���v
    for ( int u = firstNbr ( v ); -1 < u; u = nextNbr ( v, u ) ) //ö��v�������ھ�u
        switch ( status ( u ) ) { //������״̬�ֱ���
            case UNDISCOVERED:
                parent ( u ) = v; type ( v, u ) = TREE;
                if ( !TSort ( u, clock, S ) ) //�Ӷ���u��ʼ���������������
                    return false; //һ�����ֺ���ߣ��򷵻ز�����
                break;
            case DISCOVERED:
                type ( v, u ) = BACKWARD;
                return false; //һ�����ֺ���ߣ��򷵻ز�����
            default:
                type ( v, u ) = ( dTime ( v ) < dTime ( u ) ) ? FORWARD : CROSS; break;
        }
    status ( v ) = VISITED; S->push ( vertex ( v ) ); //���㱻���ΪVISITEDʱ���漴��ջ
    return true; //v���������Թ���һ��DFS�������سɹ���־    
}

template <typename Tv, typename Te> //����DFS��˫��ͨ�����ֽ��㷨
void Graph<Tv, Te>::bcc ( int s ) {
    reset(); int clock = 0; int v = s; Stack<int> S; //ջS���Լ�¼�ѷ��ʵĶ���
    do
        if ( UNDISCOVERED == status ( v ) ) {
            BCC ( v, clock, S );
            S.pop();
        }
    while ( s != ( v = ( ++v % n ) ) );
}
#define hca(x) (fTime(x))//����fTime[]�䵱hca[]
template <typename Tv, typename Te> //����DFS��˫��ͨ�����ֽ��㷨
void Graph<Tv, Te>::BCC ( int v, int& clock, Stack<int>& S ) {
    hca ( v ) = dTime ( v ) = ++clock; status ( v ) = DISCOVERED; S.push ( v ); //v�����ֲ���ջ
    for ( int u = firstNbr ( v ); -1 < u; u = nextNbr ( v, u ) )
        switch ( status ( u ) ) {
            case UNDISCOVERED:
                parent ( u ) = v; type ( v, u ) = TREE; BCC ( u, clock, S );
                if ( hca ( u ) < dTime ( v ) ) //�������غ�������u��ͨ������ߣ���ָ��v��������
                    hca ( v ) = min ( hca ( v ), hca ( u ) ); //��v������
                else { //������vΪ�ؽڵ㣨u���¼���һ��BCC�������ж����ʱ��������ջS�Ķ�����
                    while ( u != S.pop() ); S.push ( v ); //������ǰBCC�е����нڵ㣬��vΪ��
                }
                break;
            case DISCOVERED:
                type ( v, u ) = BACKWARD; if ( u != parent ( v ) ) hca ( v ) = min ( hca ( v ), dTime ( u ) ); //�ô�ǰ��dTime[u]����hca[v](ע��˴���DFS_TS�Ĳ�ͬ)
                break;
            default:
                type ( v, u ) = ( dTime ( v ) < dTime ( u ) ) ? FORWARD : CROSS; break;
        }
    status ( v ) = VISITED; //���ˣ���ǰ����v����������
}
#undef hca //�ͷ���ʱ����

template <typename Tv, typename Te> template <typename PU> //���ȼ��������
void Graph<Tv, Te>::pfs ( int s, PU prioUpdater ) {
    reset(); int v = s; //��ʼ��
    do
        if ( UNDISCOVERED == status ( v ) )
            PFS ( v, prioUpdater ); //���������
    while ( s != ( v = ( ++v % n ) ) );
}

template <typename Tv, typename Te> template <typename PU> //���ȼ��������
void Graph<Tv, Te>::PFS ( int s, PU prioUpdater ) {
    priority ( s ) = 0; status ( s ) = VISITED; parent ( s ) = -1; //��ʼ�������s����PFS����
    while ( 1 ) {
        for ( int w = firstNbr ( s ); -1 < w; w = nextNbr ( s, w ) ) //ö��s�������ھ�w
            prioUpdater ( this, s, w ); //���¶���w�����ȼ����丸����
        for ( int shortest = INT_MAX, w = 0; w < n; w++ ) //����δ����������Ķ�����
            if ( UNDISCOVERED == status ( w ) ) //ѡ����һ��
                if ( shortest > priority ( w ) ) //���ȼ���ߵĶ���s
                    shortest = priority ( w ), s = w; //���估�丸����ȷ��
        if ( VISITED == status ( s ) ) break; //ֱ�����ж�����Ѽ���
        {
            status ( s ) = VISITED; type ( parent ( s ), s ) = TREE; //��s���丸�ڵ���������
        }
    }
}

template <typename Tv, typename Te> class PrimPU { //���Prim�㷨�Ķ������ȼ�������
public:
    virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) //����g->status(v) == VISITED
    {
        if ( UNDISCOVERED == g->status ( v ) ) //����ukÿһ��δ�����ֵ��ڽӶ���v
            if ( g->priority ( v ) > g->weight ( uk, v ) ) { //��Prim�������ɳ�
                g->priority ( v ) = g->weight ( uk, v ); //�������ȼ�������
                g->parent ( v ) = uk; //���¸��ڵ�
            }
    }
};

template <typename Tv, typename Te> class DijkstraPU { //���Dijkstra�㷨�Ķ������ȼ�������
public:
    virtual void operator() ( Graph<Tv, Te>* g, int uk, int v ) {
        if ( UNDISCOVERED == g->status ( v ) ) //����ukÿһ��δ�����ֵ��ڽӶ���v
            if ( g->priority ( v ) > g->priority ( uk ) + g->weight ( uk, v ) ) { //��Dijkstra����
                g->priority ( v ) = g->priority ( uk ) + g->weight ( uk, v ); //���ɳ�
                g->parent ( v ) = uk; //���¸��ڵ�
            }
    }
};