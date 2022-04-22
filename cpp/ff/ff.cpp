/*
 * TODO list in README file
 *
 *
 * requirements:
 * file-finder <dir> <substring1>[<substring2> [<substring3>]...]
 * The application takes the directory to search as the first command line argument, followed by
 * one or more substrings (literals, not wildcards) to search for.
 * For example:
 * file-finder tmp aaa bbb ccc
 *    This would search the directory tree rooted at “tmp” for filenames (only the filename, not the rest of the path) that contain “aaa” “bbb” or “ccc” anywhere in them.
 *    Each filename that matches a substring gets added to a container of some type to record them.
 *
 *    XXX Note to Aaron: general assumptions:
 *    1. this means not exact match, so filename 'aaabbbc' matches both 'aaa' and 'bbb' substrings
 *    2. this is only filename search, not file content search. 
 *    3. only SINGLE 'result container' for all the substrings search result, so the dumping will show as:
 *       'current found files with name containing the str from the given substr list:...'
 */
/*
 * design:
 * 0. as required, we need to start new thread for each substring to do filename search
 * 1. so, we shall not let each thread do the file info collecting again and again,
 *    instead, we shall do GetAllFiles() once for all, before all working threads
 * 2. GetAllFiles() will work recursively, add all found files into local db
 * 3. specially:
 * 3.1 such db shall be a std::map<std::string filename, std::vector<std::string path> pathlist>,
 *     key:filename, value: the list of paths of all found files with that name 
 * 3.2 the matching is btwn found filenames ('text') and substring('pattern'): 
 * 3.2.1 it's NOT complete match or reversely, so cannot use the Aho-Corasic linear way
 * 3.2.2 build the found filenames into a tree/trie will not help, since filenames are NOT patterns.
 * 3.2.3 this means we have to foreach the found filenames and match substring(pattern) to them 
 * 3.2.4 so the only trick that can speed up is: KMP for such one-vs-one match
 * 3.3 in conclusion: the logic is:
 * 3.3.1 build the files info into a map
 * 3.3.2 for each substring, foreach the keys of the map and match, using KMP to speed up.
 * 4. also for each search task:
 * 4.1 using pthread to create the threads for each task
 * 4.2 using mutext to protect the gResult, shared and modified by all thread, to avoid race
 * 4.3 main proc will join and wait for each thread to finish
 * 4.4 TODO upon external signal (stop or kill), main proc shall kill all threads too
 * 4.5 TODO another thread to call DumpResult periodically
 * 5. TODO main proc input and singal handling...
 *
 */

// XXX ugly mixed use of c and c++ headers...
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <assert.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

const string gUsage = "Usage: \n \
file-finder [options] [dirname] [filename1, filename2 ...]\n \
find all the files containing any string from filenames list under specified dir and subdirs.\n \
options:\n \
-h, --help  show help msg and exit\n";

// XXX we don't have race here for such data,
// since it's updated only in the recursively called GetAllFiles.
// and the multi-thread access is after GetAllFiles and is just read-only
map<string, vector<string> > gFileDB;

// XXX this gResult will be protected by gMutex for multi-thread access
map<string, vector<string>* > gResult;

class Options
{
    public:
        string m_dir;
        vector<string> m_filenames;
};
Options gOpt;

int GetOptions(int argc, char **argv)
{
    int opt;

    if (argc <= 1)
    {
        cerr << gUsage;
        exit(EXIT_FAILURE);
    }

    // only -h so far...
    while ((opt = getopt(argc, argv, "h")) != -1) 
    {
        switch (opt) {
            case 'h':
                cerr << gUsage;
                exit(EXIT_SUCCESS);
            default:
                cerr << gUsage;
                exit(EXIT_FAILURE);
        }
    }

    // either -h or just dir filenames...
    if (argc < 3)
    {
        cerr << gUsage;
        exit(EXIT_FAILURE);
    }

    gOpt.m_dir = argv[optind++];
    for (int i = optind; i < argc; i++)
        gOpt.m_filenames.push_back(argv[i]);

    return true;
}

// XXX simply assumming either regular file or dir,
// ignoring all else: symlinks, blocks, etc...
bool IsDir(const string path) 
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        return true;

    return false;
}

// just a beautiful path '/path1/path2/filedir' vs '/path1/./../path2/filedir'
void GetRealPath(string& path) 
{
    // XXX too much for each... better global?
    char tmppath[PATH_MAX];
    char * ptr = realpath(path.c_str(), tmppath);
    if (!ptr) {
        cerr << " Error: failed to get real path of: " << path << ", reason: " << strerror(errno) << endl;
        // ignored this...
        return;
    }

    path = ptr;
}

void GetAllFiles(string& path, const string name) 
{
    GetRealPath(path);

    if (!IsDir(path)) 
    {
        // found file, add into db
        // safe, recursively not concurrently...
        gFileDB[name].push_back(path);

        return;
    }

    struct dirent *dp;
    DIR *dir = opendir(path.c_str());
    if (dir == NULL){
        cerr << " Error: failed to open dir: " << path << ", reason: " << strerror(errno) << endl;
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // construct new path from our base path
            string newpath = path + "/" + dp->d_name;

            GetAllFiles(newpath, dp->d_name);
        }
    }

    closedir(dir);
}

bool BuildFilesDB() 
{
    char path[PATH_MAX];
    string dirpath = gOpt.m_dir;
    if (dirpath[0] != '/')
    {
        // relative, get full path
        if (getcwd(path, sizeof(path) - dirpath.length() - 1) != NULL) {
            dirpath = string(path) + "/" + gOpt.m_dir;
        }
    }

    GetAllFiles(dirpath, dirpath);

    return true;
}

