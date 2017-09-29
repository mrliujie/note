#include "CSV_Reader.hpp"

bool CSVReader::LoadCSV(const char *path)
{
    std::fstream in;
    in.open(path, std::ios::in);
    if (in.is_open())
    {
        std::map<long, std::string> stringMap;
        long lIndex = 0;
        while( !in.eof())
        {
            std::string strBuff;
            getline(in,strBuff);
            if (!strBuff.empty())
            {
                //将每一行的字符串存到map中，key为顺序号，value为字符串
                stringMap[lIndex++] = strBuff;
                // std::cout << strBuff << std::endl;
            }

        }

        std::map<long, std::string>::iterator iter = stringMap.begin();//遍历每一行字符串
        for (; iter != stringMap.end(); ++iter)
        {
            std::vector<std::string> stringVec;//存储按照逗号分隔好的数据
            std::map<long, std::string> stringMapTemp;
            assert(GetParamFromString(iter->second, stringVec) > 0);
            std::vector<std::string>::size_type idx = 0;
            for (; idx != stringVec.size(); ++idx)
            {
                stringMapTemp[idx + 1] = stringVec[idx];//
            }
            m_stringMap[iter->first] = stringMapTemp;//每一行的存储情况
        }
        m_CSVName = std::string(path);
        in.close();
        return true;
    }
    else
    {
        return false;
    }
}

//用于分割字符串，将CSV表格中的一行按照规则解析成一组字符串，存储在一个vector中
//根据CSV表格中所存储的数据的不同，重载各函数
int CSVReader::GetParamFromString(std::string str,
    std::vector<std::string> &stringVec)
{
    // std::vector<std::string> strTemp;
    stringVec = split(str, ",");
    return stringVec.size();
}

std::vector<std::string> CSVReader::split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

std::string* CSVReader::getStamp(long row, long col)
{
    //因为刚好行号为关键字，所以可以用find进行查找
    std::map<long, std::map<long, std::string>>::iterator iter = m_stringMap.find(row);
    if (iter != m_stringMap.end())
    {
        std::map<long, std::string> &rStrMap = iter->second;
        std::map<long, std::string>::iterator it = rStrMap.find(col);
        if (it != rStrMap.end())
        {
            return &(it->second);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

std::map<long, float> CSVReader::getColString(long col)
{
    std::map<long, std::map<long, std::string>>::iterator iter = m_stringMap.begin();
    std::cout << "mymap.size() is " << m_stringMap.size() << '\n';
    std::map<long, float> m_output;
    while (iter != m_stringMap.end())
    {
        std::map<long, std::string> &rStrMap = iter->second;
        std::map<long, std::string>::iterator it = rStrMap.find(col);
        if (it != rStrMap.end())
        {
            std::string &tmpStr = it->second;
            if ( tmpStr[0] == '\"')
            {
                tmpStr.erase(tmpStr.begin());
                // std::cout << "erase begin : " << tmpStr << std::endl;
            }

            if ( tmpStr[tmpStr.size() - 1] == '\"')
            {
                tmpStr.erase(tmpStr.end() - 1);
                // std::cout << "erase end : " << tmpStr << std::endl;
            }
            // std::cout << "num " << iter->first << "\tdata: " << it->second << std::endl;
            if (isdigit(it->second.c_str()[0]))
            {
                m_output.insert( std::pair<long,float>( iter->first, std::stof(it->second)));

            }
            else
            {
               std::cout << "num 1 " << it->second << std::endl;
            }

        }
        else
        {
            std::cout << "return 0" << std::endl;
            return m_output;
        }
        ++iter;
    }
    if (verbose_)
    {
        std::map<long,float>::iterator it_tmp = m_output.begin();
        for ( ; it_tmp!=m_output.end(); ++it_tmp)
            std::cout << it_tmp->first << " => " << it_tmp->second << '\n';
    }
}

