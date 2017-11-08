using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TeensyDebugger
{
    public partial class StackControl : UserControl
    {

        [DllImport("USER32.DLL")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

    


        public StackControl()
        {
            InitializeComponent();
        }

        bool loaded = false;
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            loaded = true;
            this.Invoke((Action)(() => lnkLbl.Text = filepath + ":" + line.ToString()));
            this.Invoke((Action)(() => lblLabel.Text = label));
        }

        string filepath = "";
        long line = 0;

        string label = "";
        public string Filepath
        {
            get
            {
                return filepath;
            }
            set
            {
                filepath = value;
                if(loaded)   
                this.Invoke((Action)(() => lnkLbl.Text = filepath + ":" + line.ToString()));
            }
        }
        public long Line
        {
            get
            {
                return line;
            }
            set
            {
                line = value;
                if(loaded)
                this.Invoke((Action)(() => lnkLbl.Text = filepath + ":" + line.ToString()));
            }
        }
        public string Label
        {
            get
            {
                return label;
            }
            set
            {
                label = value;
                if (loaded)
                this.Invoke((Action)(() => lblLabel.Text = label));
            }
        }
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public PacketStateMachine Server { get; set; }


        private void lnkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            if (loaded && Server!=null)
            Server.Send(Encoding.ASCII.GetBytes(filepath + "|" + line.ToString()));

            Process[] sublime = Process.GetProcessesByName("sublime_text");
            if(sublime == null || sublime.Length==0)
                return;

            SetForegroundWindow(sublime[0].MainWindowHandle);
        }
    }
}
