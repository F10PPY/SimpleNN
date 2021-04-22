#include "loaddata.h"
using namespace std;
void loadOutput(std::unordered_map<int,bool> &mp, string s)
{
    std::ifstream file;
    string delim=";";
    size_t pos;
    std::string line,sub;

    file.open(DATA_PATH +s+ "/subs_csi_"+s+".csv");
    if(file.good())
    {
        getline(file,line);//skip first line
        while(getline(file,line))
        {
            pos=line.find(delim);
            if(s=="train")
            {
                mp[stoi(line)]=stoi(line.substr(pos+1));
            }
            else  mp[stoi(line)];
        }
    }
    else std::cout<<"not good"<<endl;
    file.close();
}

void loadFeatures(std::unordered_multimap<int,vector<variant<int,double>>>&mp, string s)
{
    std::ifstream file;
    string delim=";";
    size_t pos;
    std::string line,sub;

    int sk_id;
    std::unordered_multimap<int,vector<variant<int,double>>>::iterator it;


    int mon,year,comcat1;
    file.open(DATA_PATH +s+ "/subs_features_"+s+".csv");
    if(file.good())
    {
        getline(file,line);//skip first line

        while(getline(file,line))
        {
            //  cout<<line<<endl;
            pos=0;
            pos=line.find(".")+1;;
            mon=stoi(&line[pos]);
            pos=line.find(".",pos)+1;
            year=stoi(&line[pos]);
            pos=line.find(delim,pos)+1;
            comcat1=stoi(&line[pos]);
            pos=line.find(delim,pos)+1;
            sk_id=stoi(&line[pos]);
            it=mp.emplace(sk_id,vector<variant<int,double>>());
            it->second.push_back(mon);
            it->second.push_back(year);
            it->second.push_back(comcat1);
            int i=3;//com_cat2=3
            while(std::string::npos!=(pos=line.find(delim,pos)))
            {
                pos++;
                if(line[pos]==',')
                {
                    line="0."+line.substr(pos+1);
                    pos=0;
                }

                switch (i)
                {
                case COM_CAT2_i:
                case COM_CAT3_i:
                case BASE_TYPE_i:
                case ACT_i:
                case ARPU_GROUP_i:
                case COM_CAT7_i:
                case COM_CAT8_i:
                case DEVICE_TYPE_ID_i:
                case INTERNET_TYPE_ID_i:
                case COM_CAT25_i:
                case COM_CAT26_i:
                case COM_CAT34_i:
                {
                    try
                    {
                        it->second.push_back(stoi(&line[pos]));
                    }
                    catch (const std::invalid_argument&)//empty field
                    {
                        it->second.push_back(-1);
                    }
                }
                    break;
                default://double
                {
                    try
                    {
                        it->second.push_back(stod(&line[pos]));
                    }
                    catch (const std::invalid_argument&)//empty field
                    {
                        it->second.push_back(-1.0);
                    }
                }
                    break;
                }
                i++;


            }
            //            for(auto &&i:it->second)
            //            {
            //                cout<<std::setprecision (15) <<i<<" ";
            //            }
            //            cout<<endl;
        }
    }
    else std::cout<<"not good"<<endl;
    file.close();

}

void loadConsumption(std::unordered_multimap<int,vector<variant<int,double>>>&mp, string s)
{
    std::ifstream file;
    string delim=";";
    size_t pos = 0;
    std::string line,sub;

    int sk_id;
    std::unordered_multimap<int,vector<variant<int,double>>>::iterator it;

    file.open(DATA_PATH +s+ "/subs_bs_consumption_"+s+".csv");
    if(file.good())
    {
        getline(file,line);//skip first line
        //   cout<<line<<endl;
        while(getline(file,line))
        {
            //   cout<<line<<endl;
            sk_id=std::stoi(line);
            it=mp.emplace(sk_id,vector<variant<int,double>>());
            int i=0;
            while(std::string::npos!=(pos=line.find(delim,pos+1)))
            {
                if(line[pos+1]==',')
                {
                    sub="0."+line.substr(pos+2);
                }
                else
                {
                    sub=line.substr(pos+1);
                }

                switch (i)
                {
                case CELL_LAC_ID:
                    it->second.push_back(std::stoi(sub));
                    break;
                case MONTH_i:
                    it->second.push_back(std::stoi(sub.substr(sub.find(".")+1)));
                    break;
                case SUM_VOICE_MINUTES_d:
                case SUM_DATA_MB_d:
                case SUM_DATA_MINUTES_d:
                    it->second.push_back(std::stod(sub));
                    break;
                }
                i++;
            }
            //                                for(auto &&i:it->second)
            //                                {
            //                                    cout<<std::setprecision (15) <<i<<" ";
            //                                }
            //                                cout<<endl;
        }
    }
    else std::cout<<"not good"<<endl;
    file.close();
}


