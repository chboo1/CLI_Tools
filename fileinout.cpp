#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sys/filio.h>



int main(int argc, char* argv[])
{
  bool in = true;
  bool out = true;
  bool quit = false;
  bool enterAfterWrite = true;
  std::string fs;
  std::string outstr;
  std::string f2;
  int input = 0;
  for (int i = 0; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      // Argument
      if (argv[i][1] == 'i')
      {
        in = true;
        out = false;
      }
      else if (argv[i][1] == 'o')
      {
        out = true;
        in = false;
      }
      else if (argv[i][1] == 'h')
      {
        std::cout << "Use:\nfio -i/-o [file\nExample:\nfio [myfile.txt\nOptions:\n-i: input only\n-o: output only\n-h: display help\n";
        break;
      }
    }
    else if (argv[i][0] == '[')
    {
      fs = argv[i];
      fs.erase(0,1);
      break;
    }
  }
  std::cout << fs << std::endl;
  std::ofstream of;
  of.open(fs, std::ios::app);
  std::ifstream ifi;
  while (!quit)
  {
    std::cout << "\033[2J\033[H";
    std::cout << outstr << std::endl;
    if (in)
    {
      ifi.open(fs);
      while (getline(ifi, f2))
      {
        std::cout << f2 << std::endl;
      }
      ifi.close();
    }
    system("stty raw");
    input = getc(stdin);
    outstr = outstr + (char)input;
    system("stty cooked");
    if (input == 13) // When enter is pressed
    {
      outstr.erase(outstr.length() - 1, outstr.length());
      if (outstr[0] == '#') // Commands like quit
      {
        outstr.erase(0,1);
        std::cout << outstr << std::endl;
        if (outstr == "quit")
        {
          quit = true;
        }
      }
      else if (outstr[0] == '!') // Actually output to the file
      {
        outstr.erase(0,1);
        of << outstr << std::endl;
      }
      outstr = "";
    }
    else if (input == 127)
    {
      outstr.erase(outstr.length() - 1, outstr.length());
      if (outstr.length() > 0)
      {
        outstr.erase(outstr.length() - 1, outstr.length());
      }
    }
  }
  of.close();
  ifi.close();
}
