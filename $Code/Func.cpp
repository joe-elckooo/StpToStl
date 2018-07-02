#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <chrono>
#include <windows.h>
#include <io.h> 
#include <fcntl.h>

#include <TopoDS_Shape.hxx>
#include <STEPControl_Reader.hxx>
#include <StlAPI_Writer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

using namespace std;
namespace fs = experimental::filesystem;


bool is_verbose = false;

///****************************************************************************
///
///****************************************************************************
bool get_number(char *str, double& value)
{
  size_t i;
  double val = stod(str, &i);

  bool r = str[i] <= 0x20;
  if (r) value = val;

  return r;
}

///****************************************************************************
///
///****************************************************************************
bool check_file_name(const string current_path, string &file_name)
{
  fs::path p = file_name;
  if (p.is_absolute()) return true;

  fs::path fn(current_path);
  fn /= p;

  file_name.assign(fn.string());

  return false;
}

///****************************************************************************
///
///****************************************************************************
wstring AnsiToWideString(const string value, UINT code_page = CP_ACP)
{
  wstring str(value.length(), 0);
  MultiByteToWideChar(code_page, 0, value.c_str(), value.length(), &str[0], value.size());

  return str;
}

///****************************************************************************
///
///****************************************************************************
string WideToUtf8String(const wstring value)
{
  int size = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), value.length(), nullptr, 0, nullptr, nullptr);
  string str(size, '\0');
  WideCharToMultiByte(CP_UTF8, 0, value.c_str(), value.length(), &str[0], size, nullptr, nullptr);

  return str;
}

///****************************************************************************
///
///****************************************************************************
string AnsiToUtf8String(const string value, DWORD code_page = CP_ACP)
{
  return WideToUtf8String(AnsiToWideString(value, code_page));
}


///****************************************************************************
///
///****************************************************************************
int StpToStl(const wstring stp_file, const wstring stl_file, double deflection = 0, bool ascii = false)
{

  if (is_verbose)
  {
    int old_mode = _setmode(_fileno(stdout), _O_U16TEXT);

    wcout << "STP file: " << stp_file << endl;
    wcout << "STL file: " << stl_file << endl;

    _setmode(_fileno(stdout), old_mode);

    cout << "Deflection value: " << deflection << endl;
    cout << "ASCII mode: " << (ascii ? "true" : "false") << endl << endl;

    cout << "Reading the STEP file ..." << endl;
  }

  //-----> read data from STEP file
  STEPControl_Reader Reader;
  IFSelect_ReturnStatus stp_status = Reader.ReadFile(WideToUtf8String(stp_file).c_str());

  if (is_verbose)
  {
    const char *result;

    switch (stp_status)
    {
      case IFSelect_RetVoid: result = "normal execution which created nothing, or no data to process";
        break;
      case IFSelect_RetDone: result = "successfully";
        break;
      case IFSelect_RetError: result = "error: in command or input data, no execution";
        break;
      case IFSelect_RetFail: result = "error: execution was run and has failed";
        break;
      case IFSelect_RetStop: result = "error: by reason of end or stop (such as Raise)";
        break;
      default: result = "error: unknown";
        break;
    }

    cout << "\t result: " << result << endl;
  }

  if (stp_status > IFSelect_RetDone) return 1;

  Standard_Integer roots = Reader.NbRootsForTransfer();
  Standard_Integer transfer = Reader.TransferRoots();
  TopoDS_Shape shape = Reader.OneShape();

  //-----> write data to STL file
  StlAPI_Writer Writer = StlAPI_Writer();
  Writer.ASCIIMode() = ascii;

  if (deflection <= 0) deflection = 0.001;

  if (is_verbose)
  {
    cout << "Building the mesh of a shape with respect of their correctly triangulated parts ..." << endl;
  }

  BRepMesh_IncrementalMesh mesh(shape, deflection, true);
  mesh.Perform();

  if (!mesh.IsDone())
  {
    if (is_verbose)
    {
      cout << "\t result: error performing triangualtion" << endl;
    }

    return 2;
  }

  if (is_verbose)
  {
    cout << "\t result: successfully" << endl;
    cout << "Converting a shape to STL format and writing it to file ..." << endl;
  }

  StlAPI_ErrorStatus stl_status = Writer.Write(shape, WideToUtf8String(stl_file).c_str());

  if (is_verbose)
  {
    const char *result;

    switch (stl_status)
    {
      case StlAPI_StatusOK: result = "successfully";
        break;
      case StlAPI_MeshIsEmpty: result = "error: mesh is empty";
        break;
      case StlAPI_CannotOpenFile: result = "error: can not open file";
        break;
      default: result = "error: unknown";
        break;
    }

    cout << "\t result: " << result << endl;
  }

  if (stl_status > StlAPI_StatusOK) return 3;

  return 0;
}


///****************************************************************************
///
///****************************************************************************
extern "C" __declspec(dllexport) int Test(const char *value)
{
  return 314;
}


///****************************************************************************
///
///****************************************************************************
extern "C" __declspec(dllexport) int StpToStlW(const wchar_t *stp_file, const wchar_t *stl_file, double deflection = 0, bool ascii = false)
{
  return StpToStl(wstring(stp_file), wstring(stl_file), deflection, ascii);
}


