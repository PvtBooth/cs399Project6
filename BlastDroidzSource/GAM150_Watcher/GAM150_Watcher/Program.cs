using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.Win32.SafeHandles;

namespace GAM150_Watcher
{
  static class Program
  {
    public static string logFile;
    public static string zipFile;

    //[DllImport("kernel32.dll",
    //        EntryPoint = "GetStdHandle",
    //        SetLastError = true,
    //        CharSet = CharSet.Auto,
    //        CallingConvention = CallingConvention.StdCall)]
    //private static extern IntPtr GetStdHandle(int nStdHandle);
    //[DllImport("kernel32.dll",
    //    EntryPoint = "AllocConsole",
    //    SetLastError = true,
    //    CharSet = CharSet.Auto,
    //    CallingConvention = CallingConvention.StdCall)]
    //private static extern int AllocConsole();
    //private const int STD_OUTPUT_HANDLE = -11;
    //private const int MY_CODE_PAGE = 437;

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main(string[] args)
    {

      //AllocConsole();
      //IntPtr stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      //SafeFileHandle safeFileHandle = new SafeFileHandle(stdHandle, true);
      //FileStream fileStream = new FileStream(safeFileHandle, FileAccess.Write);
      //Encoding encoding = System.Text.Encoding.GetEncoding(MY_CODE_PAGE);
      //StreamWriter standardOutput = new StreamWriter(fileStream, encoding);
      //standardOutput.AutoFlush = true;
      //Console.SetOut(standardOutput);
      //
      //Console.WriteLine("DEBUG");

      //Console.Read();

      if (args.Length < 2)
      {
        //For testing the form(s) without running the program.
        //Application.EnableVisualStyles();
        //Application.SetCompatibleTextRenderingDefault(false);
        //Application.Run(new FormPreAlpha());
        return;
      }

      Process processes = Process.GetProcessById(Convert.ToInt32(args[0]));

      logFile = args[1];
      zipFile = logFile;
      zipFile = zipFile.Replace(".log", ".zip");
      
      //Console.WriteLine("ARGUMENT 0: " + args[0]);
      //Console.WriteLine("ARGUMENT 1: " + args[1]);
      //Console.WriteLine(".log FILE PATH: " + logFile);
      //Console.WriteLine(".zip FILE PATH: " + zipFile);

      processes.WaitForExit();
      
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new FormPreAlpha());

      //Console.Read();
    }
  }
}
