#include "getopt.h"

#include "embed_filesystem.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

//#define EMBED_STD_FILESYSTEM 1
//#define EMBED_EXPERIMENTAL_FILESYSTEM 2
//
//#if EMBED_USE_FILESYSTEM == EMBED_STD_FILESYSTEM
//#include <filesystem>
//namespace fs=std::filesystem;
//#else
//#include <experimental/filesystem>
//namespace fs=std::experimental::filesystem;
//#endif

#include <regex>

bool loadFileToString(std::string path, std::string fileName, std::string &buffer);
void writeString(std::ofstream &file, std::string &stringVar);
void replaceAll(std::string &str, const std::string &from, const std::string &to);
std::vector<std::string> parseNamespace(std::string parse);

int main(int argc, char **argv)
{
    int option;
    std::string inputFile;
    std::string outputFile;
	std::string nameSpace;
    std::ostringstream error;

    while((option=getopt(argc, argv, "i:n:"))!=-1)
    {
        switch(option)
        {
        case 'i':
            inputFile=optarg;
            break;
		case 'n':
            nameSpace=optarg;
            break;
        case ':':
            switch(optopt)
            {
            case 'i':
                error<<"Option -i requires a file.\n";
                break;
			case 'n':
                error<<"Option -n requires a namespace.\n";
                break;
            }
            break;
        case '?':
            error<<"Unrecognized option: '"<<optopt<<"'\n";
            break;
        }
    }

    if((argc-optind)==1)
    {
        outputFile=argv[optind];
    }
    else
    {
        if((argc-optind)==1)
        {
            error<<"Too many files listed:";

            for(int index=optind; index<argc; ++index)
                error<<argv[index]<<" ";
            error<<"\n";
        }
        else
            error<<"Not output file given\n";
    }

    std::string errorString=error.str();

    if(!errorString.empty())
    {
        std::cout<<errorString;
        return 1;
    }

    embed::fs::path filePath(inputFile);
    std::string path=filePath.parent_path().string();
    std::string fileName=filePath.filename().string();
    std::string source;
    std::string fileNameVar=fileName;

	std::replace(fileNameVar.begin(), fileNameVar.end(), '.', '_');

    if(!loadFileToString(path, fileName, source))
    {
        std::cout<<"Failed to read file "<<inputFile<<"\n";
        return 1;
    }

    std::ofstream file;

    file.open(outputFile.c_str());
    if(file.fail())
    {
        std::cout<<"Failed to write file "<<outputFile<<"\n";
        return 1;
    }

    file<<"#include <string>\n\n";

    std::vector<std::string> namespaces;
	
    if(!nameSpace.empty())
	{
        namespaces=parseNamespace(nameSpace);

        for(size_t i=0; i<namespaces.size(); ++i)
        {
            file<<"namespace "<<namespaces[i];
            if(i==namespaces.size()-1)
                file<<"\n";
            file<<"{";
        }
        file<<"\n\n";

//		file<<"namespace ";
//		file<<nameSpace;
//		file<<"\n{\n\n";
	}
	file<<"std::string "<<fileNameVar<<"=\n";
    writeString(file, source);
	file<<";\n";
    if(!namespaces.empty())
    {
        file<<"\n";
        for(size_t i=0; i<namespaces.size(); ++i)
            file<<"}";
        file<<"\n";

//        file<<"\n}\n";
    }

    return 0;
}

bool loadFileToString(std::string path, std::string fileName, std::string &buffer)
{
	std::string filePath;

	if(!path.empty())
		filePath=path+'/'+fileName;
	else
		filePath=fileName;

	FILE *file=fopen(filePath.c_str(), "r");

	if(file!=NULL)
	{
		int capacity=1024;
		char *localBuffer=(char *)malloc(capacity);
		std::regex expression("^\\s*#include\\s*[\"<]\\s*([\\w.]+)\\s*[\">]\\s*?");

		while(fgets(localBuffer, capacity, file)!=NULL)
		{
			while(strlen(localBuffer)==capacity-1)
			{
				//read to the end with a linefeed implying line was exactly capacity-1 length
				if(localBuffer[capacity-2]=='\n')
					break;

				localBuffer=(char *)realloc(localBuffer, capacity+1024);

				if(localBuffer==NULL)
					return false;

				if(fgets(&localBuffer[capacity-1], 1025, file)==NULL)
					break;
				capacity+=1024;
			}

			std::cmatch match;

			if(regex_match(localBuffer, match, expression))
				loadFileToString(path, match[1], buffer);
			else
				buffer+=localBuffer;
		}
		fclose(file);
		free(localBuffer);
		return true;
	}
	return false;
}

void writeString(std::ofstream &file, std::string &stringVar)
{
    size_t pos=0;
    size_t findPos;
	std::string value;

    while((findPos=stringVar.find('\n', pos))!=std::string::npos)
    {
		value=stringVar.substr(pos, findPos-pos);
		replaceAll(value, "\"", "\\\"");
		file<<"\""<<value<<"\\n\"\n";
        pos=findPos+1;
    }
	value=stringVar.substr(pos, findPos-pos);
	replaceAll(value, "\"", "\\\"");
    file<<"\""<<stringVar.substr(pos)<<"\\n\"";
}

void replaceAll(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos=0;

	while((start_pos=str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos+=to.length();
	}
}

std::vector<std::string> parseNamespace(std::string parse)
{
    std::vector<std::string> namespaces;
    size_t pos=0, findPos=0;

    while((findPos=parse.find("::", pos))!=std::string::npos)
    {
        namespaces.push_back(parse.substr(pos, findPos-pos));
        pos=findPos+2;
    }
    namespaces.push_back(parse.substr(pos, parse.size()-pos));
    return namespaces;
}