#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>
#include <sstream>
#define SLAIN
namespace slain
{
template <typename T>
class Matrix
{

    //data
    std::vector<std::vector<T> > matrix;
public:
    size_t height;
    size_t width;
    //
    size_t size()const noexcept;
    Matrix<T> subMatrix(size_t startH, size_t startW, size_t h, size_t w) const;
    void move(Matrix<T>&b)
    {
        height=b.height;
        width=b.width;
        b.height=0;
        b.width=0;
        matrix=std::move(b.matrix);
    }
    friend  Matrix<T> dot(Matrix<T> const &m1,Matrix<T> const &m2)
    {
        if(m1.width!=m2.height )
        {
            throw std::length_error("");
        }

        T vecdot;
        Matrix<T> result(m1.height, m2.width);

        for (size_t i=0 ; i<m1.height ; i++)
        {
            for (size_t j=0 ; j<m2.width ; j++)
            {
                vecdot=0;
                for (size_t w=0 ; w<m1.width ; w++)
                {
                    vecdot += m1.matrix[i][w]*m2.matrix[w][j];
                }
                result.matrix[i][j] = vecdot;
            }
        }
        return result;
    }
    friend Matrix<T>& dot(Matrix<T> &result, Matrix<T> const &m1,Matrix<T> const &m2)
    {
        if(m1.width!=m2.height )
        {
            throw std::length_error("");
        }
        T vecdot;
        result.height=m1.height;
        result.width=m2.width;
        result.matrix.resize(result.height);

        for (size_t i=0 ; i<m1.height ; i++)
        {
            for (size_t j=0 ; j<m2.width ; j++)
            {
                vecdot=0;
                //  result.matrix[i].resize(result.width);
                //  result.matrix[i][j] =0;
                for (size_t w=0 ; w<m1.width ; w++)
                {
                    vecdot += m1.matrix[i][w]*m2.matrix[w][j];
                    //  result.matrix[i][j]+= m1.matrix[i][w]*m2.matrix[w][j];
                }
                result.matrix[i].resize(result.width);
                result.matrix[i][j] = vecdot;
            }
        }
        return result;
    }
    friend Matrix<T> transpose(Matrix const &m)
    {
        Matrix<T> result(m.width, m.height);

        for (size_t i=0 ; i<m.width ; i++)
        {
            for (size_t j=0 ; j<m.height ; j++)
            {
                result.matrix[i][j] = m.matrix[j][i];
            }
        }
        return result;
    }
    Matrix<T>& copyAndApply(T function(T),Matrix<T> &to)
    {

        to.height = height;
        to.width = width;

        to.matrix={height,std::vector<T>(width)};

        typename std::vector<std::vector<T> >::iterator to_it =to.matrix.begin();
        typename std::vector<T>::iterator to_it2;
        for(auto&&it:matrix)
        {
            to_it2=to_it->begin();
            for(auto&&it2:it)
            {
                *to_it2=function(it2);
                to_it2++;
            }
            to_it++;
        }
        return to;
    }
    //accessors
    std::vector<T>& at(size_t y)
    {
        return matrix.at(y);
    }
    T& at(size_t y,size_t x)
    {
        return matrix.at(y).at(x);
    }
    std::vector<T>& operator[](size_t y) noexcept//undefined behaviour if accessed element >= matrix.height
    {
        return matrix[y];
    }
    const std::vector<T>& operator[](size_t y)const noexcept//undefined behaviour if accessed element >= matrix.height
    {
        return matrix[y];
    }


