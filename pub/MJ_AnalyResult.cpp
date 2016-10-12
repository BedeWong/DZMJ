#include "MJ_AnalyResult.h"
#include "MJC_CardSet.h"
#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

MJ_AnalyResult::MJ_AnalyResult()
{
    memset(this, 0, sizeof(MJ_AnalyResult));
}

MJ_AnalyResult::MJ_AnalyResult(MJC_CardSet &of)
{
    memset(this, 0, sizeof(MJ_AnalyResult));

    of.copy_chiList(this);
    of.copy_pengList(this);
    of.copy_gangList(this);
}

MJ_AnalyResult::~MJ_AnalyResult()
{

}

int MJ_AnalyResult::canHu(MJC_CardSet::CARD *ting)
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
     *      kz+dz+el==3&&dan==1
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
                    ting[tCount++] = MJC_CardSet::MJ_WANG;
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

                    ting[tCount++] = MJC_CardSet::MJ_WANG;
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
                if(dan_count==0 && dz_count+el_count+kz_count==0)
                {
                    ting[tCount++] = MJC_CardSet::MJ_WANG;//
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
//                *      kz+dz+el==0 && dan==3
//                *      kz+dz+el==3 && dan==0          单王抓
//                *      dz+el+kz==4 && dan==1
                if(dan_count==0 && dz_count+el_count+kz_count)
                {
                    ting[tCount++] = MJC_CardSet::MJ_WANG;//
                }
                else if(dz_count+kz_count+el_count==1 && dan_count==1)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    forin_dan(dan, dan_count, ting, tCount, _w)
                }
                else if(kz_count+dz_count+el_count==3 && dan_count==0)
                {
                    forin_dz(dz, dz_count, ting, tCount, _w)
                    forin_el(el, el_count, ting, tCount, _w)
                    forin_kz(kz, kz_count, ting, tCount, _w)
                    ting[tCount++] = MJC_CardSet::MJ_WANG;//
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
                                if ((fan & HU_YouLiangTao))
                                {
                                    //fan |= HU_YouYiTao;
                                    break;
                                }
                                if (fan & HU_YouYiTao) {				//  有一套在有一套==?
                                    fan |= HU_YouLiangTao;
                                    break;
                                }
                                fan |= HU_YouYiTao;
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

int MJ_AnalyResult::calc_BeiShu(MJ_AnalyResult::CARD card, int flag)
{

    return 0;
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
