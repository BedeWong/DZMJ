#include "MJ_AnalyResult.h"
#include "MJ_Player.h"
#include <cstring>
#include <QDebug>

#ifdef DEBUG
#include <iostream>
#endif

const char *MJ_AnalyResult::HuNames[32] = {
    "平胡 1番",
    "抢杠 1番",
    "杠上开花 1番",

    "自摸 2番",
    "门前清 2番",
    "小道 2番",
    "有一套 2番",
    "巧七对 2番",
    "十三浪 2番",
    "一条龙 2番",
    "混一色 2番",
    "碰碰胡 2番",
    "四归一 2番",
    "全求一 2番",
    "单王抓 2番",
    "王归位 2番",

    "大道 4番",
    "有两套 4番",
    "七风会 4番",
    "双王归位 4番",
    "单王抓王 4番",
    "双王抓 4番",
    "清一色 4番",

    "双王抓王 8番",
    "三王归位 8番",

    "四王归位 16番",
    "",
    0,
    0,
    0
};

MJ_AnalyResult::MJ_AnalyResult()
{
    memset(this, 0, sizeof(MJ_AnalyResult));
}

MJ_AnalyResult::MJ_AnalyResult(MJ_Player &of)
{
    memset(this, 0, sizeof(MJ_AnalyResult));

    of.copy_chiList(this);
    of.copy_pengList(this);
    of.copy_gangList(this);
}

MJ_AnalyResult::~MJ_AnalyResult()
{

}

