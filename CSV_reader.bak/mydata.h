#ifndef __TIMESTORAGE_H__
#define __TIMESTORAGE_H__

#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>

class MyData
{
public:
    MyData() :time_cost(0), id()
    {}
    explicit MyData(int) :time_cost(0), id("mydata1234") // explicit to avoid implicit conversion
    {}
    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "id" << id << "time_cost" << time_cost << "}";
    }
    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        time_cost = (double)node["time_cost"];
        id = (std::string)node["id"];
    }
public:   // Data Members
    double time_cost;
    std::string id;
    void set_time(std::string name, double time)
    {
        time_cost = time;
        id = name;
    }
};

//These write and read functions must be defined for the serialization in FileStorage to work
static void write(cv::FileStorage& fs, const std::string&, const MyData& x)
{
    x.write(fs);
}
static void read(const cv::FileNode& node, MyData& x, const MyData& default_value = MyData()){
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}


#endif 