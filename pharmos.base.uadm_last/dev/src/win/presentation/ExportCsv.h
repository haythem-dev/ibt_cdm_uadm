#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class CsvFile;

static CsvFile& endrow(CsvFile& file);
static CsvFile& flush(CsvFile& file);

class CsvFile
{
    std::ofstream fs_;
    bool is_first_;
    const std::string separator_;
    const std::string escape_seq_;
    const std::string special_chars_;
public:
    CsvFile(const std::string filename, const std::string separator = ";");
        
    ~CsvFile();
    
    void flush();

    void endrow();

    CsvFile& operator << (CsvFile& (*val)(CsvFile&));
   
    CsvFile& operator << (const char * val);
    
    CsvFile& operator << (const std::string & val);

    template<typename T>
    CsvFile& operator << (const T& val)
    {
        return write(val);
    }

private:
    template<typename T>
    CsvFile& write(const T& val)
    {
        if (!is_first_)
        {
            fs_ << separator_;
        }
        else
        {
            is_first_ = false;
        }
        fs_ << val;
        return *this;
    }

    std::string escape(const std::string & val);
};

static CsvFile& endrow(CsvFile& file)
{
    file.endrow();
    return file;
}

static CsvFile& flush(CsvFile& file)
{
    file.flush();
    return file;
}