int MJ_AnalyResult::canHu(MJ_Player::CARD *ting)
{
    if(ting == NULL)
        throw(0);

    int tCount = 0;
    /***  *****  *****  *****   *****
     *      按王的个数分类， 计算出可以听的牌有哪些
     *      计算王抓的
     * *****
     *   0: (dz>=1 && dan==0) || dan==1
     *   1: kz+dz+el==0&&dan==0  ||                  dan==0  (单王抓)
     *      dan==1&&kz+dz+el==1
     *   2: dz+el+kz==1 && dan==0 ||                 dz+el+kz==1 &&dan=0(单王抓)
     *      dz+el+kz == 2 && dan=1 ||
     *      dz>1 && dan==0   ||
     *      dz==1 && dan==0        -dz
     *   3: dan==1 && dz+el+kz==0 ||                 双王抓
     *      dan==0&&kz+dz+el==2 ||                        (单王抓)
     *      kz+el+dz==1 && dan==2                         kz+dz+el == 2&&dan==0 (单王抓)
     *      kz+dz+el==3 && dan==1
     *   4: dan==0 && dz+el+kz==0         三王归位
     *      kz+dz+el==1 && dan==1         双王抓
     *      kz+dz+el==0 && dan==3
     *      kz+dz+el==3 && dan==0          单王抓
     *      dz+el+kz==4 && dan==1
     */
            switch(w_count)
            {
            case 0:
                if((dz_count== 1 && el_count+kz_count==1 && dan_count==0) || dan_count==1)
                {
           //         forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount,  _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else if(dz_count>1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                }
                break;

            case 1:// 手上有一张王的情况。。。
                if(kz_count+dz_count+el_count==0 && dan_count==0)  // 王抓
                {
                    ting[tCount++] = MJ_Player::MJ_WANG;
                }
                else if(dan_count==1 && kz_count+dz_count+el_count == 1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else
                {
                    ;
                }
                break;

            case 2:
//                dz+el+kz==1 && dan==0 ||       dz+el+kz==1 &&dan=0(单王抓)
//                dz+el+kz == 2 && dan=1 ||
//                dz==2&&el+kz==2 && dan==0   ||
//                dz==1 && kz+el==3&& dan==0        -dz
                if(dz_count + el_count + kz_count == 1 && dan_count==0)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)

                    ting[tCount++] = MJ_Player::MJ_WANG;
                }
                else if(dz_count+el_count+kz_count == 2 && dan_count == 1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else if(dz_count==2 && el_count+kz_count==2 && dan_count==0)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                }
                else if(dz_count==1 && kz_count+el_count==3 && dan_count==0)
                {
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                }
                else
                {

                }

                break;

            case 3:
//                dan==1 && dz+el+kz==0 ||                 双王抓
//                dan==0&&kz+dz+el==2 ||                        (单王抓)
//                dz==1 && kz+el==0 && dan==2                         kz+dz+el == 2&&dan==0 (单王抓)
//                kz+dz+el==3&&dan==1
                if(dan_count==1 && dz_count+el_count+kz_count==0)
                {
                    ting[tCount++] = MJ_Player::MJ_WANG;//
                }
                else if(dan_count==0&&kz_count+el_count+dz_count == 2)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                }
                else if(dan_count==2 && dz_count == 1 && kz_count+el_count==0)
                {
                    forin_dan(dan, dan_count, ting, tCount, _w)
                    forin_dz(dz, dz_count, ting, tCount, _w);
                }
                else if(dan_count==1 && dz_count+kz_count+el_count==3)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                break;

            case 4:
//                *   4: dan==0 && dz+el+kz==0         三王归位
//                *      kz+dz+el==1 && dan==1         双王抓
//                *      -kz+dz+el==0 && dan==3-
//                *      kz+dz+el==3 && dan==0          单王抓
//                *      dz+el+kz==4 && dan==1
                if(dan_count==0 && dz_count+el_count+kz_count)
                {
                    ting[tCount++] = MJ_Player::MJ_WANG;//
                }
                else if(dz_count+kz_count+el_count==1 && dan_count==1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else if(dz_count==1 && kz_count+dz_count+el_count==3 && dan_count==0)
                {
                    //forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    ting[tCount++] = MJ_Player::MJ_WANG;//
                }
                else if(dz_count+el_count+kz_count==4 && dan_count==1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                break;
            }
            return tCount;
}

int MJ_AnalyResult::addHu(MJ_AnalyResult::HU _hu)
{
    switch(_hu)
    {
        case HU_PingHu		:
            break;
        case HU_QiangGang 	:	fan |= HU_QiangGang;
            break;
        case HU_GangShangHua:   fan |= HU_GangShangHua;
            break;
        //    2番
        case HU_ZiMo		:
                                fan &= ~(HU_QiangGang | HU_QuanQiuYi);
                                fan |= HU_ZiMo;
            break;
        case HU_MenQianQing	:
                                if(fan & HU_ZiMo)	fan |= HU_MenQianQing;
            break;
        case HU_XiaoDao	 	:
                                fan |= HU_XiaoDao;
            break;
        case HU_YouYiTao	:
                                if (fan & HU_YouYiTao) {				//  有一套在有一套==?
                                    fan |= HU_YouLiangTao;
                                    break;
                                }
                                fan |= HU_YouYiTao;
                                break;
        case HU_QiaoQiDui	:
                                fan |= HU_QiaoQiDui;
            break;
        case HU_ShiSanLang	:
                                fan |= HU_ShiSanLang;
            break;
        case HU_YiTiaoLong	:
                                fan |= HU_YiTiaoLong;
            break;
        case HU_HunYiSe		:
                                fan |= HU_HunYiSe;
            break;
        case HU_PengpengHu	:
                                fan |= HU_PengpengHu;
            break;
        case HU_SiGuiYi		:
                                fan |= HU_SiGuiYi;
            break;
        case HU_QuanQiuYi	:
                                fan |= HU_QuanQiuYi;
            break;
        case HU_DanWangZhua	:
                                fan |= HU_DanWangZhua;
            break;
        case HU_WangGuiWei	:
                                fan |= HU_WangGuiWei;
            break;
        //    4番
        case HU_DaDao	 	:
                                fan |= HU_DaDao;
            break;
        case HU_YouLiangTao	:
                                fan &= ~HU_YouYiTao;
                                fan |= HU_YouLiangTao;
            break;
        case HU_QiFengHui	:
                                fan |= HU_QiFengHui;
            break;
        case HU_ShuangWangGuiWei:
                                fan |= HU_ShuangWangGuiWei;
            break;
        case HU_DanWangZhuaWang	:
                                fan &= ~HU_DanWangZhua;
                                fan |= HU_DanWangZhuaWang;
            break;
        case HU_ShuangWangZhua	:
                                fan |= HU_ShuangWangZhua;
            break;
        case HU_QingYiSe			:
                                fan |= HU_QingYiSe;
            break;
        ///    8番
        case HU_ShuangWangZhuaWang	:
                                fan &= ~HU_ShuangWangZhua;
                                fan |= HU_ShuangWangZhuaWang;
            break;
        case HU_SanWangGuiWei		:
                                fan |= HU_SanWangGuiWei;
            break;
        /////  16 番
        case HU_SiWangGuiWei		:
                                fan |= HU_SiWangGuiWei;
            break;
    }

    return 0;
}

int MJ_AnalyResult::calc_BeiShu(const MJ_Player &mj_set,MJ_AnalyResult::CARD card, HU_FLAG flag)
{
    qDebug() << "MJ_AnalyResult::calc_BeiShu :";
    qDebug() << "\twang:" << mj_set.wang;
    qDebug() << "\tpai :" << mj_set.paiList;
    qDebug() << "\tChi:" << mj_set.chi;
    qDebug() << "\tpeng:" << mj_set.peng;
    qDebug() << "\tGang:" << mj_set.gang;
    qDebug() << "\tcHu  :" << mj_set.cHuList;
    qDebug() << "\tcGang:" << mj_set.cGangList;
    qDebug() << "\tcPeng:" << mj_set.cPengList;
    qDebug() << "\tcChi :" << mj_set.cChiList;

    CARD all_pai[16] = {0};  //
    CARD all_st[8] = {0};
    CARD all_sl[24] = {0};
    CARD all_sl4yjt[8] = {0};
    int all_slCount4yjt = 0;
    int all_paiCount = 0;
    int all_stCount = 0;
    int all_slCount = 0;

    bool  has_zhong = false;

    if(this->fan)
    {
        return 1;
    }

    //门清
    if(mj_set._g + mj_set._p + mj_set._c == 0 && flag == F_ZiMo)
        this->addHu(this->HU_MenQianQing);

    //杠上花
    if(flag == F_GangShangHua)
        this->addHu(this->HU_GangShangHua);

    //抢杠
    if(flag == F_QiangGang)
        this->addHu(this->HU_QiangGang);

    //自摸
    if(flag == F_ZiMo)
        this->addHu(this->HU_ZiMo);

    for(auto i=0; i<mj_set._g; i++)
    {
        CARD t = mj_set.gang[i];
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_pai[all_paiCount++] = t;
        all_pai[all_paiCount++] = t;
        all_pai[all_paiCount++] = t;
        all_pai[all_paiCount++] = t;
    }
    for(auto i=0; i<mj_set._p; i++)
    {
        CARD t = mj_set.peng[i];
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_pai[all_paiCount++] = t;
        all_pai[all_paiCount++] = t;
        all_pai[all_paiCount++] = t;

        all_st[all_stCount++] = t;
    }
    for(auto i=0; i<mj_set._c; i++)
    {
        CARD t = mj_set.chi[i];
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_pai[all_paiCount++] = t;

        all_sl[all_slCount++] = t;

        if(i%3 == 0)
        {
            all_sl4yjt[all_slCount4yjt++] = t;
        }
    }
    for(auto i=0; i<mj_set.paiCount; i++)
    {
        CARD t = mj_set.paiList[i];
        if(t == MJ_Base::MJ_ZHONG)
        {
            t = mj_set.wang;
            has_zhong = true;
        }
        all_pai[all_paiCount++] = t;
    }
    //------------------所有的牌放在一起了----------------------
    //---------  顺便把  碰 吃 的也  收集了一份--------------
    //----  手上的三同  三连  找出来，
    for(auto i=0; i<st_count; i++){
        CARD t = this->st[i];
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_st[all_stCount++] = t;
    }

    for(auto i=0; i<sl_count; i++)
    {
        CARD t = this->sl[i];
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_sl[all_slCount++]= t;

        //  记录进有几套判断中
        all_sl4yjt[all_slCount4yjt++] = t;

        t -= 1;
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_sl[all_slCount++]= t;

        t -= 1;
        if(t == MJ_Player::MJ_ZHONG)
        {
            has_zhong = true;
            t = mj_set.wang;
        }
        all_sl[all_slCount++]= t;
    }

    if(card == MJ_Player::MJ_WANG)
    //if(card == mj_set.wang)
    {
        switch(this->w_count)
        {
        case 1:
            if(this->dan_count == 0 && this->el_count+this->kz_count+ this->dz_count==0)
            {
                if(mj_set.NewCard == MJ_Player::MJ_WANG)
                    this->addHu(this->HU_DanWangZhuaWang);
                else
                    this->addHu(this->HU_DanWangZhua);
            }
            break;
        case 2:
            if(dz_count+el_count+kz_count==1 && dan_count==0)
            {
                if(mj_set.NewCard == MJ_Player::MJ_WANG)
                    this->addHu(this->HU_DanWangZhuaWang);
                else
                    this->addHu(this->HU_DanWangZhua);
            }
            break;
        case 3:
            if(dan_count==1 && dz_count+el_count+kz_count==0)
            {
                if(mj_set.NewCard == MJ_Player::MJ_WANG)
                    this->addHu(this->HU_ShuangWangZhuaWang);
                else
                    this->addHu(this->HU_ShuangWangZhua);
            }
            else if(dan_count==0&&kz_count+dz_count+el_count==2)
            {
                if(mj_set.NewCard == MJ_Player::MJ_WANG)
                    this->addHu(this->HU_DanWangZhuaWang);
                else
                    this->addHu(this->HU_DanWangZhua);
            }
            break;
        case 4:
            if(dan_count==0 && dz_count+el_count+kz_count==0)
            {
                this->addHu(this->HU_SanWangGuiWei);
            }
            else if(kz_count+dz_count+el_count==1 && dan_count==1 )
            {
                this->addHu(this->HU_ShuangWangZhua);
            }
            else if(dz_count==1 && kz_count+el_count==2 && dan_count==0)
            {
                this->addHu(this->HU_DanWangZhua);
            }
            break;
        }

        return 0;
    }

    //  如果不是 王抓.   把 胡的那张牌  结合进来
    if(card != MJ_Player::MJ_WANG)
    {
        for(auto i=0; i<this->dz_count;i++)
        {
            if(card == this->dz[i])     //  与对子能结合成三同
            {
                CARD t = card;
                if(t == MJ_Player::MJ_ZHONG)
                {
                    has_zhong = true;
                    t = mj_set.wang;
                }
                all_st[all_stCount++] = t;
                this->dz_count--;
                break;
            }
        }
        for(auto i=0; i<this->kz_count;i++) //坎张结合成三连
        {
            if(card == this->kz[i])
            {
                CARD t = card;
                if(t == MJ_Player::MJ_ZHONG)
                {
                    has_zhong = true;
                    t = mj_set.wang;
                }
                // 记录进有几套判断
                all_sl4yjt[all_slCount4yjt++] = t+1;

                all_sl[all_slCount++] = t+1;
                all_sl[all_slCount++] = t;
                all_sl[all_slCount++] = t-1;
                this->kz_count--;
                break;
            }
        }
        for(auto i=0; i<this->el_count; i++)
        {
            CARD c = card;
            if(c == MJ_Player::MJ_ZHONG)
                c = mj_set.wang;
            if(c == this->el[i]+1)
            {
                CARD t = c;
                if(t == MJ_Player::MJ_ZHONG)
                {
                    has_zhong = true;
                    t = mj_set.wang;
                }
                all_sl[all_slCount++] = c;
                all_sl[all_slCount++] = el[i];
                all_sl[all_slCount++] = el[i]-1;
                this->el_count--;
                break;
            }


            if(c==this->el[i]-2)
            {
                CARD t = c;
                if(t == MJ_Player::MJ_ZHONG)
                {
                    has_zhong = true;
                    t = mj_set.wang;
                }
                all_sl[all_slCount++] = el[i];
                all_sl[all_slCount++] = el[i]-1;
                all_sl[all_slCount++] = el[i]-2;
                this->el_count--;
                break;
            }
        }
    }

    this->analy_sort(all_pai, all_paiCount);
    this->analy_sort(all_st, all_stCount);
    this->analy_sort(all_sl, all_slCount);

    //  有几套
    int tao = 0;
    hasYOUJITAO(all_sl4yjt, all_slCount4yjt, &tao);
    if(tao == 1)
    {
        this->addHu(this->HU_YouYiTao);
    }
    else if(tao == 2)
    {
        this->addHu(this->HU_YouLiangTao);
    }
    else {}

    // 清/混一色
    int color = 0;
    for(int i=0; i<all_paiCount; i++)
    {
        if(isFENG(all_pai[i]))
            color |= 0x01;
        else if(isTONG(all_pai[i]))
            color |= 0x02;
        else if(isTIAO(all_pai[i]))
            color |= 0x04;
        else if(isWAN(all_pai[i]))
            color |= 0x08;
    }
    if(color == 0x08 || color == 0x04 || color == 0x02)//清一色
        this->addHu(this->HU_QingYiSe);
    else if(color == 0x09 || color == 0x05 || color == 0x03)
        this->addHu(this->HU_HunYiSe);

    //一条龙
    int ytl = 0;
    if(all_slCount > 3)
        for(int i=0; i<all_slCount; i++)
        {
            ytl = 1;
            CARD  j=MJ_Player::MJ_YIWAN;
            for(; j>=MJ_Player::MJ_JIUWAN; j = j-1)
            {
                if(strchr(all_sl, j) == NULL)
                {
                    ytl = 0;
                    break;
                }
            }
            if(ytl)
            {
                this->addHu(this->HU_YiTiaoLong);
                break;
            }

            ytl = 1;
            for(j=MJ_Player::MJ_YITONG; j>=MJ_Player::MJ_JIUTONG; j -= 1)
            {
                if(strchr(all_sl, j) == NULL)
                {
                    ytl = 0;
                    break;
                }
            }
            if(ytl)
            {
                this->addHu(this->HU_YiTiaoLong);
                break;
            }

            ytl = 1;
            for(j = MJ_Player::MJ_YITIAO; j>=MJ_Player::MJ_JIUTIAO; j -= 1)
            {
                if(strchr(all_sl, j) == NULL)
                {
                    ytl = 0;
                    break;
                }
            }
            if(ytl)
            {
                this->addHu(this->HU_YiTiaoLong);
                break;
            }
        }

    //四归一
    int sgy = 0;
    if(card != MJ_Player::MJ_WANG)
        for(const char* str = all_pai;str != NULL; sgy++)
        {
            str = strchr(str, card);
            if(str == NULL)
                break;
            str++;
        }

    if(sgy == 3)
        this->addHu(this->HU_SiGuiYi);

    //单调将
    if(mj_set.paiCount == 1 && flag == this->F_JiePao)
        this->addHu(this->HU_QuanQiuYi);

    //几王归位
    if(this->w_count ==3 && flag==F_JiePao && this->dan_count==1 && card == this->dan[0])
        this->addHu(this->HU_ShuangWangGuiWei);
    else if(this->w_count == 4 && this->kz_count+this->dz_count + this->el_count+ this->dan_count==0)
        this->addHu(this->HU_SanWangGuiWei);
    else if(this->w_count == 4 && this->kz_count+this->el_count==2 && this->dan_count==0 && this->dz_count==1)
    {
        this->addHu(this->HU_ShuangWangGuiWei);
    }
    else if(this->w_count==2 && this->el_count+this->kz_count+dan_count==0 && this->dz_count==1 && dz[0] == card)
    {
        this->addHu(this->HU_ShuangWangGuiWei);
    }

    //大小道
    if(has_zhong==0 && this->w_count==0)
        this->addHu(this->HU_DaDao);
    else if(has_zhong==1 && this->w_count==0)
        this->addHu(this->HU_XiaoDao);


    // 碰碰胡
    switch(this->w_count)
    {
    case 0:
        if(all_stCount == 4)
            this->addHu(this->HU_PengpengHu);
        break;
    case 1:
        if(all_stCount == 4 || (all_stCount==3 && this->dz_count==1))
            this->addHu(this->HU_PengpengHu);
        break;
    case 2:
        if(all_stCount == 4)
            this->addHu(this->HU_PengpengHu);
        break;
    case 3:
        if(all_stCount==3)
        {
            this->addHu(this->HU_PengpengHu);
        }
        else if(all_stCount==2 && dz_count==2)
            this->addHu(this->HU_PengpengHu);
        else if(all_slCount==1 && dz_count==3)
            this->addHu(this->HU_PengpengHu);
        else if(all_slCount==0 && dz_count==5)
            this->addHu(this->HU_PengpengHu);
        break;
    case 4:
        if(all_stCount==2 && dan_count==1)
            this->addHu(this->HU_PengpengHu);
        else if(all_stCount == 3 && dan_count==1)
            this->addHu(this->HU_PengpengHu);
        else if(all_stCount==1 && dz_count==3)
            this->addHu(this->HU_PengpengHu);
        else if(all_stCount==0 && dz_count==4 && dan_count ==1 )
            this->addHu(this->HU_PengpengHu);
        break;

    }

    return 0;
}

int MJ_AnalyResult::H_ShiSanLang(CARD *ting)
{
    int t_count = 0;

    return t_count;
}

int MJ_AnalyResult::H_QiFengHui(CARD *ting)
{
    int t_count = 0;
    int i=0, feng = 0, tong = 0, tiao = 0, wan = 0;

    for(; i<this->dan_count; i++)
    {
        if(dan[i] == this->dan[i+1])
        {
            return t_count;
        }

        if(isFENG(dan[i]))
        {
            feng++;
        }
        else if(isWAN(dan[i]))
        {
            if(wan)
            {
                if(dan[i-1] - dan[i] < 3)
                    return 0;
            }
            wan++;
        }
        else if(isTIAO(dan[i]))
        {
            if(tiao)
            {
                if(dan[i-1] - dan[i] < 3)
                    return 0;
            }
            tiao++;
        }
        else if(isTONG(dan[i]))
        {
            if(tong)
            {
                if(dan[i-1] - dan[i] < 3)
                    return 0;
            }
            tong++;
        }
        else
        {
            return 0;
        }
    }  //  通过本循环的话至少说明牌符合十三浪的要求了

    if(feng!=7)
        return 0;



    return t_count;
}

int MJ_AnalyResult::H_QiaoQiDui(CARD *ting)
{
    int t_count = 0;
    for(auto i=0; i<this->dan_count - 1; i++)
    {
        if(this->dan[i] == this->dan[i+1])
        {
            this->dz[this->dz_count++] = this->dan[i];
            this->dan[i] = MJ_Player::MJ_noCard;
            this->dan[i+1] = MJ_Player::MJ_noCard;
        }
    }

    if(this->w_count == this->dan_count-1)
    {
        forin_dan(dan, dan_count, ting, t_count, MJ_Player::MJ_noCard)
    }
    else if(this->w_count-this->dan_count-1 >=0)
    {
        forin_dz(dz, dz_count, ting, t_count, MJ_Player::MJ_noCard)
        forin_dan(dan, dan_count, ting, t_count, MJ_Player::MJ_noCard)
    }

    int w = 0;
    if(w = this->w_count-this->dan_count-1 >0)
    {
        // 王归位了
        if(w == 1)
            this->addHu(this->HU_WangGuiWei);
        else if(w == 2)
            this->addHu(this->HU_SanWangGuiWei);
        else if(w == 3)
            this->addHu(this->HU_SanWangGuiWei);
    }

    this->addHu(this->HU_QiaoQiDui);

    return t_count;
}

std::list<const char *> MJ_AnalyResult::HU_names()
{
     std::list<const char *> lst = std::list<const char *>();

//     if(fan == 1)
//         switch(fan & 0x07)
//         {
//         case HU_PingHu:
//             lst.push_back(HuNames[0]);
//             break;
//         case HU_QiangGang:
//             lst.push_back(HuNames[1]);
//             break;
//         case HU_GangShangHua:
//             lst.push_back(HuNames[2]);
//             break;
//         }

     int off = 1;
     for(auto i=0x01; i<=0x01000000; i*=2, off++)
     {
         if(fan & i)
             lst.push_back(HuNames[off]);
     }
     if(fan == 0)
     {
         lst.push_back(HuNames[0]);
     }

     return lst;
}

int MJ_AnalyResult::getFan()
{
    int beishu = 0;

    /*if(fan & 0x07)  beishu += 1;*/
    if(fan & 0x04)  beishu += 2;
    for(auto i=0x08; i<=0x4000; i<<=1)
    {
        if(fan & i)
            beishu += 2;
    }

    for(auto i=0x8000; i<=0x200000; i<<=1)
    {
        if(fan & i)
            beishu += 4;
    }

    for(auto i=0x400000; i<=0x800000; i<<=1)
    {
        if(fan & i)
            beishu += 8;
    }

    if(fan & 0x1000000)
        beishu += 16;

    if(fan == 0)
    {
        beishu += 1;
    }

    return beishu;
}

void MJ_AnalyResult::analy_sort(MJ_AnalyResult::CARD *li, int len)
{
    for(int i=0; i<len-1; i++)
    {
        for (int j=0; j<len-1-i; j++)
        {
            if(li[j] < li[j+1])
            {
                CARD t = li[j];
                li[j] = li[j+1];
                li[j+1] = t;
            }
        }
    }
}


#ifdef DEBUG
void MJ_AnalyResult::printResult()
{
    std::cout << "三同：" << st_count << " | " <<  st << std::endl;
    std::cout << "三连：" << sl_count << " | " <<  sl << std::endl;
    std::cout << "对子：" << dz_count << " | " <<  dz << std::endl;
    std::cout << "坎张：" << kz_count << " | " <<  kz << std::endl;
    std::cout << "二连：" << el_count << " | " << el << std::endl;
    std::cout << "单张：" << dan_count << " | " <<  dan << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}

#endif