void DumpFileDB()
{
    cout << "Files found under dir: " << gOpt.m_dir << endl;
    for (const auto& [key, value] : gFileDB)
    {
        cout << "name: " << key << ", paths:" << endl;
        for (const auto& path : value)
        {
            cout << "\t" << path << endl;
        }
    }
}

void DumpResult()
{
    cout << "Files found naming matching given substrs under dir: " << gOpt.m_dir << endl;
    for (const auto& [key, value] : gResult)
    {
        cout << "name: " << key << ", paths:" << endl;
        for (const auto& path : *value)
        {
            cout << "\t" << path << endl;
        }
    }
}

// KMP codes
int* KMP_BuildLps(const string pattern) 
{
    int n = pattern.length();
    int* lps = new int[n];
    lps[0] = 0;
    // just KMP, off topic, no verbose comments to explain....
    for (int i = 0; i < n-1; i++) 
    {
        int* p = &lps[i+1];
        *p = lps[i] + 1;
        // lps[i]=x, then here decided by p[i+1] ?= p[x]
        while (*p > 0 && pattern[i+1] != pattern[*p-1])
        {
            // next best: lps[x]+1
            int next = lps[*p-1] + 1;
            if (*p == next)
                *p = 0;
            else
                *p = next;
        }
    }
    return lps;
}

bool KMP_Search(const string pattern, const string text, int* lps) 
{
    int n = pattern.length();
    int m = text.length();
    for (int i = 0, j = 0; i < m; i++)
    {
        if (pattern[j] == text[i]) 
        {
            j++;
            // one match is enough
            if (j == n)
                return true;
        } 
        else if (j > 0)
        {
            j = lps[j-1];
            // revisit i
            i--;
        }
        // else such i cannot be found anywhere.
    }

    return false;
}

// match pattern against text, return true at once for first match
bool StrMatch(const string text, const string pattern) 
{
    // to speed up, using KMP, which is linear, and will be helpful
    // if text is much longer > pattern
    // XXX yet here not rather economic... most of time text is not so long... and we are not searching for multi matching...
    int* lps = KMP_BuildLps(pattern);
    bool ret = KMP_Search(pattern, text, lps);
    delete []lps;
    return ret;
}

// thread codes

// threadinfo passed from proc into threads
typedef struct 
{
    // id from pthread_create()
    pthread_t thread_id;
    // local defined thread number
    int       thread_num;
    // substring for search
    string    substring;
} ThreadInfo;

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

// worker thread for real search task
void* DoSearchTask(void *arg) 
{
   ThreadInfo *tinfo = (ThreadInfo*)arg;
   string substr = tinfo->substring;

   for (const auto& [key, value] : gFileDB)
   {
       if (StrMatch(key, substr))
       {
           if (!gResult.count(key)) 
           {
               // only set if not already found by other threads
               
               // only acquire lock when needs update the gResult.
               pthread_mutex_lock(&gMutex);

               cout << "In thread: " << tinfo->thread_num << " for substring: " << substr << ", updating gResult" << endl;

               gResult[key] = &gFileDB[key];

               cout << "In thread: " << tinfo->thread_num << "for substring: " << substr << ", updated gResult, now size: " << gResult.size() << endl;

               pthread_mutex_unlock(&gMutex);
           }
       }
   }

   pthread_exit(NULL);
}

// do the file search for gOpt.m_filenames, for each of them:
//  start a new thread to do searh against gFileDB, store result into gResult
//  XXX wrap the pthread calling into a Thread class and a Lock class
void DoWork()
{

    int rc, opt, num_threads;

    pthread_attr_t attr;
    int stack_size;
    void *res;

    num_threads = gOpt.m_filenames.size();

    // init thread creation attributes 
    rc = pthread_attr_init(&attr);
    if (rc != 0)
    {
        cerr << "Failed in pthread_attr_init" << endl;
        exit(EXIT_FAILURE);
    }

    // init the mutext locker
    if (pthread_mutex_init(&gMutex, NULL) != 0)
    {
        cerr << "Failed in pthread_mutex_init" << endl;
        exit(EXIT_FAILURE);
    }
  

    ThreadInfo *tinfo = new ThreadInfo[num_threads];

    // one thread for each search substring
    for (int i = 0; i < num_threads; i++) {
        tinfo[i].thread_num = i + 1;
        tinfo[i].substring = gOpt.m_filenames[i];

        rc = pthread_create(&tinfo[i].thread_id,
                            &attr,
                            &DoSearchTask,
                            &tinfo[i]);
        if (rc != 0)
        {
            cerr << "Failed in pthread_create" << endl;
            exit(EXIT_FAILURE);
        }
    }

    pthread_attr_destroy(&attr);

    // now join with each thread
    // XXX not recall clearly, here shall handle waiting?
    for (int i = 0; i < num_threads; i++)
    {
        rc = pthread_join(tinfo[i].thread_id, &res);
        if (rc != 0)
        {
            cerr << "Failed in pthread_join" << endl;
            exit(EXIT_FAILURE);
        }

        cout << "Joined with thread: " << tinfo[i].thread_num << endl;
    }

    delete [] tinfo;

    pthread_mutex_destroy(&gMutex);
}

int main(int argc, char *argv[])
{
    int retval = 0;

    if (!GetOptions(argc, argv))
    {
        exit(EXIT_FAILURE);
    }

    // if (geteuid() != 0)
    //     std::cerr << argv[0] << ": WARNING euid not root, may fail to run...\n";

    // get all the files info under gOpt.m_dir 
    if (!BuildFilesDB())
    {
        cerr << " Error: failed to build file info db...\n";
        exit(EXIT_FAILURE);
    }

    // DumpFileDB();

    DoWork();
    DumpResult();

    exit(EXIT_SUCCESS);
}
