//
// File: directory.h
// Created by: adrian <adrian@lisas.de>
// Created on: Thu Jan 30 17:28:51 2003
//

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include <config.h>
#include <string>
#include <list>
using namespace std;

class directory
{
	public:
		directory(string current_dir);
		 ~directory();
	
	    bool has_childs();
        int how_many();
        int how_many_files();
        
        string get_directory(int id);
        string get_file(int id);
        string get_current();
        void make_childs();
    directory* get_childs(int id);
    int how_many_childs();
        string get_short_name();
        directory *get_child_by_path(string path);
		
	protected:
        string current_dir;
		list <string> directories;
		list <string> files;
        list <directory *> childs;
        bool is_dir(string dir);
        string short_name;
    
        string normalize_path(string path);
};


#endif	//_DIRECTORY_H_

