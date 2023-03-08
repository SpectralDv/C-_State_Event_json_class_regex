
#include <iostream>
#include <fstream>
#include <regex>

//std::regex re(R"(\")""([A-Za-z]*[^\"])""(\")");
//std::regex re{"(?:^|[\\s[:punct:]])(event)(?=$|[\\s[:punct:]])"};
//std::regex re{"([[:punct:]])""("+word+")""([[:punct:]])"};
//std::regex re{"(\")""([A-Za-z]*[^\"])""(\")"};
//std::regex re{"([A-Za-z]*[^\"\\,\\t])"};

class IFill
{
    
};

class ReadFile
{
public:
    ReadFile(){}
    ~ReadFile(){}
    std::string GetReadFile(std::string nameFile)
    {
        std::string str;
        std::string strbuf;
        std::ifstream in(nameFile);
        if(in.is_open())
        {
            while(getline(in,strbuf))
            {
                str += strbuf;
            }
        }
        in.close();
        return str;
    }
};

class Parser
{
public:
    Parser(){}
    ~Parser(){}
    void RegParser(std::string str,std::string word)
    {
        std::regex re(R"(\")""("+word+")""(\")");
        std::sregex_iterator begin{std::begin(str),std::end(str),re},end;
        std::for_each(begin,end,[begin = std::cbegin(str)](const std::smatch & matches)
        {
            const std::ssub_match & match = matches[1];
            std::cout<<match<<": "<<"start pos: "<<std::distance(begin,match.first)<<std::endl;
        });
    }
    std::string RegParser1(std::string str,std::string word)
    {
        std::regex reLeft(R"([\[])");
        std::regex reRight(R"([\]])");
        std::regex re{"(\")""("+word+")""(\"\\:\\[)"};
        std::regex reStr{"([.]*[^\"\\s])"};
        std::smatch match;

        for (std::sregex_iterator ibegin{str.begin(),str.end(),re},iend;ibegin!=iend;++ibegin)
        {
            if(std::regex_search(str, match, re)) {
                str = match.suffix();
                std::regex_search(str,match,reRight);
                str = match.prefix();
                break;
            }
        }
        return str;
    }
    void RegParser2(std::string str,std::string word)
    {
        std::smatch res;
        std::regex re{"(?:^|[\\s[:punct:]])([А-Яа-яЁё\\w]+)(?=$|[\\s[:punct:]])"};
        int i = 0;
        for (std::sregex_iterator ibegin{str.begin(),str.end(),re},iend;ibegin!=iend;++ibegin)
        {
            if (ibegin->str(1)==word)
            {
                std::cout<<ibegin->str(1)<<": "<<i<<"\n";
            }
            i++;
        }
    }
    void RegParser3(std::string str,std::string word)
    {
        std::regex reLeft(R"([\[])");
        std::regex reRight(R"([\]])");
        std::smatch res;

        std::regex_search(str,res,reLeft);
        str = res.suffix();
        std::cout<<res[0]<<"\n";

        std::regex_search(str,res,reRight);
        str = res.prefix();
        std::cout<<res[0]<<"\n";

        std::cout<<str<<"\n\n";
    }
};

class ModelSTA
{
private:
    std::vector<std::string> vState;
public:
    ModelSTA(){}
    ~ModelSTA(){}
private:
    void AddState(std::string state)
    {
        vState.emplace_back(state);
    }
public:
    void Parser(std::string str)
    {
        std::regex re{"([A-Za-z0-9]*[^\"\\,\\s])"};
        std::smatch match;

        for (std::sregex_iterator ibegin{str.begin(),str.end(),re},iend;ibegin!=iend;++ibegin)
        {
            if(std::regex_search(str, match, re)) {
                AddState(match[0]);
                str = match.suffix();
            }
        }
    }
    int GetCountState()
    {
        return vState.size();
    }
    std::string GetState(int index)
    {
        return vState[index];
    }
};

int main()
{
    Parser *p = new Parser;
    ReadFile *rf = new ReadFile;
    ModelSTA *mSTA = new ModelSTA;

    std::cout<<"1\n";
    mSTA->Parser(p->RegParser1(rf->GetReadFile("state_event.json"),"event"));
    std::cout<<"\n";

    for (int i = 0; i < mSTA->GetCountState(); i++)
    {
        std::cout<<mSTA->GetState(i)<<" ";
    }
    
    getchar();
    return 0;
}
