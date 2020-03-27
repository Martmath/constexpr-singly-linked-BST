using System;
using System.Collections.Generic;
 using System.Diagnostics; 
using System.IO;
using System.Linq;
//using System.Text;using System.Threading.Tasks;
//using System.Text.Json;//using System.Text.Json.Serialization;
//using System.Runtime.Serialization.Json; using System.Web.Script.Serialization; 
//using System.Runtime.Serialization; using System.Runtime.Serialization.Formatters.Binary; 
using System.Xml.Serialization; 
//using System.Windows.Forms;
//using System.Collections;

namespace ConsoleAppl
{
    // using SPair = Tuple<string, string>;//class F{ public string A;public string B;} 
    [Serializable]
    public class MO
    { // public int n1 ;[NonSerialized] public int n2;
        public string fname; public DateTime dt;
        public bool Equals(MO other)
        {
            if (other is null) return false;
            return this.fname == other.fname && this.dt == other.dt;
        }
        public override bool Equals(object obj) => Equals(obj as MO);
        public override int GetHashCode() => (fname, dt).GetHashCode();
    }
    public class CComparer : IComparer<MO>
    {
        public int Compare(MO a, MO b)
        { if ((a.fname == b.fname) && (a.dt == b.dt)) return 0; return 1; }
    }

