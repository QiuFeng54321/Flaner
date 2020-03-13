#include <lexer/utility.hh>
#include <string>

namespace flaner
{
namespace lexer
{
namespace util
{
    bool isBlank(wchar_t ch)
    {
        std::wstring blanks = L"\n\r\t\f \x0b\xa0\u2000"
            "\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009"
            "\u200a\u200b\u2028\u2029\u3000";
        for (auto i = blanks.begin(); i != blanks.end(); i++)
        {
            if (*i == ch)
            {
                return true;
            }
        }
        return false;
    }

}
}
}
