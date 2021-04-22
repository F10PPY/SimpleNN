#include "network.h"
namespace slain
{
double setRand(double )
{
    return  static_cast<double>(rand() % 10000 + 1)/10000-0.5;
}
double afSigmoid(double x)
{
    return 1/(1+exp(-x));
}
double bpSigmoid(double x)
{
   // x= exp(-x)/((1+exp(-x))*(1+exp(-x)));
    double e=exp(-x);
    return e/((1+e)*(1+e));
    //  x=1/(1+exp(-x))*(1-1/(1+exp(-x)));
}
double afBinary(double x)
{
    return x>0.000000000000000000? true:false;
}
void Network::learn(std::vector <std::vector<double>>&X,std::vector <std::vector<double>>&Y,size_t max_epochs,double target_mse)
{
    for (size_t i=0 ; i<max_epochs; i++)
    {
        mse=0;
        for( size_t j=0;j<X.size();j++)
        {
            Matrix <double> inn(X.at(j));
            aggregateOutput(inn);

            Matrix<double>out{{Y.at(0).at(j)}};
            std::cout<<"input:"<<inn;
            std::cout<<n_groups.back().xw_segs.back().weights<<"out: "<<getOutput()<<"expected: "<<Y.at(0).at(j)<<std::endl;
            update(out);
        }
        std::cout << "Epoch:" << i+1 << "/"<<max_epochs <<" MSE: "<<(mse/=X.size())<< std::endl;
        if(mse<=target_mse)
        {
            std::cout<<"Target mse reached."<<std::endl;
            break;
        }
    }
}
}
