#ifndef IODEFINESANDCONSTS_H
#define IODEFINESANDCONSTS_H

#define PutLine() PutDelimitedLine(f,c0,c1,esc,v)
#define PutLines() PutDelimitedFile(f,c0,c1,esc,subaccounts)
#define GetLine() GetDelimitedLine(f,c0,c1,esc,v)
#define GetLines() GetDelimitedFile(f,c0,c1,esc,file)
#define FlushInputBuffer() while((junk=getchar()) != '\n' && junk != EOF)

const char c0 = '\0';
const int c1 = '\n';
const int esc = '`';

#endif
