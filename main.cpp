#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include <cmath>

struct Posting
{
    std::string doc_id;
    int doc_freq;
};
int main()
{
    std::cout << "Ensure  1) there are 20 files in the data directory" << std::endl;
    std::cout << "        2) there are stoplist.txt in the current directory" << std::endl;

    std::map<std::string, std::list<Posting> > term_docs_positions;
    std::map<std::string, int> docid_terms_in_doc;
    const char *stoplist = "stoplist.txt";
    {
        std::ifstream fin(stoplist);
        if (!fin)
        {
            std::cout << "read error:" << stoplist << std::endl;
            return -1;
        }
        std::string tmp;
        while(fin >> tmp){
            //generate the term key
            term_docs_positions[tmp];
        }
    }
    // fill term_docs_positions
    {
        for(int i=1; i<=20; i++)
        {
            std::stringstream ss;
            ss << "./data/file"<<std::setw(2)<<std::setfill('0') << i<<".txt";
            std::string file = ss.str();
            std::ifstream fin(file.c_str());
            if(!fin)
            {
                std::cout << "read error:" << file << std::endl;
                continue;
            }
            std::string tmp;
            while(fin >> tmp)
            {
                auto& content = term_docs_positions[tmp];
                auto find_it = std::find_if(content.begin(), content.end(), [&](Posting& p){
                    return p.doc_id == file;
                });
                if(find_it == content.end())
                {
                    content.push_back(Posting{file, 1});
                }
                else
                {
                    find_it->doc_freq++;
                }
            }
            std::cout << "read over:" << file << std::endl;
            
        }
    }
    // fill  docid_terms_in_doc
    {
        for(auto mit=term_docs_positions.begin(); mit!=term_docs_positions.end(); ++mit)
        {
            auto& content = mit->second;
            for(auto it=content.begin(); it!=content.end(); ++it)
            {
                docid_terms_in_doc[it->doc_id] += it->doc_freq;
            }
        }
    }
    while(true){
        std::cout<<"Input the term: ";
        std::string term;
        std::cin >> term;
        if(term == "QUIT") break;

        auto find_it = term_docs_positions.find(term);
        if(find_it == term_docs_positions.end())
        {
            std::cout << "term not found" <<std::endl;
            continue;
        }
        std::printf("%20s%20s%20s%20s%20s\n","doc_id","weighting","tf","idf","tf-idf");
        auto& content = find_it->second;
        for(auto it=content.begin(); it!=content.end(); ++it)
        {
            std::printf("%20s%20d%20f%20f%20f\n",
                it->doc_id.c_str(), 
                it->doc_freq,
                it->doc_freq/1.0/docid_terms_in_doc[it->doc_id],
                std::log10(20.0/content.size()),
                it->doc_freq/1.0/docid_terms_in_doc[it->doc_id] * std::log10(20.0/content.size())
            );
        }


    }
    
}