    namespace T4Run
    {
        class Program
        {//SPair srf=new SPairfsdf", "safda"); //static F[] a = new F[2] { new F { A = "qq", B = "ww" }, new F { A = "qq", B = "ww" } };
         //"$(DevEnvDir)TextTransform.exe" -out "$(ProjectDir)%f%.cpp" "$(ProjectDir)%f%.tt"
         //"C:\Program Files (x86)\Microsoft Visual Studio\2019\Common7\IDE\TextTransform.exe" -out "C:\Users\User\source\repos\ConsoleApplication6\T4Run\bin\Debug\%f%.cpp" "C:\Users\User\source\repos\ConsoleApplication6\T4Run\bin\Debug\%f%.tt"
            static void Main(string[] args)
            {                  
                Console.WriteLine("Number Of Cores: {0}, Number Of Logical Processors: {1} ", CoresCount, Environment.ProcessorCount);                

                var Z = ((char)(34)).ToString(); var Q = Environment.CurrentDirectory; var D0 = args[0];
               // "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\Common7\\IDE\\TextTransform.exe";
                var D = String.Join(" ", args.Skip(1));
                        //.Replace(".cpp", ".cpp" + Z).Replace(".tt", ".tt" + Z)
                        //.Replace("c:\\", Z + "c:\\").Replace("C:\\", Z + "C:\\"); // string H1 = "C:\\&f&.cpp C:\\mo"; D = H1; 
 //"-out C:\\Users\\User\\source\\repos\\ConsoleApplication6\\ConsoleApplication6\\&f&.h C:\\Users\\User\\source\\repos\\ConsoleApplication6\\ConsoleApplication6\\&f&.tt";
                //MessageBox.Show(D);  //return;

                bool noQ = false; string res = ""; int da = 0;
                for (int i = 0; i < D.Length; i++)
                {
                    if (D[i] == '.') noQ = true;
                    else if (D[i] == ':') { res = res.Substring(0, i + da - 1) + Z + D[i - 1]; da++; }
                    else if ((D[i] == ' ') && noQ) { res = res + Z; noQ = false; da++; }
                    else if (D[i] == '\\') noQ = false; res = res + D[i];
                }
                if (noQ) res = res + Z; D = res;
                // Console.WriteLine(D); Console.Read(); MessageBox.Show(D); return;
                /* IFormatter f1 = new BinaryFormatter();
                Stream s = new FileStream("MyF.bin", FileMode.Open, FileAccess.Read, FileShare.Read);
                var js = new JavaScriptSerializer(); DataContractJsonSerializer JsonSerializer.Serialize(*/
                var filesList = Directory.GetFiles(Q, "*.tt").Select(x => new MO
                {
                    fname = Path.GetFileNameWithoutExtension(x),
                    dt = Directory.GetLastWriteTime(x)
                }).ToList();
           
                var dataTT = new List<MO>(); 
                if (File.Exists("datatt.xml"))
                {
                    XmlSerializer xs = new XmlSerializer(filesList.GetType());
                    try
                    {
                        using (FileStream fs = new FileStream("datatt.xml", FileMode.OpenOrCreate))
                        {
                            // var qqqw = (List<MO>)xs.Deserialize(fs);
                            dataTT.AddRange(((List<MO>)xs.Deserialize(fs)));
                        }
                    }
                    catch { File.Delete("datatt.xml"); }
                }
                var filesListNew = filesList
                     .Except(dataTT)
                     .ToList();
                Console.WriteLine("Number Of Files: {0}, Number Of modified files: {1} ", filesList.Count, filesListNew.Count);
                var filesListCreate = filesListNew.Select(x => D.Replace("&f&", x.fname)).ToList();
                // MessageBox.Show (qqe1[0]); MessageBox.Show(D0);
                //parrallel proc is ok, too mach procceses is not good
                int N = Convert.ToInt32(Math.Round(Convert.ToDouble(Environment.ProcessorCount) * 1.2,0)); 
                //5;// 1;//round(Environment.ProcessorCount*1.2);
                if (N > filesListCreate.Count) N = filesListCreate.Count;

                /*List<string> tst = new List<string>() { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
                if (N > tst.Count) N = tst.Count;         
                List<List<string>> QQ = new List<List<string>>();
                for (int i = 0; i < N; i++) QQ.Add(ThreadFromProcesses(tst, i, N).Item2);  */ 
                
                var baseStruct = RunnerBaseStruct(filesListCreate, N).ToList();
                List<Process> ps = new List<Process>();

                Stopwatch sw = new Stopwatch();
                sw.Start();
                for (int i = 0; i < baseStruct.Count; i++)
                {
                    try
                    {                       
                        Process exeProcess = BaseProcess(D0, baseStruct[i].Item1);//Verb = "tttt"
                        if (baseStruct[i].Item2.Count != 0)
                        {
                            exeProcess.EnableRaisingEvents = true;
                            exeProcess.Exited += new System.EventHandler(ThreadFromProcessesCreate(D0, baseStruct[i].Item2));
                        }
                        ps.Add(exeProcess);
                        exeProcess.Start();
                    }
                    catch { }
                }
                foreach (var process in ps) { process.WaitForExit(); process.Dispose(); }
                sw.Stop();             
                Console.WriteLine("Procs complete in '{0}' ms", sw.ElapsedMilliseconds);

                XmlSerializer xss = new XmlSerializer(filesList.GetType());
                using (FileStream fs = new FileStream("datatt.xml", FileMode.OpenOrCreate))
                { xss.Serialize(fs, filesList); fs.Close(); }
            }
            static int CoresCount
            {
                get
                {
                    int coreCount = 0;
                    foreach (var item in new System.Management.ManagementObjectSearcher("Select * from Win32_Processor").Get())
                        coreCount += int.Parse(item["NumberOfCores"].ToString());
                    return coreCount;
                }
            }  
            static Action<object, EventArgs> ThreadFromProcessesCreate(string P, List<string> l)
            {
                Action<object, EventArgs> ww = ProcessZero(P, l[l.Count - 1]);
                for (int i = l.Count - 2; i >= 0; i--)
                    ww = ProcessNode(P, l[i], ww);
                return ww;
            }
            static IEnumerable<(string, List<string>)> RunnerBaseStruct(List<string> tst, int N)
            {
                if (N > tst.Count) N = tst.Count;
                for (int i = 0; i < N; i++) yield return ThreadFromProcesses(tst, i, N);
            }
            //https://stackoverflow.com/questions/58410905/enumerating-list-faster-than-ilist-icollection-and-ienumerable
            static (string, List<string>) ThreadFromProcesses(List<string> l, int n, int N)
            {
                string res0 = l[n];
                List<string> res1 = new List<string>(); // n += N;
                while ((n += N) < l.Count) res1.Add(l[n]); //N=4 //0,4,8
                return (res0, res1);
            }           
            static Process BaseProcess(string fName, string Arg) => new Process {
                StartInfo = new ProcessStartInfo
                {
                    CreateNoWindow = false,
                    UseShellExecute = false,
                    FileName = fName,
                    WindowStyle = ProcessWindowStyle.Hidden,
                    Arguments = Arg
                }
            }; 
            static Action<object, EventArgs> ProcessNode(string fName, string Arg, Action<object, EventArgs> action)
            {
                return (x, y) =>
                {                    
                    Process processZero = BaseProcess(fName, Arg);
                    processZero.EnableRaisingEvents = true;
                    processZero.Exited += new System.EventHandler(action);
                    processZero.Start();
                    processZero.WaitForExit();
                    processZero.Dispose();
                };
            }
           // http://tomasp.net/blog/cannot-return-anonymous-type-from-method.aspx/
            static Action<object, EventArgs> ProcessZero(string fName, string Arg)
            {
                return (x, y) =>
                {
                    Process prc = BaseProcess(fName, Arg);
                    prc.Start();
                    prc.WaitForExit();
                    prc.Dispose();
                };
            }
        }
    }
}
