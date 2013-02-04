#include <math.h>
#include <iostream>
#include <cstring.h>
#include <fstream>
#include <conio>
#include <stdio.h>

int main(void)
{
   int width,height;
   int data;
   int count=0;
   int layer=0;
   string str;
   string name;
   string nameofmap;
   //char inputfile[20];
   char outputfile[20];

  //cout <<"Type in the file to open";
  //cin >> nameofmap;
  cout <<"Type in layer #";
  cin >> layer;
  cout <<"Type in layer name";
  cin >> name;

  for(int x=0; x< name.length();x++)
   outputfile[x]=name[x];

  //for(int x=0; x< nameofmap.length();x++)
  // inputfile[x]=nameofmap[x];

   outputfile[name.length()]='.';
   outputfile[name.length()+1]='h';

   //inputfile[nameofmap.length()]='.';
   //inputfile[nameofmap.length()+1]='h';

   ifstream inFile("map.txt");
   ofstream outFile(outputfile, ios::out);



for(int i=0; i<=layer;i++)
  {
   inFile >> str;
   while(str!="Layer:")
    inFile >> str;
  }

   inFile >> str;
   cout << str;


   //getch();

   inFile >> width;
   inFile >> height;

	outFile << "const unsigned char "<< name <<"[" << height << "][" << width <<"]={" <<'\n';

   for(int a=0; a<width;a++)
    {
    for(int b=0; b<height; b++)
     {
      inFile>> data;

      outFile << "0x";

          for (int a=1; a>=0; a--)    //2*sizeof(int) - 1
            outFile << "0123456789ABCDEF"[(((data) >> a*4) & 0xF)];




     outFile << ", ";
      //getch();

      count++;
      if(count==16)
      {
      count=0;
      outFile<<'\n';
      }
     }

    }
    outFile << "};" <<'\n';






	outFile.close();
   inFile.close();

	return 0;
}