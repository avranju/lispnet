using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Nerdworks.Lisp.Runtime;

namespace LispShell
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                //
                // boot lisp
                //
                LispRuntime.Boot();

                //
                // create a runtime object and keep evaluating till "quit" is entered
                //
                using (LispRuntime runtime = new LispRuntime())
                {
                    //
                    // setup console to read up to 10K of code
                    //
                    int BUFFER_SIZE = 1024 * 10;
                    byte[] buffer = new byte[BUFFER_SIZE];
                    System.IO.Stream input = Console.OpenStandardInput(BUFFER_SIZE);

                    string form;
                    EvalResult result;
                    Console.Write("> ");
                    while ((form = ReadLine(input, buffer)) != "quit\r\n")
                    {
                        result = runtime.Eval(form);
                        Console.WriteLine(result.ToString());
                        Console.Write("> ");
                    }
                }

                //
                // shutdown lisp
                //
                LispRuntime.Shutdown();
            }
            catch (Exception ex)
            {
                Console.WriteLine("ERROR: {0}", ex.ToString());
            }
        }

        static string ReadLine(Stream input, byte[] buffer)
        {
            int length = input.Read(buffer, 0, buffer.Length);
            return System.Text.Encoding.UTF7.GetString(buffer, 0, length);
        }
    }
}
