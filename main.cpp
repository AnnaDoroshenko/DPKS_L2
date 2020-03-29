#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <utility>


struct Argument
{
    private:
        std::string longName;
        std::string shortName;
        std::optional<std::vector<unsigned int>> numArgs;

    public:
        Argument(const std::string& longName, const std::string& shortName,
                const std::optional<std::vector<unsigned int>>& numArgs)
                : longName(longName), shortName(shortName), numArgs(numArgs) {}

        friend std::ostream& operator<<(std::ostream& os, const Argument& arg);

        bool operator==(const Argument& arg) const
        {
            return (arg.longName == longName);
        }
};


std::ostream& operator<<(std::ostream& os, const Argument& arg);
std::pair<std::string, unsigned int> putifyArg (const std::string& arg);
void retrieveArg (const std::string& arg, std::vector<Argument>& argbox);
void printUnique (const std::vector<Argument>& args);
bool contains (const std::string& arg, char ch);


std::ostream& operator<<(std::ostream& os, const Argument& arg)
{
    os << "Arg: " << arg.longName;
    if (arg.numArgs)
    {
        os << " with subargs: ";
        for (unsigned int i = 0; i < arg.numArgs->size(); i++)
        {
            os << (*arg.numArgs)[i] << " ";
        }
    }
    
    return os;
}


std::pair<std::string, unsigned int> putifyArg (const std::string& arg) 
{
    const unsigned int size = arg.size();
    unsigned int pos = 0;

    while (arg.at(pos) == '-') pos++;

    return {arg.substr(pos, size - pos), pos};
}


bool contains (const std::string& arg, char ch)
{
    for (unsigned int i = 0; i < arg.size(); i++)
    {
        if (arg[i] == ch) return true;
    }

    return false;
}


void retrieveArg (const std::string& arg, std::vector<Argument>& argbox)
{
    std::optional <std::vector<unsigned int>> numArgs = std::nullopt;
    std::string mainarg;
    if (contains(arg, '='))
    {
        const std::size_t pos = arg.find("=");
        const std::string subarg = arg.substr(pos + 1, arg.size() - pos - 1);
        mainarg = arg.substr(0, pos);
        if (mainarg == "--help" || mainarg == "--version")
        {
            std::cout << "No subargs are allowed for arg: " << mainarg << std::endl;
            return;
        }
        std::vector<unsigned int> subargs;
        for (unsigned int c = 0; c < subarg.size(); c++)
        {
            const char current = subarg[c];
            if (current == ',') continue;
            subargs.push_back(current - '0');
        }
        numArgs = {subargs};
    } else mainarg = arg;
    if (mainarg == "--help" || mainarg == "-h" || mainarg == "?") argbox.push_back(Argument("help", "h", numArgs));
    else if (mainarg == "--version" || mainarg == "-V") argbox.push_back(Argument("version", "V", numArgs));
    else if (mainarg == "--list" || mainarg == "-l") argbox.push_back(Argument("list", "l", numArgs));
    else if (mainarg == "--value" || mainarg == "-v") argbox.push_back(Argument("value", "v", numArgs));
    else
    {
        const std::pair<std::string, unsigned int> purified = putifyArg(mainarg);
        if (purified.second >= 2)
        {
            std::cout << "Invalid arg: " << arg << std::endl;
            return;
        }
        const std::string& pureArg = purified.first;

        for (unsigned int i = 0; i < pureArg.size(); i++)
        {
            const auto argList = (i == pureArg.size() - 1) ? numArgs : std::nullopt;
            if (pureArg[i] == 'h')
            {
                if (argList) std::cout << "No subargs are allowed for arg: -h" << std::endl;
                else argbox.push_back(Argument("help", "h", argList));
            }
            else if (pureArg[i] == 'V') 
            {
                if (argList) std::cout << "No subargs are allowed for arg: -V" << std::endl;
                else argbox.push_back(Argument("version", "v", argList));
            }
            else if (pureArg[i] == 'l') argbox.push_back(Argument("list", "l", argList));
            else if (pureArg[i] == 'v') argbox.push_back(Argument("value", "v", argList));
            else std::cout << "Invalid arg: -" << pureArg[i] << std::endl;
        }
    }  
}


void printUnique (const std::vector<Argument>& args)
{
    for (unsigned int i = 0; i < args.size(); i++)
    {
        bool unique = true;
        for (unsigned int j = 0; j < args.size(); j++)
        {
            if (unique && (i == j)) std::cout << args[i] << std::endl;
            if (args[i] == args[j]) unique = false;
        }
    }
}


int main (int argc, char* argv[])
{
    std::vector<Argument> arguments;

    for (unsigned int a = 1; a < argc; a++)
    {
        retrieveArg(std::string(argv[a]), arguments);
    }

    printUnique(arguments);

    return 0;
}