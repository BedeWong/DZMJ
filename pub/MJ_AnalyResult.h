#ifndef MJ_ANALYRESULT_H
#define MJ_ANALYRESULT_H

#define DEBUG

class MJC_CardSet;
class MJ_AnalyResult
{
    friend class MJC_CardSet;
public:
    typedef char CARD;

public:
    MJ_AnalyResult();

    ~MJ_AnalyResult();

    int canHu(CARD *);

private:
    int st_count;
    int sl_count;
    int dz_count;
    int el_count;
    int kz_count;
    int dan_count;
    int w_count;

    CARD _w;
    CARD st[4];
    CARD sl[4];
    CARD dz[8];
    CARD el[8];
    CARD kz[8];
    CARD dan[8];

#ifdef DEBUG
public:
    void printResult();
#endif
};

#define forin_dz(dz, Count, into, cur, w) do{ \
    for(auto i=0; i<(Count); i++) \
    {\
        ((dz)[i]!=(w) && ((into)[(cur)++] = (dz)[i])) || ((into)[(cur)++] = (w)); \
    }}while(0);

#define forin_el(el, Count, into, cur, w) do{ \
    for(auto i=0; i<(Count); i++) \
    {\
        if(isMJCARD(el[i] + 1)) \
            (((el)[i]+1) != w && ((into)[(cur)++] = (el)[i]+1)) || (((into)[(cur)++] = w)); \
        if(isMJCARD(el[i] - 2)) \
            ((((el)[i]-2) != w) && ((into)[(cur)++] = (el)[i]-2)) || ((into)[(cur)++] = w); \
    }}while(0);

#define forin_kz(kz, Count, into, cur, w) forin_dz(kz, Count, into, cur, w)
#define forin_dan(dan, Count, into, cur, w) forin_dz(dan, Count, into, cur, w)

#endif // MJ_ANALYRESULT_H
