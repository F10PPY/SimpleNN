#ifndef LOADDATA_H
#define LOADDATA_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <variant>
#include <iomanip>
#include <fstream>
#define DATA_PATH "/mnt/hgfs/Dataset/"

using namespace std;
enum subs_features{SNAP_DATE_MONTH_i,SNAP_DATE_YEAR_i,COM_CAT1_i,COM_CAT2_i,COM_CAT3_i,BASE_TYPE_i,ACT_i,ARPU_GROUP_i,COM_CAT7_i,COM_CAT8_i,DEVICE_TYPE_ID_i,INTERNET_TYPE_ID_i,REVENUE,ITC,
                   VAS,RENT_CHANNEL,ROAM,COST,COM_CAT17,COM_CAT18,COM_CAT19,COM_CAT20,COM_CAT21,COM_CAT22,COM_CAT23,COM_CAT24,
                   COM_CAT25_i,COM_CAT26_i,COM_CAT27,COM_CAT28,COM_CAT29,COM_CAT30,COM_CAT31,COM_CAT32,COM_CAT33,COM_CAT34_i,
                   subs_features_size=COM_CAT34_i+1
                  };
enum subs_consum{CELL_LAC_ID,MONTH_i,SUM_VOICE_MINUTES_d,SUM_DATA_MB_d,SUM_DATA_MINUTES_d,
                 subs_consum_size=SUM_DATA_MINUTES_d+1
                };
enum subs_bs_data{CELL_LAC_ID_D,DATA_VOL_MB,START_TIME_DATA_DAY_i,START_TIME_DATA_MONTH_i,START_TIME_DATA_HOUR_i,
                  subs_bs_data=START_TIME_DATA_HOUR_i+1
                 };
enum subs_bs_voice{CELL_LAC_ID_V,VOICE_DUR_MIN_d,START_TIME_VOICE_DAY_i,START_TIME_VOICE_MONTH_i,START_TIME_VOICE_HOUR_i,
                   subs_bs_voice_size=START_TIME_VOICE_HOUR_i+1
                  };
enum bs_avg_kpi{
    T_DATE,CELL_LAC_ID_AVG,CELL_AVAILABILITY_2G,CELL_AVAILABILITY_3G,CELL_AVAILABILITY_4G,CSSR_2G,CSSR_3G,ERAB_PS_BLOCKING_RATE_LTE,ERAB_PS_BLOCKING_RATE_PLMN_LTE,ERAB_PS_DROP_RATE_LTE,
    HSPDSCH_CODE_UTIL_3G,NODEB_CNBAP_LOAD_HARDWARE,PART_CQI_QPSK_LTE,PART_MCS_QPSK_LTE,PROC_LOAD_3G,PSSR_2G,PSSR_3G,PSSR_LTE,RAB_CS_BLOCKING_RATE_3G,RAB_CS_DROP_RATE_3G,RAB_PS_BLOCKING_RATE_3G,
    RAB_PS_DROP_RATE_3G,RBU_AVAIL_DL,RBU_AVAIL_DL_LTE,RBU_AVAIL_UL,RBU_OTHER_DL,RBU_OTHER_UL,RBU_OWN_DL,RBU_OWN_UL,RRC_BLOCKING_RATE_3G,RRC_BLOCKING_RATE_LTE,RTWP_3G,SHO_FACTOR,TBF_DROP_RATE_2G,
    TCH_DROP_RATE_2G,UTIL_BRD_CPU_3G,UTIL_CE_DL_3G,UTIL_CE_HW_DL_3G,UTIL_CE_UL_3G,UTIL_SUBUNITS_3G,UL_VOLUME_LTE,DL_VOLUME_LTE,TOTAL_DL_VOLUME_3G,TOTAL_UL_VOLUME_3G,
    bs_avg_kpi_size=TOTAL_UL_VOLUME_3G+1
};
enum avg_2g{g2_T_DATE_DAY,g2_T_DATE_MONTH,g2_CELL_AVAILABILITY_2G,g2_CSSR_2G,g2_PSSR_2G,g2_TBF_DROP_RATE_2G,g2_TCH_DROP_RATE_2G,
            avg_2g_size=g2_TCH_DROP_RATE_2G+1
           };
enum avg_2g_voice{DAY,MONTH,CSSR,CH_DROP_RATE
};

enum avg_4g{g4_T_DATE,g4_CELL_AVAILABILITY_4G,g4_DL_VOLUME_LTE,g4_ERAB_PS_BLOCKING_RATE_LTE,g4_ERAB_PS_DROP_RATE_LTE,g4_PART_CQI_QPSK_LTE,g4_PART_MCS_QPSK_LTE,g4_PSSR_LTE,g4_RBU_AVAIL_DL,
            g4_RBU_AVAIL_UL,g4_RBU_OTHER_DL,g4_RBU_OTHER_UL,g4_RBU_OWN_DL,g4_RBU_OWN_UL,g4_RRC_BLOCKING_RATE_LTE,g4_UL_VOLUME_LTE,
            avg_4g_size=g4_UL_VOLUME_LTE+1
           };
enum avg_3{g3_T_DATE
          };
enum {DATA,VOICE};
inline std::ostream& operator<< (std::ostream& os, variant<int,double> const& v)
{
    std::visit([&os](auto const& e){ os << e; }, v);
    return os;
}
void loadOutput(std::unordered_map<int,bool> &mp,string);
void loadFeatures(unordered_multimap<int,vector<variant<int,double>>>&mp,string);
void loadConsumption(std::unordered_multimap<int,vector<variant<int,double>>>&mp,string);
void loadSession(std::unordered_multimap<int,vector<variant<int,double>>>&mp,bool type,string);
void loadBsAvgKpi();
void splitBsAvgKpi();
void cleanAvg2g(std::unordered_multimap<int,vector<variant<int,double>>>&mp);
void load2gVoice(std::unordered_multimap<int,vector<variant<int,double>>>&mp);
#endif // LOADDATA_H
