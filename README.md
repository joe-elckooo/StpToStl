# About

StpToStl an utility to Convert ISO 10303 STEP file (AP203, AP214) (.stp) to StereoLithography file (.stl)

## Dependencies

[OpenCascade](https://github.com/tpaviot/oce.git)

## Compiling

1. Get the source and compile Open CASCADE library
  [OpenCascade](https://github.com/tpaviot/oce.git)
2. Get the source StpToStl
```
..
oce
StpToStl
```
3. Run `StpToStl\get_libs.bat` to get the necessary dll libraries
4. Open Solution File `StpToStl.sln` in Microsoft Visual Studio 2017



## Example of using in .NET (C#)

```
public static class JStpToStl
{

  [DllImport(@"Libs_x32\StpToStl.dll", EntryPoint = "StpToStlW", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
  static extern int StpToStl(string in_file, string out_file, double deflection = 0, bool ascii = false);

  ///****************************************************************************
  /// <summary>
  /// Convert ISO 10303 STEP file (AP203, AP214) (.stp) to StereoLithography file (.stl)
  /// </summary>
  /// <param name="in_file">Input file name</param>
  /// <param name="out_file">Output file name</param>
  /// <param name="deflection">Linear deflection, default value is 0.001</param>
  /// <param name="ascii">The generated STL file is an ASCII file</param>
  /// <returns></returns>
  public static int StepToStl(string in_file, string out_file, double deflection = 0, bool ascii = false)
  {
    return StpToStl(in_file, out_file, deflection, ascii);
  }

}
```

