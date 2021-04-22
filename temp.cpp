#include "temp.h"
#include "network.h"
#include "ntb_util.h"
using namespace slain;
static double learningRate;
Matrix <double> X,  H, Y_HAT,Y, dJdB1, dJdB2, dJdW1, dJdW2, ERR;
Matrix <double> W1,W2;

void loadTraining(const char *filename, std::vector<std::vector<double> > &input, std::vector<std::vector<double> > &output)
{
    size_t trainingSize = 946;
    input.resize(trainingSize);
    output.resize(trainingSize);

    ifstream file(filename);
    if(file.good())
    {
        string line;
        int n;

        for (size_t i=0 ; i<trainingSize ; i++) // load 946 examples
        {
            for (size_t h=0 ; h<32 ; h++) // 'images' are 32*32 pixels
            {
                getline(file, line);
                for (size_t w=0 ; w<32 ; w++)
                {
                    input[i].push_back(atoi(line.substr(w,1).c_str()));
                }
            }
            getline(file, line);
            output[i].resize(10); // output is a vector of size 10 because there is 10 categories (0 to 9)
            n = atoi(line.substr(0,1).c_str()); // get the number that is represented by the array
            output[i][n] = 1; // set value to one at the index that represents the number. Other values are automatically 0 because of the resize()
        }
    }
    file.close();
}
void init(size_t inputNeuron, size_t hiddenNeuron, size_t outputNeuron, double rate)
{
    learningRate = rate;

    W1 = Matrix<double>(inputNeuron, hiddenNeuron);
    W2 = Matrix<double>(hiddenNeuron, outputNeuron);
    srand (time(nullptr)); // to generate random weights
    W1.applyFunc(setRand);
    W2.applyFunc(setRand);
}
Matrix <double>& computeOutput(const std::vector<double> input)
{
    X = Matrix<double>({input});
    dot(H,X,W1).applyFunc(afSigmoid);
    dot(Y_HAT,H,W2).applyFunc(afSigmoid);
    return Y_HAT;
}
void learn(const std::vector<double>& expectedOutput)
{
    Y = Matrix<double>({expectedOutput});

    ERR=Y-Y_HAT;
    dJdB2.move(ERR.multiply(dot(H,W2).applyFunc(bpSigmoid)));
    dot(dJdW2,transpose(H),dJdB2);
    W2 += dJdW2.multiply(learningRate);



    dJdB1.move( dot(dJdB2,transpose(W2)).multiply(dot(X,W1).applyFunc(bpSigmoid)));
    dot(dJdW1,transpose(X),dJdB1);
    W1 += dJdW1.multiply(learningRate);

}
double stepFunction(double x)
{
    if(x>0.9)
    {
        return 1.0;
    }
    if(x<0.1)
    {
        return 0.0;
    }
    return x;
}

bool afBinaryStep(const Matrix <double>& input,const Matrix <double> &weight,const double bias)
{
    cout<<"out: "<<dot(input,transpose(weight))[0][0]<<endl;
    return dot(input,transpose(weight))[0][0]+bias>0.00000001? true:false;
}
void stepic()
{
    std::vector <Matrix<double>> x={{{1,0.3}},{{0.4,0.5}},{{0.7,0.8}}};
    Matrix<bool> y{{1,1,0}};
    Matrix<double> w{{0,0}};
    bool y_hat = false;
    bool perfect=false;

    int iter_count=0;
    double bias=0.1;
    while(perfect!=true)
    {
        perfect=true;
        iter_count++;
        for(size_t i=0;i<x.size();++i)
        {
            y_hat=afBinaryStep(x[i],w,bias);
            cout<<" input: "<<x[i]<<w<<"y_hat: "<<y_hat<<" expected: "<<y.at(0).at(i)<<endl;
            if(y_hat!=y[0][i])
            {
                perfect=false;
                w+=x[i]*(y[0][i]-y_hat);
                bias+=(y[0][i]-y_hat);
            }
        }
    }
    cout<<endl<<w<<"iterations: "<<iter_count<<endl;
}

void test(size_t n)
{
    std::vector<std::vector<double> > inputVector, outputVector;
    loadTraining("../training", inputVector, outputVector); // load data from file called "training"


    init(1024, 16, 10, 0.7);

    Matrix <double> input({inputVector[0]});

    Network net;
    net.addGroup(15,0.2,af::sigmoid);
    net.addGroup(10,0.2,af::sigmoid);
    // net.addGroup(10,0.2);

    net.n_groups[0].addInput(input);
    net.n_groups[1].addInput(net.n_groups[0]);
    //  net.n_groups[2].addInput(net.n_groups[1]);

    net.n_groups[0].xw_segs[0].weights.applyFunc(setRand);
    net.n_groups[1].xw_segs[0].weights.applyFunc(setRand);
    //  net.n_groups[2].xw_segs[0].weights.applyFunc(setRand);


    //   computeOutput(inputVector[0]);
    // net.aggregateOutput(input,afSigmoid);
    ntb::clStart();

    for (size_t i=0 ; i<n ; i++)
    {
        for (size_t j=0 ; j<inputVector.size()-20 ; j++) // skip the last 10 examples to test the program at the end
        {
            //  cout<<computeOutput(inputVector[j])<<endl;
           // computeOutput(inputVector[j]);
           // learn(outputVector[j]);

            Matrix <double> input({inputVector[j]});

            //            net.n_groups[0].xw_segs[0].setInput(input);
            //            net.n_groups[0].calcWeighted().calcOutput(afSigmoid);
            //            net.n_groups[1].calcWeighted().calcOutput(afSigmoid);
            //            net.n_groups[2].calcWeighted().calcOutput(afSigmoid);
            net.aggregateOutput(input);
            //   //  cout<<"MY: "<<net.neuron_groups[1].calcOutput().evaluate(sigmoid).output<<endl;
            Matrix <double> output({outputVector[j]});
            net.update(output);
        }
        cout << "#" << i+1 << "/"<<n << endl;
    }
    cout<<ntb::clElapsed()<<endl;
    // test
    cout << endl << "expected output : actual output" << endl;
    for (size_t i=inputVector.size()-20 ; i<inputVector.size() ; i++) // testing on last 10 examples
    {
        for (size_t j=0 ; j<10 ; j++)
        {
            cout << outputVector[i][j] << " ";
        }
        cout << ": " << computeOutput(inputVector[i]).applyFunc(stepFunction);
        Matrix <double> input({inputVector[i]});
        // net.n_groups[0].xw_segs[0].setInput(input);
        // net.n_groups[0].calcWeighted().calcOutput(afSigmoid);
        //  net.n_groups[1].calcWeighted().calcOutput(afSigmoid);
        net.aggregateOutput(input);
        cout << "\033[1;32m MY: " << net.getOutput().applyFunc(stepFunction) <<"\033[0m"<< endl;
    }
    //  pause();

    //   // catch (const std::exception& e)
    //    {
    //        cout<<e.what()<<endl;
    //        pause();
    //    }
}

