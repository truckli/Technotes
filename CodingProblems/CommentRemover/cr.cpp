#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

class CommentEraser {
    public:
        static void erase(const char *in, char *out, size_t len);
};

void CommentEraser::erase(const char *in, char *out, size_t len)
{
    if (!in || !out || !len) return;

    const char *p = in, *pend = in + len;
    char *q = out, oldc = 0;
    bool in_dq = false, in_sq = false, in_esc = false;
    bool in_lc = false, in_bc = false;
    
    //escape sign must be in either dq or sq
    //' can be in dq without escaping, be in sq with escaping
    //" can be in dq with escaping, be in sq with or without escaping, which means, " is no quotation sigh when in sq or when escaped
    while (p != pend) {
       char c = *p; 
       if (in_lc) {
           if ('\n' == c) {
               in_lc = false;
               *q++ = '\n';
           }
       } else if (in_bc) {
           if ('/' == c && '*' == oldc) {
               in_bc = false;
           }
       } else { //not in comment
           if ('\\' == c) {//backslash
                   in_esc = !in_esc;
           } else { // not backslash 
               if ('\'' == c) { //'
                   if (!in_esc && !in_dq) {
                       in_sq = !in_sq;//true single quotation
                   }
               } else if ('"' == c) {
                   if (!in_esc && !in_sq)
                       in_dq = !in_dq;//true double quotation
               } else if ('/' == c) {
                   if (!in_dq && '/' == oldc)
                       in_lc = true;
               } else if ('*' == c) {
                   if (!in_dq && '/' == oldc)
                       in_bc = true;
               }
               in_esc = false;
           }
           if (in_lc || in_bc)//new comment 
               q--;//unwrite '/'
           else
               *q++ = c;
       }//not in comment
       oldc = c;
       p++;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " filename " << endl;
    }

    ifstream is(argv[1], ios::binary);
    if (!is) {
        cout << "Cannot open file " << argv[1] << "!" << endl;
    }
    
    struct stat astat;
    if (stat(argv[1], &astat) < 0) {
        cout << "Cannot get length of file " << argv[1] << "!" << endl;
    }
    
    size_t insize = astat.st_size;//file size
    char *inbuf = new char[insize]; 

    is.read(inbuf, insize);
    is.close();

    char *outbuf = new char[insize + 1];//last byte for \0
    CommentEraser::erase(inbuf, outbuf, insize); 
    outbuf[insize]  = 0;
    cout << outbuf << endl;
    return 0;
}