void loadSession(std::unordered_multimap<int,vector<variant<int,double>>>&mp,bool type,string s)
{
    std::ifstream file;
    string delim=";";
    size_t pos;
    std::string line;


    std::unordered_multimap<int,vector<variant<int,double>>>::iterator it;

    if(type==VOICE)file.open(DATA_PATH +s+ "/subs_bs_voice_session_"+s+".csv");
    else file.open(DATA_PATH +s+ "/subs_bs_data_session_"+s+".csv");
    if(file.good())
    {
        getline(file,line);//skip first line
        //   cout<<line<<endl;
        while(getline(file,line))
        {
            //  cout<<line<<endl;
            pos=0;
            it=mp.emplace(stoi(line),vector<variant<int,double>>());
            int i=0;
            while(std::string::npos!=(pos=line.find(delim,pos)))
            {
                pos++;
                if(line[pos]==',')
                {
                    line="0."+line.substr(pos+1);
                    pos=0;
                }
                switch (i)
                {
                case CELL_LAC_ID_D:
                    it->second.push_back(stoi(&line[pos]));
                    break;
                case START_TIME_VOICE_DAY_i:
                    //  case START_TIME_VOICE_MONTH_i:
                    //  case START_TIME_VOICE_HOUR_i:
                    it->second.push_back(stoi(&line[pos]));
                    it->second.push_back(stoi(&line[pos+3]));
                    it->second.push_back(stoi(&line[pos+6]));
                    break;
                case VOICE_DUR_MIN_d:
                    it->second.push_back(stod(&line[pos]));
                    break;
                }
                i++;
            }
            //                for(auto &&i:it->second)
            //                        {
            //                                 cout<<std::setprecision (15) <<i<<" ";
            //                          }
            //                             cout<<endl;
        }
    }
    else std::cout<<"not good"<<endl;
    file.close();
}

