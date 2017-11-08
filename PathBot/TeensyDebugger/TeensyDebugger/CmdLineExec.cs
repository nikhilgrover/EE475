using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Threading;

namespace TeensyDebugger
{
    public delegate void ProcessEventHander(object sender, ProcessEventArgs e);

    public class ProcessEventArgs : EventArgs
    {
        public string Output { get; set; }
        public string Error { get; set; }
        public ProcessEventArgs(string output, string error)
        {
            this.Output = output;
            this.Error = error;
        }
    }

    class CmdLineExec
    {
        string[] cmds;

        public CmdLineExec(string[] cmds)
        {
            this.cmds = cmds;
        }

        public event ProcessEventHander ProcessOutputHandler;
        public event Action OnComplete;
        Task taskCompile = null;
        CancellationTokenSource source = new CancellationTokenSource();
        bool compileStarted;
        public bool Run()
        {
            if (taskCompile != null && !taskCompile.IsCompleted)
            {
                return false;//last compile task is still running
            }

            taskCompile = new Task((Action)(() => ExecRoutine(source.Token)), source.Token);
            taskCompile.Start();
            Debug.WriteLine("Compile Exited");
            return true;
        }

        public bool Cancel()
        {
            source.Cancel();
            if (taskCompile != null)
                taskCompile.Wait();
            
            return true;
        }
        //public event EventHandler Update

        async void ExecRoutine(CancellationToken Token)
        {
            Process proc = new Process();
            proc.StartInfo.FileName = "CMD.exe";
            proc.StartInfo.RedirectStandardOutput = true;
            proc.StartInfo.RedirectStandardInput = true;
            proc.StartInfo.RedirectStandardError = true;
            proc.StartInfo.UseShellExecute = true;

            proc.StartInfo.CreateNoWindow = true;
            proc.StartInfo.UseShellExecute = false;
            proc.Start();

            Task cmd = new Task((Action)(() =>
            {
                for (int i = 0; i < cmds.Length; i++)
                {
                    proc.StandardInput.WriteLine(cmds[i]);
                }

            }));

            cmd.Start();

            while (!Token.IsCancellationRequested)
            {
                if (proc.HasExited)
                    break;


                if (ProcessOutputHandler != null)
                     ProcessOutputHandler.Invoke(this, new ProcessEventArgs(proc.StandardOutput.ReadLine(), null));

                proc.Refresh();
                await Task.Delay(1);
            }


            if (ProcessOutputHandler != null)
                ProcessOutputHandler.Invoke(this, new ProcessEventArgs( null, proc.StandardError.ReadToEnd()));

            if (!proc.HasExited)
                proc.Kill();
            if (OnComplete != null)
                OnComplete.Invoke();
        }

    }
}
