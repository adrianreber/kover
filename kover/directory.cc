//
// File: directory.h
// Created by: adrian <adrian@lisas.de>
// Created on: Thu Jan 30 17:28:51 2003
//

#include "directory.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

directory::directory(string current_dir)
{
    struct stat stat_struct;
    struct dirent *dirent_struct;
    DIR *dir = NULL;
    string full_path;
    int start = 0;
    int stop = 0;
    
    current_dir = normalize_path(current_dir);
  
    if ((current_dir.find("/dev/")==0) || (current_dir.find("/proc/")==0))
        return;
    
    stop = current_dir.rfind("/") - 1;
    start = current_dir.rfind("/",stop);
    short_name = current_dir.substr(start+1,stop-start);
    
    this->current_dir = current_dir;
    /* does the directory exist */
    if (stat(current_dir.c_str(), &stat_struct) == -1)
        return;
    dir = opendir(current_dir.c_str());
    if (!dir)
        return;
    dirent_struct = readdir(dir);
    while (dirent_struct) {
        full_path = current_dir;
        full_path += dirent_struct->d_name;
        if (is_dir(full_path) && !(!strcmp(dirent_struct->d_name, ".")))
            directories.push_back(dirent_struct->d_name);
		  if (!is_dir(full_path))
			   files.push_back(dirent_struct->d_name);
        dirent_struct = readdir(dir);
    }
    
    directories.sort();
}

bool directory::is_dir(string dir)
{
    struct stat stat_struct;

    stat(dir.c_str(), &stat_struct);
    if (stat_struct.st_mode & S_IFDIR)
        return true;
    return false;
}

directory::~directory()
{
    printf("deleted: %s\n",current_dir.c_str());
}

bool directory::has_childs()
{
    if (childs.size())
        return true;
    return false;
}

int directory::how_many()
{
    return (int) directories.size();
}

int directory::how_many_files()
{
    return (int) files.size();
}

int directory::how_many_childs()
{
    return (int) childs.size();
}

string directory::get_directory(int id) {
    if (id < 0 || id >= how_many())
        return string();
    list <string> :: iterator dir;
    int i = 0;
    for (dir = directories.begin(); dir != directories.end(); dir++) {
        if (i++ == id)
            return *dir;
    }
    return string();
}

string directory::get_file(int id) {
    if (id < 0 || id >= how_many_files())
        return string();
    list <string> :: iterator file;
    int i = 0;
    for (file = files.begin(); file != files.end(); file++) {
        if (i++ == id)
            return *file;
    }
    return string();
}

directory* directory::get_childs(int id) {
    if (id < 0 || id >= how_many_childs())
        return NULL;
    list <directory *> :: iterator dir;
    int i = 0;
    for (dir = childs.begin(); dir != childs.end(); dir++) {
        if (i++ == id)
            return *dir;
    }
    return NULL;
}

string directory::get_current()
{
    return current_dir;
}

string directory::get_short_name() {
    return short_name;
}

void directory::make_childs()
{
    childs.clear();
    list <string> :: iterator dir;
    int i = 0;
    for (dir = directories.begin(); dir != directories.end(); dir++) {
        childs.push_back(new directory(current_dir + *dir));
    }
}

string directory::normalize_path(string path) {
    if (path.substr(path.size()-1,path.size()).compare("/"))
        path += "/";
    return path;
}


directory *directory::get_child_by_path(string path) {
    path = normalize_path(path);
    list <directory *> :: iterator dir;
    for (dir = childs.begin(); dir != childs.end(); dir++) {
        printf("%s:%s:%d\n",(*dir)->get_current().c_str(),
                path.c_str(),path.size());
        if (!(*dir)->get_current().compare(0,path.size(),path))
            return (*dir);
    }
    return this;
}
