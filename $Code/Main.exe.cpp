#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <chrono>

#include "StpToStl.h"

using namespace std;
namespace fs = experimental::filesystem;



///****************************************************************************
///
///****************************************************************************

int main(int argc, char *argv[])
{
  const string file_ext = "stl";


  if (argc < 2)
  {
    cout << "Convert ISO 10303 STEP file (AP203, AP214) (.stp) to StereoLithography file (.stl)" << endl;
    cout << endl << "[ jelckooo | 2018 | v1.0 ]" << endl;

    cout << endl << "StpToStl [/a] [/d deflection] [/v] source.stp [destination.stl]" << endl << endl;
    cout << "/a\t\t the generated STL file is an ASCII file" << endl << "\t\t default the generated STL file is a binary file" << endl;
    cout << "/d deflection\t linear deflection, default value is 0.001" << endl;
    cout << "/v\t\t verbose output" << endl;

    return -1;
  }

  double deflection = 0.001;
  bool ascii = false;

  fs::path path = argv[0];
  path.remove_filename();
  string current_path = path.string();

  string src_file;
  string dst_file;

  auto time_begin = chrono::steady_clock::now();

  int i = 1;
  for (; i < argc; i++)
  {
    if (argv[i][0] == '/')
    {
      switch (argv[i][1])
      {
        case 'd':
          if (argv[i][2] != 0x00) get_number(&argv[i][2], deflection);
          else
          {
            int j = i + 1;
            if ((j < argc) && get_number(&argv[j][0], deflection)) ++i;
          }
          break;

        case 'a':
          ascii = true;
          break;

        case 'v':
          is_verbose = true;
          cout << endl;
          break;

        default: break;
      }
    }
    else break;
  } // for i

  if (i < argc) src_file = argv[i]; ++i;
  if (i < argc) dst_file = argv[i]; ++i;

  if (src_file.size() == 0) return -1;

  if (dst_file.size() == 0)
  {
    fs::path file_name(src_file);

    file_name.replace_extension(file_ext);
    dst_file = file_name.string();
  }

  check_file_name(current_path, src_file);
  check_file_name(current_path, dst_file);

  int r = StpToStl(AnsiToWideString(src_file), AnsiToWideString(dst_file), deflection, ascii);

  if (is_verbose)
  {
    auto time_end = chrono::steady_clock::now();
    auto time_delta = chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin);

    cout << "Execution time: " << time_delta.count() << " ms" << endl;
  }

  return r;
}


