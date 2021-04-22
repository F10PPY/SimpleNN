#ifndef NETWORK_H
#define NETWORK_H
#include <vector>
#include <fstream>
#include <cmath>
#include <time.h>
#include <random>
#include <array>

#include "matrix.h"
namespace slain
{
enum af{sigmoid,binary,
       size=binary+1};
double setRand(double);
double afSigmoid(double x);
double afBinary(double x);
double bpSigmoid(double x);
template <typename T>
std::array <T (*)(T),af::size> af_arr;



inline double bpBinary(double x)
{ //return connected_to_xwseg->input;
    return x;
}

struct NeuronGroup
{
    struct XWSegment;
    Matrix <double> output;
    XWSegment* connected_to_xwseg=nullptr;
    double learning_rate;
    double momentum=0;
    double bias=1;
    size_t activation;

    NeuronGroup(const size_t num_neurons, const double _learning_rate, int _activation):output(1,num_neurons),learning_rate(_learning_rate),activation(_activation)
    {}
    struct XWSegment //input-weights segment
    {
        const NeuronGroup* connected_input_grp=nullptr;//for back prop
        const Matrix<double>* input;
        Matrix<double> weights;
        Matrix <double> inputs_weighted;
        XWSegment(const Matrix<double> *_input,const size_t num_neurons):input(_input),weights(input->width,num_neurons),inputs_weighted(1,input->width)
        {}
        void setInput(const Matrix<double>& m)
        {
            input=&m;
        }
        XWSegment& calcWeighted()
        {
            dot(inputs_weighted,*input,weights);
            return *this;
        }
    };
    std::vector<XWSegment> xw_segs;

    XWSegment& addInput(const Matrix<double>& m)
    {
        if(0==m.size()||m.height>1) throw std::length_error("");
        xw_segs.emplace_back(&m,output.width);
        return xw_segs.back();
    }
    XWSegment& addInput(NeuronGroup& n)
    {
        if(0==n.output.size()||n.output.height>1) throw std::length_error("");
        xw_segs.emplace_back(&n.output,output.width);
        xw_segs.back().connected_input_grp=&n;
        n.connected_to_xwseg=&xw_segs.back();
        return xw_segs.back();

    }

//    void activate()
//    {
//        switch (activation)
//        {
//        case sigmoid:
//            output.applyFunc(afSigmoid);
//            break;
//        case binary:
//            output.applyFunc(afBinary);
//            break;
//        }
//    }

      NeuronGroup& calcOutput()
    {
        output.fill(0);
        for(auto&&it:xw_segs)
        {
            output+=it.inputs_weighted;
        }
        output+=bias;
        output.applyFunc(af_arr<double>[activation]);
      //  activate();

        return *this;
    }
};

class Network
{
    public:
    double mse;
    Network()
    {
        af_arr<double>[af::sigmoid]=&afSigmoid;
        af_arr<double>[af::binary]=&afBinary;
        srand(time(nullptr));//temp
    }

    std::vector <NeuronGroup> n_groups;

    NeuronGroup& addGroup(const size_t num_neurons,const double learning_rate,int activation)
    {
        n_groups.emplace_back(num_neurons,learning_rate,activation);
        return  n_groups[0];
    }
    Matrix <double>& getOutput()
    {
        return n_groups.back().output;
    }
    // template<typename T>
    Matrix <double>& aggregateOutput(const Matrix <double>&in)
    {
        n_groups[0].xw_segs[0].setInput(in);
        for(auto&&it:n_groups)
        {
            for(auto&&i:it.xw_segs)
            {
                i.calcWeighted();
            }
            it.calcOutput();
        }
        return n_groups.back().output;
    }
    void learn(std::vector<std::vector<double> > &X, std::vector<std::vector<double> > &Y, size_t max_epochs, double target_mse);
    Matrix<double> error;  //,wDelta;//per xw_segment
    void update(const Matrix <double> &Y)
    {
        error=Y-n_groups.back().output;
        mse+=error[0][0]*error[0][0];
        //  Matrix<double> wDelta;
        if(binary==n_groups.back().activation)
        {
            if(0!=error[0][0])
            {
            Matrix <double> temp = (transpose(*n_groups.back().xw_segs.back().input)*error[0][0]).multiply(n_groups.back().learning_rate);
            n_groups.back().xw_segs.back().weights+=temp;
            n_groups.back().bias+=1*error[0][0];
            }
        }
        else
        {
            // n_groups.back().xw_segs.back().inputs_weighted.copyAndApply(bpSigmoid,wDelta).multiply(error); //counting wDelta
            //n_groups.back().xw_segs.back().weights+=dot(transpose(*n_groups.back().xw_segs.back().input),wDelta).multiply(n_groups.back().learning_rate);
            n_groups.back().xw_segs.back().inputs_weighted.applyFunc(bpSigmoid).multiply(error); //counting wDelta
            n_groups.back().xw_segs.back().weights+=dot(transpose(*n_groups.back().xw_segs.back().input), n_groups.back().xw_segs.back().inputs_weighted).multiply(n_groups.back().learning_rate);
        }
        for(auto &&i=n_groups.rbegin()+1;i!=n_groups.rend();++i)
        {
            for(auto &&it=i->xw_segs.rbegin();it!=i->xw_segs.rend();++it)
            {
                //    dot(error,wDelta,transpose(i->connected_to_xwseg->weights));
                //     it->inputs_weighted.copyAndApply(bpSigmoid,wDelta).multiply(error);
                //    it->weights+=dot(transpose(*it->input),wDelta).multiply(i->learning_rate);
                dot(error,(i-1)->xw_segs.back().inputs_weighted,transpose(i->connected_to_xwseg->weights));
                it->inputs_weighted.applyFunc(bpSigmoid).multiply(error);
                it->weights+=dot(transpose(*it->input), it->inputs_weighted).multiply(i->learning_rate);
            }
        }
    }
};

}//namespace end
#endif // NETWORK_H