void splitBsAvgKpi()
{
    std::ifstream file;
    size_t pos = 0;
    int col=-1;
    size_t n=0,g2=0,g3=0,g4=0,empty=0;

    string delim=";";
    ofstream avg_2g("avg_2g.csv"),avg_3g("avg_3g.csv"),avg_4g("avg_4g.csv"),avg_empty("avg_empty");
    std::string line;
    file.open(DATA_PATH "bs_avg_kpi.csv");
    if(file.good())
    {
        getline(file,line);//skip line
        while(getline(file,line))
            // for(int i=0;i<2000;i++)
        {
            // getline(file,line);
            n++;
            pos=0;
            for(col=-1;col<4;col++)pos=line.find(delim,pos+1);
            try
            {
                //col--;
                while(col!=-1)
                {
                    col++;
                    pos++;
                    if(line.at(pos)!=';')
                    {
                        switch (col)
                        {
                        case CSSR_2G:
                        case PSSR_2G:
                        case TBF_DROP_RATE_2G :
                        case TCH_DROP_RATE_2G:
                            g2++;
                            avg_2g<<line<<endl;
                            break;
                        case DL_VOLUME_LTE:
                        case ERAB_PS_BLOCKING_RATE_LTE:
                        case ERAB_PS_DROP_RATE_LTE :
                        case PART_CQI_QPSK_LTE:
                        case PART_MCS_QPSK_LTE:
                        case PSSR_LTE:
                        case RBU_AVAIL_DL:
                        case RBU_AVAIL_UL:
                        case RBU_OTHER_DL:
                        case RBU_OTHER_UL:
                        case RBU_OWN_DL:
                        case RBU_OWN_UL:
                        case RRC_BLOCKING_RATE_LTE:
                        case UL_VOLUME_LTE:
                            g4++;
                            avg_4g<<line<<endl;
                            break;
                        default://3g
                            g3++;
                            avg_3g<<line<<endl;
                            break;
                        }
                        col=-1;
                    }
                }
            }
            catch (...)
            {
                empty++;
                avg_empty<<line<<endl;
                //eol
            }


            //  cout<<line<<endl;
        }
    }
    else std::cout<<"not good"<<endl;
    file.close();
    avg_2g.close();
    avg_3g.close();
    avg_4g.close();
    avg_empty.close();
    cout<<endl;
    //total 58.941,383
    //empty 81,775
    //2g 15,880,098
    //3g 29,451,591
    //4g 13,527,919
}
void cleanAvg2g(std::unordered_multimap<int,vector<variant<int,double>>>&mp)
{
    std::ifstream file;
    string delim=";";
    std::string line,temp;

    std::unordered_multimap<int,vector<variant<int,double>>>::iterator it;
    size_t pos;
    int day,month;
    file.open(DATA_PATH "avg_2g.csv");
    ofstream avg_2g_clean(DATA_PATH"avg_2g_clean.csv");
    avg_2g_clean<<"T_DATE_DAY;T_DATE_MONTH;CELL_AVAILABILITY_2G;CSSR_2G;PSSR_2G;TBF_DROP_RATE_2G;TCH_DROP_RATE_2G"<<endl;
    int no_data=0;
    if(file.good())
    {
        while(getline(file,line))
        {
            pos=0;
            //  cout<<line<<endl;
            day=stoi(line);
            month=stoi(&line[3]);
            it=mp.emplace(stoi(&line[6]),vector<variant<int,double>>());

            it->second.push_back(day);
            it->second.push_back(month);
            pos=line.find(delim)+1;
            temp=line.substr(0,line.find(delim,pos+1)+1);
            int i=2;
            while(std::string::npos!=(pos=line.find(delim,pos)))
            {
                pos++;
                if(line[pos]==',')
                {
                    line="0."+line.substr(pos+1);
                    pos=0;
                }
                switch (i)
                {
                case CELL_AVAILABILITY_2G:
                    temp+=line.substr(pos,line.find(delim,pos)-pos+1);
                    break;
                case CSSR_2G:
                    temp+=line.substr(pos,line.find(delim,pos)-pos+1);
                    break;
                case PSSR_2G:
                    temp+=line.substr(pos,line.find(delim,pos)-pos+1);
                    break;
                case TBF_DROP_RATE_2G:
                    temp+=line.substr(pos,line.find(delim,pos)-pos+1);
                    break;
                case TCH_DROP_RATE_2G:
                    temp+=line.substr(pos,line.find(delim,pos)-pos)+'\n';
                    avg_2g_clean.write(temp.c_str(),temp.size());
                    break;
                    //                    try
                    //                {
                    //                    it->second.push_back(stod(&line[pos]));
                    //                } catch (...)
                    //                {
                    //                    no_data++;
                    //                }
                    //                    break;
                }
                i++;
            }
            //            for(auto &&i:it->second)
            //            {
            //                cout<<std::setprecision (15) <<i<<" ";
            //            }
            //            cout<<endl;
            //            cout<<no_data<<endl;
        }

    }
    else std::cout<<"not good"<<endl;
    file.close();
    avg_2g_clean.close();
}

void load2gVoice(std::unordered_multimap<int,vector<variant<int,double>>>&mp)
{
    std::ifstream file;
    string delim=";";
    std::string line,temp;

    std::unordered_multimap<int,vector<variant<int,double>>>::iterator it;
    size_t pos;
    int day,month;
    int no_data=0;
    file.open(DATA_PATH "avg_2g_clean.csv");
    if(file.good())
    {
        getline(file,line);
      //  for(int ii=0;ii<10000;ii++)
            while(getline(file,line))
        {
          //  getline(file,line);
            pos=0;
            //  cout<<line<<endl;
            day=stoi(line);
            month=stoi(&line[3]);
            it=mp.emplace(stoi(&line[6]),vector<variant<int,double>>());

            it->second.push_back(day);
            it->second.push_back(month);
            pos=line.find(delim)+1;
            temp=line.substr(0,line.find(delim,pos+1)+1);
            int i=2;
            while(std::string::npos!=(pos=line.find(delim,pos)))
            {
                pos++;
                if(line[pos]==',')
                {
                    line="0."+line.substr(pos+1);
                    pos=0;
                }
                switch (i)
                {
                case g2_CSSR_2G:
                case g2_TCH_DROP_RATE_2G:
                    try
                {
                    it->second.push_back(stod(&line[pos]));
                } catch (...)
                {
                    mp.erase(it);
                    pos=string::npos;
                    no_data++;
                }
                    break;




                    //                    try
                    //                {
                    //                    it->second.push_back(stod(&line[pos]));
                    //                } catch (...)
                    //                {
                    //                    no_data++;
                    //                }
                    //                    break;
                }
                i++;
            }
            //            for(auto &&i:it->second)
            //            {
            //                cout<<std::setprecision (15) <<i<<" ";
            //            }
            //            cout<<endl;
            //            cout<<no_data<<endl;
        }

    }
    else std::cout<<"not good"<<endl;
    cout<<"2gvoice no_data: "<<no_data<<endl;
    file.close();

}
















