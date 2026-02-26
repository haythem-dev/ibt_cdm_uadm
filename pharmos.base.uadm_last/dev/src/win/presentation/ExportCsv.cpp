
#include "ExportCsv.h"

CsvFile:: CsvFile(const std::string filename, const std::string separator )
: fs_()
, is_first_(true)
, separator_(separator)
, escape_seq_("\"")
, special_chars_("\"")
{
    fs_.exceptions(std::ios::failbit | std::ios::badbit);
    fs_.open(filename);
}

CsvFile::~CsvFile()
{
    flush();
    fs_.close();
}

void CsvFile::flush()
{
    fs_.flush();
}

void CsvFile::endrow()
{
    fs_ << std::endl;
    is_first_ = true;
}

CsvFile& CsvFile::operator << (CsvFile& (*val)(CsvFile&))
{
    return val(*this);
}

CsvFile& CsvFile::operator << (const char * val)
{
    return write(escape(val));
}

CsvFile& CsvFile::operator << (const std::string & val)
{
    return write(escape(val));
}

std::string CsvFile::escape(const std::string & val)
{
    std::ostringstream result;
    result << '"';
    std::string::size_type to, from = 0u, len = val.length();
    while (from < len &&
        std::string::npos != (to = val.find_first_of(special_chars_, from)))
    {
        result << val.substr(from, to - from) << escape_seq_ << val[to];
        from = to + 1;
    }
    result << val.substr(from) << '"';
    return result.str();
}

