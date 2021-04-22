#include <iostream>
#include <unistd.h>
#include <exception>
#include "ntb_util.h"
#include "temp.h"
#include "network.h"
#include "loaddata.h"
//#include "map"


using namespace std;
using namespace ntb;
using namespace slain;

void test(size_t);
void omar(int);
void stepic();
int main()
{
     //stepic();
    // omar(10);
    //        Matrix<double> m1,m2(100,100);
    //        ntb::clStart();
    //        for(int i=0;i<100000;i++)
    //        {
    //            m2={100,100};
    //           // m2.copyAndApply(afSigmoid,m1);
    //           // m1=m2;
    //            m1.move(m2.applyFunc(afSigmoid));
    //        }
    //        ntb::clElapsedPrint();

    // test(30);
    std::vector <vector<double>> x={{{1,0.3}},{{0.4,0.5}},{{0.7,0.8}}};
    std::vector <vector<double>> y={{1,1,0}};
    Network nn;
    nn.addGroup(1,0.2,af::binary);
    Matrix <double> inn(x.at(0));
    nn.n_groups[0].addInput(inn);

    nn.learn(x,y,1000,0);





    unordered_multimap<int,vector<variant<int,double>>> consum;// SK_ID/ CELL_LAC_ID;MON;SUM_MINUTES;SUM_DATA_MB;SUM_DATA_MIN //last 3 months /month DATA IS COMPLETE
    //  loadConsumption(consum,"train");

    unordered_map<int,bool>skid_csi_map_train;//SK_ID/answer
    loadOutput(skid_csi_map_train,"train");
    unordered_map<int,bool>skid_csi_map_test;//SK_ID/answer
    loadOutput(skid_csi_map_test,"test");

    unordered_multimap<int,vector<variant<int,double>>> features;// SK_ID/ last year /month
    //  loadFeatures(features,"train");


    unordered_multimap<int,vector<variant<int,double>>> data_sessions;// SK_ID/last 3 months /hour  DATA IS COMPLETE
    // 3853;61951;,00000356118439;04.04 05:00:00
    // 61951 3.56118439e-06 4 4 5
    //  loadSession(data_sessions,DATA,"train");

    unordered_multimap<int,vector<variant<int,double>>> voice_sessions_train,voice_sessions_test;// SK_ID/last 3 months /hour DATA IS COMPLETE
    //  373;269094;,0015972208333333333;27.04 23:00:00
    //   269094 0.00159722083333333 27 4 23
    loadSession(voice_sessions_train,VOICE,"train");
    loadSession(voice_sessions_test,VOICE,"test");


    unordered_multimap<int,vector<variant<int,double>>> avg_2g_voice;// cell_lac/3month /day  DAY,MONTH,CSSR,CH_DROP_RATE
    load2gVoice(avg_2g_voice);



    //  vector<vector<double>> data;

    Network v2g;
    v2g.addGroup(1,0.1,af::sigmoid);
    Matrix<double> inv({{get<double>(avg_2g_voice.begin()->second.at(CSSR)),get<double>(avg_2g_voice.begin()->second.at(CH_DROP_RATE))}});
    v2g.n_groups[0].addInput(inv);
    v2g.n_groups[0].xw_segs[0].weights.applyFunc(setRand);
    size_t max_epochs=5;
    size_t cnt,total=0;
    double sum=0;
    int correct=0;
    for (size_t i=0 ; i<max_epochs; i++)
    {
        v2g.mse=0;
        total=0;
        correct=0;
        for(auto&&it:skid_csi_map_train)
        {
            cnt=0;
            sum=0;
            auto range = voice_sessions_train.equal_range(it.first);
            for(auto&& i=range.first;i!=range.second;i++)
            {

                auto avg_range = avg_2g_voice.equal_range(get<int>(i->second.at(CELL_LAC_ID_D)));
                for(auto&& j=avg_range.first;j!=avg_range.second;j++)
                {
                    if(get<int>(j->second.at(avg_2g_voice::MONTH))==get<int>(i->second.at(START_TIME_VOICE_MONTH_i))&&
                            get<int>(j->second.at(avg_2g_voice::DAY))==get<int>(i->second.at(START_TIME_VOICE_DAY_i)))
                    {
                        Matrix<double> in({{get<double>(j->second.at(CSSR)),get<double>(j->second.at(CH_DROP_RATE))}});
                        v2g.aggregateOutput(in);
                        sum+=v2g.getOutput()[0][0];
                        Matrix <double> out({{static_cast<double>((it.second))}});
                        v2g.update(out);
                        cnt++;
                        total++;
                    }

                }
            }
            sum/=cnt;
            if(sum>0.5)sum=1;
            else sum=0;
            if(static_cast<bool>(sum)==it.second)correct++;
        }


        std::cout << "Epoch:" << i+1 << "/"<<max_epochs <<" MSE: "<<(v2g.mse/=total)<< std::endl<<"correct: "<<correct<<endl;
    }


    ofstream test("out");
    for(auto&&it:skid_csi_map_test)
    {
        cnt=0;
        sum=0;
        auto range = voice_sessions_test.equal_range(it.first);
        for(auto&& i=range.first;i!=range.second;i++)
        {

            auto avg_range = avg_2g_voice.equal_range(get<int>(i->second.at(CELL_LAC_ID_D)));
            for(auto&& j=avg_range.first;j!=avg_range.second;j++)
            {
                if(get<int>(j->second.at(avg_2g_voice::MONTH))==get<int>(i->second.at(START_TIME_VOICE_MONTH_i))&&
                        get<int>(j->second.at(avg_2g_voice::DAY))==get<int>(i->second.at(START_TIME_VOICE_DAY_i)))
                {
                    Matrix<double> in({{get<double>(j->second.at(CSSR)),get<double>(j->second.at(CH_DROP_RATE))}});
                    v2g.aggregateOutput(in);
                    sum+=v2g.getOutput()[0][0];
                    cnt++;
                }
            }
        }
        sum/=cnt;
        if(sum>0.5)sum=1;
        else sum=0;
        test.write(to_string(sum).c_str(),1);
        test.write("\n",1);
    }



    return 0;
    //    int n=0;
    //    for (auto&&m:consum)
    //    {
    //        n++;
    //        // cout<<std::setprecision (15)<<m.second.at(COM_CAT26)<<" ";
    //       // cout<<std::setprecision (15) <<get<double>(m.second.at(DATA_VOL_MB))<<" ";
    //        for(auto &&i:m.second)
    //        {
    //             cout<<std::setprecision (15)<<fixed <<i<<" ";
    //        }
    //        cout<<endl;
    //    }



    Network net;
    net.addGroup(1,0.1,af::binary);
    //  net.addGroup(1,0.1);
    //  net.addGroup(1,0.1);

    Matrix<double> in({{static_cast<double>(get<int>(features.begin()->second.at(ACT_i)))}});
    net.n_groups[0].addInput(in);
    // net.n_groups[1].addInput(net.n_groups[0]);
    // net.n_groups[2].addInput(net.n_groups[1]);

    net.n_groups[0].xw_segs[0].weights.applyFunc(setRand);
    //  net.n_groups[1].xw_segs[0].weights.applyFunc(setRand);
    //  net.n_groups[2].xw_segs[0].weights.applyFunc(setRand);
    max_epochs=10;
    for (size_t i=0 ; i<max_epochs; i++)
    {
        net.mse=0;
        for(auto i=features.begin();i!=features.end();i++)
        {
            Matrix<double> in({{static_cast<double>(get<int>(i->second.at(ACT_i)))}});
            net.aggregateOutput(in);
            Matrix <double> out({{static_cast<double>(skid_csi_map_train[i->first])}});
            net.update(out);
        }
        std::cout << "Epoch:" << i+1 << "/"<<max_epochs <<" MSE: "<<(net.mse/=features.size())<< std::endl;
    }


    //    int total=0,correct=0;
    //    for(auto i=features.begin();i!=features.end();i++)
    //    {
    //        total++;
    //        Matrix<double> in({{static_cast<double>(get<int>(i->second.at(ACT_i)))}});

    //        // if(get<int>(i->second.at(ACT_i))==0)  net.aggregateOutput(in);
    //        net.aggregateOutput(in);

    //        // if(static_cast<bool>(net.getOutput()[0][0])==skid_csi_map[i->first])correct++;
    //        if(0==skid_csi_map[i->first])correct++;
    //        cout << "\033[1;32m" <<net.getOutput() <<"\033[0m"<<" correct: "<<skid_csi_map[i->first]<< endl;
    //    }

    //    for(auto &&it=skid_csi_map.begin();it!=skid_csi_map.end();it++)
    //    {
    //        auto range = features.equal_range(it->first);
    //        for(auto&& i=range.first;i!=range.second;i++)
    //        {
    //            Matrix<double> in({{static_cast<double>(get<int>(i->second.at(ACT_i)))}});
    //            net.aggregateOutput(in);
    //            if(net.getOutput()==)
    //        }
    //    }

    return 0;
}