    //these functions are applied to the caller
    void fill(const T value);
    void addRows(const size_t rows);
    void addCols(const size_t cols);
    void setRows(const size_t x);
    void setCols(const size_t y);
    Matrix<T> &add(T const value);
    Matrix<T> &subtract(T const value);
    Matrix<T> &multiply(T const value);
    Matrix<T> &add(Matrix<T> const &m);
    Matrix<T> &subtract(Matrix<T> const &m);
    Matrix<T> &multiply(Matrix<T> const &m);
    Matrix<T> &dot(Matrix<T> const &m);
    Matrix<T> &transpose();
    Matrix<T> &applyFunc(T function(T));
    //
    //operator overloads
    Matrix<T>& operator=(Matrix<T> const &b)//copy assignment
    {
        //std::cout<<"copy ass"<<std::endl;
        height=b.height;
        width=b.width;
        //        matrix.resize(height);
        //          for (size_t i=0; i<height; i++)
        //          {
        //            matrix[i].resize(width);
        //          }
        matrix=b.matrix;
        return *this;
    }
    Matrix<T> operator+(Matrix<T> const &m)const
    {
        if(height<m.height || width<m.width)
        {
            throw std::length_error("");
        }
        Matrix result(height,width);
        for (size_t i=0; i<m.height ; ++i)
        {
            for (size_t j=0; j<m.width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] + m.matrix[i][j];
            }
        }
        return result;

    }
    Matrix<T> operator-(Matrix<T> const &m)const
    {
        if(height<m.height || width<m.width)
        {
            throw std::length_error("");
        }
        Matrix result(height,width);
        for (size_t i=0; i<m.height ; ++i)
        {
            for (size_t j=0; j<m.width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] - m.matrix[i][j];
            }
        }
        return result;
    }
    Matrix<T> operator*(Matrix<T> const &m)const
    {
        if(height!=m.height || width!=m.width)
        {
            throw std::length_error("");
        }
        Matrix result(height,width);
        for (size_t i=0; i<m.height ; ++i)
        {
            for (size_t j=0; j<m.width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] * m.matrix[i][j];
            }
        }
        return result;
    }
    Matrix<T> operator+(T const value)const
    {
        Matrix result(height,width);
        for (size_t i=0; i<height ; ++i)
        {
            for (size_t j=0; j<width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] + value;
            }
        }
        return result;
    }
    Matrix<T> operator-(T const value)const
    {
        Matrix result(height,width);
        for (size_t i=0; i<height ; ++i)
        {
            for (size_t j=0; j<width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] - value;
            }
        }
        return result;
    }
    Matrix<T> operator*(T const value)const
    {
        Matrix result(height,width);
        for (size_t i=0; i<height ; ++i)
        {
            for (size_t j=0; j<width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] * value;
            }
        }
        return result;
    }
    Matrix<T> operator/(T const value)const
    {
        Matrix result(height,width);
        for (size_t i=0; i<height ; ++i)
        {
            for (size_t j=0; j<width ; ++j)
            {
                result.matrix[i][j]=matrix[i][j] / value;
            }
        }
        return result;
    }
    bool operator==(Matrix const &m) const
    {
        if(height==m.height && width==m.width)
        {
            for (size_t i=0 ; i<height ; i++)
            {
                for (size_t j=0 ; j<width ; j++)
                {
                    if(matrix[i][j]!=m.matrix[i][j])
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        return false;
    }
    bool operator!=(Matrix const &m)const
    {
        return !operator==(m);
    }
    void operator+=(Matrix const &m)
    {
        add(m);
    }
    void operator-=(Matrix const &m)
    {
        subtract(m);
    }
    void operator*=(Matrix const &m)
    {
        multiply(m);
    }
    void operator+=(T const value)
    {
        add(value);
    }
    void operator-=(T const value)
    {
        subtract(value);
    }
    void operator*=(T const value)
    {
        multiply(value);
    }
    friend std::ostream& operator<<(std::ostream &os, Matrix<T> const &m)
    {
        for(size_t i=0;i<m.height;++i)
        {
            for(size_t j=0;j<m.width;++j)
            {
                os<<m.matrix[i][j]<<" ";
            }
            os<<std::endl;
        }
        return os;
    }
    //
    //constructors, destructors
    Matrix(const size_t _height=0, const size_t _width=0, const T fillval=0)
        :height(_height),width(_width)
    {
        // if(0==width&&0!=height)width=1;
        // if(0==height&&0!=width)height=1;
        matrix={height, std::vector<T>(width,fillval)};
    }

    Matrix(std::vector<std::vector<T> > const &vecvec)
    {
        height = vecvec.size();
        width = vecvec[0].size();
        matrix.resize(height);
        for (size_t i=0 ; i<height ; i++)
        {
            if(vecvec[i].size()!=width) throw std::length_error("");// prevents not equal length rows
            matrix[i] = vecvec[i];
        }
    }
    Matrix(std::vector<T> const &vec)
    {
        height = 1;
        width = vec.size();
        // matrix.resize(width);
        matrix.push_back(vec);
    }
    Matrix(std::initializer_list< std::initializer_list<T> > const ilil)
    {
        height = ilil.size();
        width = ilil.begin()->size();
        matrix.resize(height);
        size_t i=0;
        for (auto il : ilil)
        {
            if(il.size()!=width) throw std::length_error("");// prevents not equal length rows
            matrix[i++]=il;
        }
    }
    Matrix(const Matrix<T>& b)//copy ctor
    {
        //std::cout<<"copy ctor"<<std::endl;
        height = b.height;
        width = b.width;
        matrix = b.matrix;
    }
    ~Matrix(){}
    //
};
template <typename T>
Matrix<T>& Matrix<T>::add(Matrix const &m)
{
    if(height<m.height || width<m.width)
    {
        throw std::length_error("");
    }
    for (size_t i=0 ; i<m.height ; i++)
    {
        for (size_t j=0 ; j<m.width ; j++)
        {
            matrix[i][j] += m.matrix[i][j];
        }
    }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::add(T const value)
{
    for (size_t i=0 ; i<height ; i++)
    {
        for (size_t j=0 ; j<width ; j++)
        {
            matrix[i][j] += value;
        }
    }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::subtract(T const value)
{
    for (size_t i=0 ; i<height ; i++)
    {
        for (size_t j=0 ; j<width ; j++)
        {
            matrix[i][j] -= value;
        }
    }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::subtract(Matrix const &m)
{
    if(height<m.height || width<m.width)
    {
        throw std::length_error("");
    }
    for (size_t i=0 ; i<m.height ; i++)
    {
        for (size_t j=0 ; j<m.width ; j++)
        {
            matrix[i][j] -= m.matrix[i][j];
        }
    }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::multiply(T const value)
{
    for (size_t i=0 ; i<height; i++)
    {
        for (size_t j=0 ; j<width; j++)
        {
            matrix[i][j] *= value;
        }
    }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::multiply(Matrix const &m)
{
    if(height!=m.height || width!=m.width)
    {
        throw std::length_error("");
    }
    for (size_t i=0 ; i<height ; i++)
    {
        for (size_t j=0 ; j<width ; j++)
        {
            matrix[i][j]*= m.matrix[i][j];
        }
    }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::dot(Matrix const &m)
{
    if(width!=m.height )
    {
        throw std::length_error("");
    }
    T vecdot;
    Matrix<T> result(height, m.width);

    for (size_t i=0 ; i<height ; i++)
    {
        for (size_t j=0 ; j<m.width ; j++)
        {
            vecdot=0;
            for (size_t w=0 ; w<width ; w++)
            {
                vecdot += matrix[i][w]*m.matrix[w][j];
            }
            result.matrix[i][j] = vecdot;
        }
    }
    this->move(result);
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::transpose()
{
    Matrix<T> result(width, height);

    for (size_t i=0 ; i<width ; i++)
    {
        for (size_t j=0 ; j<height ; j++)
        {
            result.matrix[i][j] = matrix[j][i];
        }
    }
    this->move(result);
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::applyFunc(T function(T))
{
    for(auto&&it:matrix)
    {
        for(auto&&i:it)
        {
            i=function(i);
        }
    }
    return *this;
}
template <typename T>
Matrix<T> Matrix<T>::subMatrix(size_t startH, size_t startW, size_t h, size_t w) const
{
    size_t endH=startH+h,endW=startW+w;
    if(endH>height || endW>width)
    {
        throw std::length_error("");
    }
    Matrix<T> result(h,w);

    for (size_t i=0 ; i<endH ; i++)
    {
        for (size_t j=0 ; j<endW ; j++)
        {
            result.matrix[i][j] = matrix[i+startH][j+startW];
        }
    }
    return result;
}
template <typename T>
void  Matrix<T>::setRows(const size_t _height)
{
    //    if(0!=_height)
    //    {
    //        if(0==width)width=1; //when adding rows to empty matrix, width can't be zero anymore
    //    }
    std::vector <T>dummy(width);
    matrix.resize(_height,dummy);
    height=_height;
}
template <typename T>
void  Matrix<T>::setCols(const size_t _width)
{
    //    if(0!=_width)
    //    {
    //         matrix.resize(1);
    //        if(0==height)height=1; //when adding cols to empty matrix, height can't be zero anymore
    //    }
    for(size_t i=0;i<height;++i)
    {
        matrix[i].resize(_width);
    }
    width=_width;
}
template <typename T>
void  Matrix<T>::addRows(const size_t rows)
{
    setRows(height+rows);
}
template <typename T>
void  Matrix<T>::addCols(const size_t cols)
{
    setCols(width+cols);
}
template <typename T>
void Matrix<T>::fill(T const value)
{
    for (size_t i=0 ; i<height ; i++)
    {
        for (size_t j=0 ; j<width ; j++)
        {
            matrix[i][j] = value;
        }
    }
}
template <typename T>
size_t Matrix<T>::size()const noexcept
{
    return width*height;
}
};//namespace end
#endif // MATRIX_H
